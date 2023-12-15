#version 460 core

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec3 aColour;
layout (location = 3) in vec2 aTex;

out vec3 crntPos;
out vec3 Normal;
out vec3 colour;
out vec2 texCoord;

uniform mat4 camMatrix;

uniform mat4 model;
uniform mat4 translation;
uniform mat4 rotation;
uniform mat4 scale;
uniform vec3 camPos;

void main()
{
	crntPos = vec3(model * translation * -rotation * scale * vec4(aPos - vec3(camPos.x / 50, camPos.y / 50, camPos.z / 50), 1.0f));
	Normal = aNormal;
	colour = aColour;
	texCoord = mat2(0.0, -1.0, 1.0, 0.0) * aTex;
	gl_Position = camMatrix * vec4(crntPos, 1.0);
}