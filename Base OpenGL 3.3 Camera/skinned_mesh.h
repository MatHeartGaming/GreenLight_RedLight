#pragma once
#include <map>
#include <memory>
#include <string>
#include <vector>

//#include "glad.h"
//#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/rotate_vector.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <assimp/Importer.hpp>  // C++ importer interface
#include <assimp/postprocess.h> // Post processing flags
#include <assimp/scene.h>       // Output data structure
#include <glm/gtx/quaternion.hpp>

#include "glad/glad.h"
#include "texture.h"

class SkinnedMesh
{
public:
	SkinnedMesh();
	~SkinnedMesh();
	bool     loadMesh(const std::string& filename);
	void     render();
	unsigned int getCountBones() const { return countBones; }

	void boneTransform(const float             timeInSeconds,
		std::vector<glm::mat4>& transforms);

private:
#define NUM_BONES_PER_VEREX 4

	unsigned int countBones = 0;

	struct BoneInfo
	{
		glm::mat4 boneOffset;
		glm::mat4 finalTransformation;
	};

	struct VertexBoneData
	{
		unsigned int      ids[NUM_BONES_PER_VEREX] = { 0 };
		float    weights[NUM_BONES_PER_VEREX] = { 0.0 };

		VertexBoneData()
		{
			for (unsigned int i = 0; i < NUM_BONES_PER_VEREX; i++)
			{
				ids[i] = 0;
				weights[i] = 0.0;
			}
		}

		void addBoneData(unsigned int BoneID, float Weight);
	};

	struct Vertex
	{
		glm::vec3      position;
		glm::vec3      normal;
		glm::vec2      textureCoords;
		VertexBoneData boneData;
	};

#define INVALID_MATERIAL 0xFFFFFFFF

	struct MeshEntry
	{
		unsigned int               materialIndex = INVALID_MATERIAL;
		std::vector<unsigned int>  indices;
		std::vector<Vertex>        vertices;
		unsigned int               vao = 0;
		unsigned int               vertexBufferId = 0;
		unsigned int               indexBufferId = 0;
	};

	std::vector<BoneInfo> boneInfos;

	void calcInterpolatedScaling(glm::vec3& out,
		float             animationTime,
		const aiNodeAnim* pNodeAnim);

	void calcInterpolatedRotation(glm::quat& out,
		float             animationTime,
		const aiNodeAnim* pNodeAnim);

	void calcInterpolatedPosition(glm::vec3& out,
		float             animationTime,
		const aiNodeAnim* pNodeAnim);

	unsigned int findScaling(float animationTime, const aiNodeAnim* pNodeAnim);

	unsigned int findRotation(float animationTime, const aiNodeAnim* pNodeAnim);

	unsigned int findPosition(float animationTime, const aiNodeAnim* pNodeAnim);

	const aiNodeAnim* findNodeAnim(const aiAnimation* pAnimation,
		const std::string  nodeName);

	void readNodeHeirarchy(float            animationTime,
		const aiNode* pNode,
		const glm::mat4& parentTransform);

	bool initFromScene(const aiScene* pScene, const std::string& filename);

	void initMesh(const aiMesh* paiMesh, MeshEntry& mesh);

	void loadBones(const aiMesh* paiMesh, MeshEntry& mesh);

	bool initMaterials(const aiScene* pScene, const std::string& filename);

	enum VB_TYPES
	{
		INDEX_BUFFER,
		POS_VB,
		NORMAL_VB,
		TEXCOORD_VB,
		BONE_VB,
		NUM_VBs
	};

	std::vector<MeshEntry> entries;
	std::vector<Texture*> textures;

	std::map<std::string, unsigned int> boneMapping; // maps a bone name to its index
	unsigned int                        numBones = 0;
	glm::mat4                       globalInverseTransform;

	const aiScene* pScene;
	Assimp::Importer importer;
};


