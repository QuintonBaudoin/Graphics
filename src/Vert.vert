#version 410 
layout(location=0) in vec4 Position; 
layout(location=1) in vec4 Color;
layout(location=2) in vec4 Texcoord;

out vec4 vColor; 
out vec2 frag_texcoord;

uniform mat4 ProjectionView; 
uniform float time;

uniform float heightScale; 
void main()
{ 
	frag_texcoord = texcoord;
	vColor = Color; 
	gl_Position = ProjectionView * Position;
 } 