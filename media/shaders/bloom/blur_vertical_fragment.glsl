#version 140

uniform sampler2D texture;

out vec4 frag_color;

void
main()
{
	vec3 t = 
		vec3(
			texture2D(
				texture,
				gl_FragCoord.xy/vec2(256.0,256.0)));

	frag_color = 
		vec4(
			t,
			1.0);
}
