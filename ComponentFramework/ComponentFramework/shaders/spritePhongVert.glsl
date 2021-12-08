#version 460
layout (location = 0) in vec4 vVertex;

out vec2 TexCoords;

uniform mat4 projectionMatrix;
uniform mat4 modelMatrix;

void main() 
{
	TexCoords = vVertex.zw;
	gl_Position =  projectionMatrix * modelMatrix * vec4(vVertex.xy, 0.0, 1.0); 
}
