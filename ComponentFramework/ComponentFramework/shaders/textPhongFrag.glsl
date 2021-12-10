#version 450
in  vec3 vertNormal;
in  vec3 lightDir;
in  vec3 eyeDir;
in  vec2 uvCoords;
out vec4 fragColor;

uniform sampler2D myTexture;

void main() { 
	
	float diff = max(dot(vertNormal, lightDir), 0.0);
	vec4 specLightColor = vec4(0.2, 0.2, 0.2, 0.0);

	vec4 textureColor = texture(myTexture, uvCoords);
	vec4 ambientColor = 0.1 * textureColor;

	/// Reflection is based on the incedent light source which means it is 
	/// a vector FROM the light source not the direction TO the light source

	vec3 reflection = normalize(reflect(-lightDir, vertNormal));
	float spec = max(dot(eyeDir, reflection), 0.0);
	if(diff > 0.0){
		spec = pow(spec,14.0);
	} 
	
	fragColor =  ambientColor + (diff * textureColor) + (spec * specLightColor);
	
} 

