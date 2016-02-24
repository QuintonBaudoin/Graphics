#version 410 
					
layout(location=0) in vec4 Position; 
layout(location=1) in vec2 Texcoord;


							
//out vec4 vColour; 
out vec2 frag_texcoord;

uniform sampler2D perlin_texture;
uniform float scaleHeight = 10;
							
uniform mat4 ProjectionView;

uniform mat4 Model; 
							
void main() 
{ 
  vec4 pos = Position;
  pos.y += texture(perlin_texture, Texcoord).r * scaleHeight;

  frag_texcoord = Texcoord;
  //vColour = Colour; 
  gl_Position = ProjectionView * Model * pos; 
}