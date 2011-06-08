#version 140

$$$HEADER$$$

in vec2 screen_pos;
out vec4 frag_color;

void
main()
{
	frag_color = 
		texture(
			tex,
			(screen_pos + vec2(1.0,1.0))/2.0);
}
