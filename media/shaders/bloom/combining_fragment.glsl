#version 140

$$$HEADER$$$

out vec4 frag_color;

void
main()
{
	vec2 normalized = 
		gl_FragCoord.xy/vec2(1024.0,768.0);

	normalized = vec2(normalized.x,1.0 - normalized.y);

	frag_color = 
		texture2D(
			original,
			normalized)
		+
		texture2D(
			blurred,
			normalized);
}
