#version 460
in  vec3 vertNormal;
in  vec3 lightDir;
in  vec3 eyeDir;
out vec4 fragColor;

void main() { 
	vec4 ks = vec4(0.6, 0.6, 0.6, 0.0);
	vec4 kd = vec4(0.6, 0.6, 0.6, 0.0); 
	vec4 ka =  vec4(0.1, 0.1, 0.1, 0.0); 
	
	float diff = max(dot(vertNormal, lightDir), 0.0);

	/// Reflection is based on the incedent light source which means it is 
	/// a vector FROM the light source not the direction TO the light source
	vec3 reflection = normalize(reflect(-lightDir, vertNormal));
	float spec = max(dot(eyeDir, reflection), 0.0);
	if(diff > 0.0){
		spec = pow(spec,14.0);
	}
	
	fragColor =  ka + (diff * kd) + (spec * ks );
} 

