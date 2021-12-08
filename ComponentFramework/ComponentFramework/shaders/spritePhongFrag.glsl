#version 460
in  vec2 TexCoords;
out vec4 fragColor;

uniform sampler2D myTexture;
uniform vec3 spriteColor;

void main() 
{ 
	fragColor =  vec4(spriteColor, 1.0) * texture(image, TexCoords);
}

