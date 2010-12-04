#version 140

uniform sampler2D original,tone_mapped;

out vec4 frag_color;

void
main()
{
	frag_color = 
		texture2D(
			original,
			gl_FragCoord.xy / vec2(1024.0,768.0))+
		texture2D(
			tone_mapped,
			gl_FragCoord.xy / vec2(1024.0,768.0));
}
