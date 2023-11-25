#version 460 core
out vec4 FragColor;

in vec3 Normal;
in vec2 TexCoords;
flat in ivec4 BoneIds;
in vec4 BoneWeights;

uniform sampler2D textureDiffuse0;
uniform sampler2D textureSpecular0;

uniform int displayBoneIndex;

void main() {
    bool found = false;
    for(int i = 0; i < 4; i++) {
        if(BoneIds[i] == -1)
            continue;
        if(BoneIds[i] == displayBoneIndex) {
            if(BoneWeights[i] >= 0.7)
                FragColor = vec4(1.0, 0.0, 0.0, 1.0) * BoneWeights[i];
            else if(BoneWeights[i] >= 0.4 && BoneWeights[i] < 0.7)
                FragColor = vec4(0.0, 1.0, 0.0, 1.0) * BoneWeights[i];
            else if(BoneWeights[i] >= 0.1)
                FragColor = vec4(1.0, 1.0, 0.0, 1.0) * BoneWeights[i];
            found = true;
            break;
        }
    }
    if(!found)
        FragColor = vec4(vec3(0.0), 1.0);
    // FragColor = texture(textureDiffuse0, TexCoords);
}