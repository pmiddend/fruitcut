#version 140

$$$HEADER$$$

in vec2 texcoord_interp;
out vec4 frag_color;

void main()
{
	//frag_color = vec4(0.0,0.0,0.0,1.0);
	frag_color = texture2D(texture,texcoord_interp);
}
