#version 140

$$$HEADER$$$

out vec4 frag_color;

void
main()
{
	frag_color = 
		texture2D(
			original,
			gl_FragCoord.xy / vec2(1024.0,768.0))
		+
		texture2D(
			blurred,
			gl_FragCoord.xy / vec2(1024.0,768.0));
}
