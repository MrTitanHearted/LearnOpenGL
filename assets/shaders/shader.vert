// #version 460 core

// layout(location = 0) in vec3 aPos;
// layout(location = 1) in vec3 aNormal;
// layout(location = 2) in vec2 aTexCoords;

// uniform mat4 proj;
// uniform mat4 view;
// uniform mat4 model;

// out vec2 TexCoords;

// void main() {
//     gl_Position = proj * view * model * vec4(aPos, 1.0);
//     TexCoords = aTexCoords;
// }

#version 460 core

layout(location = 0) in vec3 aPos;
layout(location = 1) in vec3 aNormal;
layout(location = 2) in vec2 aTexCoords;
layout(location = 3) in ivec4 aBoneIds;
layout(location = 4) in vec4 aBoneWeights;

uniform mat4 proj;
uniform mat4 view;
uniform mat4 model;

const int MAX_BONES = 100;
const int MAX_BONE_INFLUENCE = 4;
uniform mat4 bones[MAX_BONES];

out vec2 TexCoords;

void main() {
    vec4 totalPosition = vec4(0.0);
    for(int i = 0; i < MAX_BONE_INFLUENCE; i++) {
        if(aBoneIds[i] == -1)
            continue;
        if(aBoneIds[i] >= MAX_BONES) {
            totalPosition = vec4(aPos, 1.0);
            break;
        }
        vec4 localPosition = bones[aBoneIds[i]] * vec4(aPos, 1.0);
        totalPosition += localPosition * aBoneWeights[i];
    }

    mat4 viewModel = view * model;
    gl_Position = proj * viewModel * totalPosition;
    TexCoords = aTexCoords;
}
