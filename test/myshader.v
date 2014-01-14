varying vec2 texcoord;
varying vec3 outNormal; 
varying vec3 position; 

void main()
{
	gl_Position = gl_ModelViewProjectionMatrix * gl_Vertex;
	position = vec3(gl_ModelViewMatrix * gl_Vertex);
	outNormal = gl_NormalMatrix * gl_Normal;
	texcoord = gl_MultiTexCoord0.st;
} 
