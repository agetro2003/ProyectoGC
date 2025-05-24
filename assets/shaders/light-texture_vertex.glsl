#version 330 core

layout (location = 0) in vec3 aPos;       // position
layout (location = 1) in vec4 aColor;     // color
layout (location = 2) in vec2 aTexCoord;  // texture coordinates
layout (location = 3) in vec3 aNormal;    // normal

uniform mat4 modelView;
uniform mat4 projection;

out vec3 FragPos;
out vec3 Normal;
out vec4 vertexColor;
out vec2 texCoord;

void main()
{
    gl_Position = projection * modelView * vec4(aPos, 1.0);
    FragPos = vec3(modelView * vec4(aPos, 1.0));
    Normal = (modelView * vec4(aNormal, 0.0)).xyz;
    vertexColor = aColor;
    texCoord = aTexCoord;
}
