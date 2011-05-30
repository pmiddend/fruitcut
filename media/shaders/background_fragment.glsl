#version 140

$$$HEADER$$$

in vec2 texcoord_interp;
out vec4 frag_color;

void
main()
{
	frag_color = 
		texture(
			tex,
			texcoord_interp);
}
