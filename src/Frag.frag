#version 410 
in vec4 vColor; 
in vec2 frag_texcoord;
out vec4 FragColor; 
uniform sampler2D texture;
void main() {FragColor = texture(texture,frag_texcorrd);} 