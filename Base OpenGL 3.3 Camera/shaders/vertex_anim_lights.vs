#version 330

layout(location = 0) in vec3 position;
layout(location = 1) in vec2 texCoords;
layout(location = 2) in vec3 normal;
layout(location = 3) in ivec4 boneIds;
layout(location = 4) in vec4 weights;

out vec2 fragTexCoords;
out vec3 FragPos;
out vec3 Normal;
out vec4 FragPosLightSpace;

const int MAX_BONES = 100;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;
uniform mat4 bones[MAX_BONES];
uniform mat4 lightSpaceMatrix;

void main()
{
  mat4 boneTransform = bones[boneIds[0]] * weights[0];
  boneTransform += bones[boneIds[1]] * weights[1];
  boneTransform += bones[boneIds[2]] * weights[2];
  boneTransform += bones[boneIds[3]] * weights[3];


  FragPos = vec3(model * boneTransform * vec4(position, 1.0));
  Normal = -transpose(inverse(mat3(model * boneTransform))) * normal;  

  FragPosLightSpace = lightSpaceMatrix * vec4(FragPos, 1.0);

  fragTexCoords = texCoords;
  gl_Position = projection * view * model * boneTransform * vec4(position, 1.0);
}

