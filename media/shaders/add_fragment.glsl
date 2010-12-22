#version 140

$$$HEADER$$$

out vec4 frag_color;

void
main()
{
	vec2 normalized = 
		gl_FragCoord.xy / target_size;

	// Final color
	frag_color = 
		texture(tex1,normalized) + 
		texture(tex2,normalized);
}
