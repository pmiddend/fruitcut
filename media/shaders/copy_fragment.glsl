#version 140

$$$HEADER$$$

out vec4 frag_color;

void
main()
{
	vec2 normalized = 
		gl_FragCoord.xy/texture_size;

	frag_color = 
		texture2D(
			texture,
			normalized);
}
