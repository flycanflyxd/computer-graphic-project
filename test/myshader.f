uniform sampler2D texture;
uniform vec3 light; 
uniform vec4 l_ambient; 
uniform vec4 l_diffuse; 
uniform vec4 l_specular; 

varying vec2 texcoord; 
varying vec3 outNormal; 
varying vec3 position; 

void main() 
{ 
	vec4 texcolor = texture2D(texture, texcoord); 
	vec3 normal = normalize(outNormal);
	vec3 afterLight = normalize(light - position); 
	vec3 afterCamera = normalize(-position); 
	vec3 reflection = -reflect(afterCamera, normal); 
	vec3 ambient = vec3(l_ambient * texcolor); 
	float diffuseContribution = max(0.0, dot(normal, afterLight)); 
	vec3 diffuse = vec3(diffuseContribution * l_diffuse * texcolor); 
	float specularContribution = pow(max(0.0, dot(reflection, afterLight)), 32.0); 
	vec3 specular = vec3(specularContribution * l_specular); 
	gl_FragColor = vec4(ambient + diffuse + specular, 1.0); 
}