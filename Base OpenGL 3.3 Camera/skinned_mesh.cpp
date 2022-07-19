#include <iostream>

#include "skinned_mesh.h"
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

//#include <glm/gtc/type_ptr.hpp>

#define ASSIMP_LOAD_FLAGS                                                      \
  (aiProcess_Triangulate | aiProcess_GenSmoothNormals | aiProcess_FlipUVs |    \
   aiProcess_JoinIdenticalVertices)

#define ARRAY_SIZE_IN_ELEMENTS(a) (sizeof(a) / sizeof(a[0]))

#define POSITION_LOCATION    0
#define TEX_COORD_LOCATION   1
#define NORMAL_LOCATION      2
#define BONE_ID_LOCATION     3
#define BONE_WEIGHT_LOCATION 4

unsigned int animationIndex = 0;

static inline glm::vec3 vec3_cast(const aiVector3D& v)
{
	return glm::vec3(v.x, v.y, v.z);
}

static inline glm::vec2 vec2_cast(const aiVector3D& v)
{
	// it's aiVector3D because assimp's texture coordinates use that
	return glm::vec2(v.x, v.y);
}

static inline glm::quat quat_cast(const aiQuaternion& q)
{
	return glm::quat(q.w, q.x, q.y, q.z);
}

static inline glm::mat4 mat4_cast(const aiMatrix4x4& m)
{
	return glm::transpose(glm::make_mat4(&m.a1));
}

SkinnedMesh::SkinnedMesh() {}

SkinnedMesh::~SkinnedMesh() {}

bool SkinnedMesh::loadMesh(const std::string& filename)
{
	bool ret = false;

	pScene = importer.ReadFile(filename.c_str(), ASSIMP_LOAD_FLAGS);

	if (pScene)
	{
		globalInverseTransform =
			glm::inverse(mat4_cast(pScene->mRootNode->mTransformation));
		ret = initFromScene(pScene, filename);
	}
	else
	{
		printf("Error parsing '%s': '%s'\n",
			filename.c_str(),
			importer.GetErrorString());
	}

	// Make sure the VAO is not changed from the outside
	glBindVertexArray(0);

	return ret;
}

void SkinnedMesh::VertexBoneData::addBoneData(unsigned int BoneID, float Weight)
{
	for (unsigned int i = 0; i < ARRAY_SIZE_IN_ELEMENTS(ids); i++)
	{
		if (weights[i] == 0.0)
		{
			ids[i] = BoneID;
			weights[i] = Weight;
			return;
		}
	}

	// should never get here - more bones than we have space for
	// assert(0);
}

void SkinnedMesh::render()
{
	for (const auto& mesh : entries)
	{
		glBindVertexArray(mesh.vao);

		const auto materialIndex = mesh.materialIndex;

		assert(materialIndex < textures.size());

		if (textures[materialIndex])
		{
			textures[materialIndex]->Bind(GL_TEXTURE1);
		}

		glDrawElements(GL_TRIANGLES, mesh.indices.size(), GL_UNSIGNED_INT, 0);

		glBindVertexArray(0);
	}
}

void SkinnedMesh::boneTransform(const float timeInSeconds, std::vector<glm::mat4>& transforms)
{
	glm::mat4 Identity(1.0f);

	float TicksPerSecond =
		(float)(pScene->mAnimations[animationIndex]->mTicksPerSecond != 0
			? pScene->mAnimations[animationIndex]->mTicksPerSecond
			: 25.0f);
	float duration = pScene->mAnimations[animationIndex]->mDuration;

	float TimeInTicks = timeInSeconds * TicksPerSecond;

	float AnimationTime = fmod(TimeInTicks, duration);

	readNodeHeirarchy(AnimationTime, pScene->mRootNode, Identity);

	transforms.resize(boneInfos.size());

	for (unsigned int i = 0; i < boneInfos.size(); i++)
	{
		transforms[i] = boneInfos[i].finalTransformation;
	}
}

void SkinnedMesh::calcInterpolatedScaling(glm::vec3& out,
	float             animationTime,
	const aiNodeAnim* pNodeAnim)
{
	if (pNodeAnim->mNumScalingKeys == 1)
	{
		out = vec3_cast(pNodeAnim->mScalingKeys[0].mValue);
		return;
	}

	unsigned int ScalingIndex = findScaling(animationTime, pNodeAnim);
	unsigned int NextScalingIndex = (ScalingIndex + 1);
	assert(NextScalingIndex < pNodeAnim->mNumScalingKeys);
	float DeltaTime = (float)(pNodeAnim->mScalingKeys[NextScalingIndex].mTime -
		pNodeAnim->mScalingKeys[ScalingIndex].mTime);
	float Factor =
		(animationTime - (float)pNodeAnim->mScalingKeys[ScalingIndex].mTime) /
		DeltaTime;
	assert(Factor >= 0.0f && Factor <= 1.0f);
	const auto& Start = vec3_cast(pNodeAnim->mScalingKeys[ScalingIndex].mValue);
	const auto& End = vec3_cast(pNodeAnim->mScalingKeys[NextScalingIndex].mValue);
	auto        Delta = End - Start;

	out = Start + Factor * Delta;
}

void SkinnedMesh::calcInterpolatedRotation(glm::quat& out,
	float             animationTime,
	const aiNodeAnim* pNodeAnim)
{
	// we need at least two values to interpolate...
	if (pNodeAnim->mNumRotationKeys == 1)
	{
		out = quat_cast(pNodeAnim->mRotationKeys[0].mValue);
		return;
	}

	unsigned int RotationIndex = findRotation(animationTime, pNodeAnim);
	unsigned int NextRotationIndex = (RotationIndex + 1);
	assert(NextRotationIndex < pNodeAnim->mNumRotationKeys);
	float DeltaTime = (float)(pNodeAnim->mRotationKeys[NextRotationIndex].mTime -
		pNodeAnim->mRotationKeys[RotationIndex].mTime);
	float Factor =
		(animationTime - (float)pNodeAnim->mRotationKeys[RotationIndex].mTime) /
		DeltaTime;
	assert((animationTime -
		(float)pNodeAnim->mRotationKeys[RotationIndex].mTime) >= 0.0f);
	assert(DeltaTime >= 0.0f);
	assert(Factor >= 0.0f && Factor <= 1.0f);
	const auto& StartRotationQ =
		quat_cast(pNodeAnim->mRotationKeys[RotationIndex].mValue);
	const auto& EndRotationQ =
		quat_cast(pNodeAnim->mRotationKeys[NextRotationIndex].mValue);
	out = glm::mix(StartRotationQ, EndRotationQ, Factor);

	out = glm::normalize(out);
}

void SkinnedMesh::calcInterpolatedPosition(glm::vec3& out,
	float             animationTime,
	const aiNodeAnim* pNodeAnim)
{
	if (pNodeAnim->mNumPositionKeys == 1)
	{
		out = vec3_cast(pNodeAnim->mPositionKeys[0].mValue);
		return;
	}

	unsigned int PositionIndex = findPosition(animationTime, pNodeAnim);
	unsigned int NextPositionIndex = (PositionIndex + 1);
	assert(NextPositionIndex < pNodeAnim->mNumPositionKeys);
	float DeltaTime = (float)(pNodeAnim->mPositionKeys[NextPositionIndex].mTime -
		pNodeAnim->mPositionKeys[PositionIndex].mTime);
	float Factor =
		(animationTime - (float)pNodeAnim->mPositionKeys[PositionIndex].mTime) /
		DeltaTime;
	assert(Factor >= 0.0f && Factor <= 1.0f);
	const auto& Start = vec3_cast(pNodeAnim->mPositionKeys[PositionIndex].mValue);
	const auto& End =
		vec3_cast(pNodeAnim->mPositionKeys[NextPositionIndex].mValue);
	auto Delta = End - Start;

	out = Start + Factor * Delta;
}

unsigned int SkinnedMesh::findScaling(float             animationTime,
	const aiNodeAnim* pNodeAnim)
{
	assert(pNodeAnim->mNumScalingKeys > 0);

	for (unsigned int i = 0; i < pNodeAnim->mNumScalingKeys - 1; i++)
	{
		if (animationTime < (float)pNodeAnim->mScalingKeys[i + 1].mTime)
		{
			return i;
		}
	}

	assert(0);

	return 0;
}

unsigned int SkinnedMesh::findRotation(float             animationTime,
	const aiNodeAnim* pNodeAnim)
{
	assert(pNodeAnim->mNumRotationKeys > 0);

	for (unsigned int i = 0; i < pNodeAnim->mNumRotationKeys - 1; i++)
	{
		if (animationTime < (float)pNodeAnim->mRotationKeys[i + 1].mTime)
		{
			return i;
		}
	}

	assert(0);

	return 0;
}

unsigned int SkinnedMesh::findPosition(float             AnimationTime,
	const aiNodeAnim* pNodeAnim)
{
	for (unsigned int i = 0; i < pNodeAnim->mNumPositionKeys - 1; i++)
	{
		if (AnimationTime < (float)pNodeAnim->mPositionKeys[i + 1].mTime)
		{
			return i;
		}
	}

	assert(0);

	return 0;
}

const aiNodeAnim* SkinnedMesh::findNodeAnim(const aiAnimation* pAnimation,
	const std::string  nodeName)
{
	for (unsigned int i = 0; i < pAnimation->mNumChannels; i++)
	{
		const aiNodeAnim* pNodeAnim = pAnimation->mChannels[i];

		if (std::string(pNodeAnim->mNodeName.data) == nodeName)
		{
			return pNodeAnim;
		}
	}

	return NULL;
}

void SkinnedMesh::readNodeHeirarchy(float            animationTime,
	const aiNode* pNode,
	const glm::mat4& parentTransform)
{
	std::string NodeName(pNode->mName.data);

	const aiAnimation* pAnimation = pScene->mAnimations[animationIndex];

	glm::mat4 NodeTransformation(mat4_cast(pNode->mTransformation));

	const aiNodeAnim* pNodeAnim = findNodeAnim(pAnimation, NodeName);

	if (pNodeAnim)
	{
		// Interpolate scaling and generate scaling transformation matrix
		glm::vec3 Scaling;
		calcInterpolatedScaling(Scaling, animationTime, pNodeAnim);
		glm::mat4 ScalingM = glm::scale(glm::mat4(1.0f), Scaling);

		// Interpolate rotation and generate rotation transformation matrix
		glm::quat RotationQ;
		calcInterpolatedRotation(RotationQ, animationTime, pNodeAnim);
		glm::mat4 RotationM =
			glm::toMat4(RotationQ); // Matrix4f(RotationQ.GetMatrix());

		// Interpolate translation and generate translation transformation matrix
		glm::vec3 Translation;
		calcInterpolatedPosition(Translation, animationTime, pNodeAnim);
		glm::mat4 TranslationM = glm::translate(glm::mat4(1.0f), Translation);

		// Combine the above transformations
		NodeTransformation = TranslationM * RotationM * ScalingM;
	}

	glm::mat4 GlobalTransformation = parentTransform * NodeTransformation;

	if (boneMapping.find(NodeName) != boneMapping.end())
	{
		unsigned int BoneIndex = boneMapping[NodeName];
		boneInfos[BoneIndex].finalTransformation = globalInverseTransform *
			GlobalTransformation *
			boneInfos[BoneIndex].boneOffset;
	}

	for (unsigned int i = 0; i < pNode->mNumChildren; i++)
	{
		readNodeHeirarchy(animationTime, pNode->mChildren[i], GlobalTransformation);
	}
}

bool SkinnedMesh::initFromScene(const aiScene* pScene,
	const std::string& filename)
{
	entries.resize(pScene->mNumMeshes);
	textures.resize(pScene->mNumMaterials);

	// Initialize the meshes in the scene one by one
	for (unsigned int i = 0; i < entries.size(); i++)
	{
		const aiMesh* paiMesh = pScene->mMeshes[i];
		entries[i].materialIndex = pScene->mMeshes[i]->mMaterialIndex;
		initMesh(paiMesh, entries[i]);
	}

	if (!initMaterials(pScene, filename))
	{
		return false;
	}

	for (auto& mesh : entries)
	{
		// Generate and populate the buffers with vertex attributes and thise
		// indices
		glGenVertexArrays(1, &mesh.vao);
		glBindVertexArray(mesh.vao);

		glGenBuffers(1, &mesh.vertexBufferId);
		glGenBuffers(1, &mesh.indexBufferId);

		glBindBuffer(GL_ARRAY_BUFFER, mesh.vertexBufferId);
		glBufferData(GL_ARRAY_BUFFER,
			sizeof(Vertex) * mesh.vertices.size(),
			&mesh.vertices[0],
			GL_STATIC_DRAW);

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh.indexBufferId);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER,
			mesh.indices.size() * sizeof(unsigned int),
			&mesh.indices[0],
			GL_STATIC_DRAW);

		glEnableVertexAttribArray(POSITION_LOCATION);
		glVertexAttribPointer(POSITION_LOCATION,
			3,
			GL_FLOAT,
			GL_FALSE,
			sizeof(Vertex),
			reinterpret_cast<void*>(offsetof(Vertex, position)));

		glEnableVertexAttribArray(NORMAL_LOCATION);
		glVertexAttribPointer(NORMAL_LOCATION,
			3,
			GL_FLOAT,
			GL_FALSE,
			sizeof(Vertex),
			reinterpret_cast<void*>(offsetof(Vertex, normal)));

		glEnableVertexAttribArray(TEX_COORD_LOCATION);
		glVertexAttribPointer(
			TEX_COORD_LOCATION,
			2,
			GL_FLOAT,
			GL_FALSE,
			sizeof(Vertex),
			reinterpret_cast<void*>(offsetof(Vertex, textureCoords)));

		glEnableVertexAttribArray(BONE_ID_LOCATION);
		glVertexAttribIPointer(
			BONE_ID_LOCATION,
			NUM_BONES_PER_VEREX,
			GL_INT,
			sizeof(Vertex),
			reinterpret_cast<void*>(offsetof(Vertex, boneData) +
				offsetof(VertexBoneData, ids)));

		glEnableVertexAttribArray(BONE_WEIGHT_LOCATION);
		glVertexAttribPointer(
			BONE_WEIGHT_LOCATION,
			NUM_BONES_PER_VEREX,
			GL_FLOAT,
			GL_FALSE,
			sizeof(Vertex),
			reinterpret_cast<void*>(offsetof(Vertex, boneData) +
				offsetof(VertexBoneData, weights)));

		glBindVertexArray(0);
	}

	return (glGetError() == GL_NO_ERROR);
}

void SkinnedMesh::initMesh(const aiMesh* paiMesh, MeshEntry& mesh)
{
	const aiVector3D zero3d(0.0f, 0.0f, 0.0f);

	// Populate the vertex attribute vectors
	for (unsigned int i = 0; i < paiMesh->mNumVertices; i++)
	{
		const aiVector3D* pPos = &(paiMesh->mVertices[i]);
		const aiVector3D* pNormal = &(paiMesh->mNormals[i]);
		const aiVector3D* pTexCoord = paiMesh->HasTextureCoords(0)
			? &(paiMesh->mTextureCoords[0][i])
			: &zero3d;

		Vertex vertex;
		vertex.position = vec3_cast(*pPos);
		vertex.normal = vec3_cast(*pNormal);
		vertex.textureCoords = vec2_cast(*pTexCoord);

		mesh.vertices.push_back(vertex);
	}

	loadBones(paiMesh, mesh);

	// Populate the index buffer
	for (unsigned int i = 0; i < paiMesh->mNumFaces; i++)
	{
		const aiFace& Face = paiMesh->mFaces[i];
		assert(Face.mNumIndices == 3);

		mesh.indices.push_back(Face.mIndices[0]);
		mesh.indices.push_back(Face.mIndices[1]);
		mesh.indices.push_back(Face.mIndices[2]);
	}
}

void SkinnedMesh::loadBones(const aiMesh* paiMesh, MeshEntry& mesh)
{
	for (unsigned int i = 0; i < paiMesh->mNumBones; i++)
	{
		unsigned int boneIndex = 0;
		std::string boneName(paiMesh->mBones[i]->mName.data);

		if (boneMapping.find(boneName) == boneMapping.end())
		{
			// Allocate an index for a new bone
			boneIndex = numBones;
			numBones++;

			BoneInfo bi;
			boneInfos.push_back(bi);
			boneInfos[boneIndex].boneOffset =
				mat4_cast(paiMesh->mBones[i]->mOffsetMatrix);

			boneMapping[boneName] = boneIndex;
		}
		else
		{
			boneIndex = boneMapping[boneName];
		}

		for (unsigned int j = 0; j < paiMesh->mBones[i]->mNumWeights; j++)
		{
			unsigned int  vertexId = paiMesh->mBones[i]->mWeights[j].mVertexId;
			float weight = paiMesh->mBones[i]->mWeights[j].mWeight;

			mesh.vertices[vertexId].boneData.addBoneData(boneIndex, weight);
		}
	}
}

bool SkinnedMesh::initMaterials(const aiScene* pScene,
	const std::string& filename)
{
	// Extract the directory part from the file name
	std::string::size_type SlashIndex = filename.find_last_of("/");
	std::string            Dir;

	if (SlashIndex == std::string::npos)
	{
		Dir = ".";
	}
	else if (SlashIndex == 0)
	{
		Dir = "/";
	}
	else
	{
		Dir = filename.substr(0, SlashIndex);
	}

	bool Ret = true;

	// Initialize the materials
	for (unsigned int i = 0; i < pScene->mNumMaterials; i++)
	{
		const aiMaterial* pMaterial = pScene->mMaterials[i];

		textures[i] = NULL;

		if (pMaterial->GetTextureCount(aiTextureType_DIFFUSE) > 0)
		{
			aiString Path;

			if (pMaterial->GetTexture(aiTextureType_DIFFUSE,
				0,
				&Path,
				NULL,
				NULL,
				NULL,
				NULL,
				NULL) == AI_SUCCESS)
			{
				std::string p(Path.data);

				if (p.substr(0, 2) == ".\\")
				{
					p = p.substr(2, p.size() - 2);
				}

				std::string FullPath = Dir + "/" + p;

				textures[i] = new Texture(GL_TEXTURE_2D, FullPath.c_str());

				if (!textures[i]->Load())
				{
					printf("Error loading texture '%s'\n", FullPath.c_str());
					delete textures[i];
					textures[i] = NULL;
					Ret = false;
				}
				else
				{
					printf("%d - loaded texture '%s'\n", i, FullPath.c_str());
				}
			}
		}
	}

	return Ret;
}
