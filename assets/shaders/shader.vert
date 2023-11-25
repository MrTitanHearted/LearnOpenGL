#version 460 core
layout(location = 0) in vec3 aPos;
layout(location = 1) in vec3 aNormal;
layout(location = 2) in vec2 aTexCoords;
layout(location = 3) in ivec4 aBoneIds;
layout(location = 4) in vec4 aBoneWeights;

out vec3 Normal;
out vec2 TexCoords;
flat out ivec4 BoneIds;
out vec4 BoneWeights;

const int MAX_BONES = 200;
uniform mat4 bones[MAX_BONES];

uniform mat4 view;
uniform mat4 proj;
uniform mat4 model;

void main() {
    mat4 boneTransform = mat4(0.0);
    for(int i = 0; i < 4; i++) boneTransform += aBoneWeights[i] * bones[aBoneIds[i]];

    // gl_Position = proj * view * model * bones[49] * vec4(aPos, 1.0);
    gl_Position = proj * view * model * bones[0] * vec4(aPos, 1.0);
    Normal = aNormal;
    TexCoords = aTexCoords;
    BoneIds = aBoneIds;
    BoneWeights = aBoneWeights;
}