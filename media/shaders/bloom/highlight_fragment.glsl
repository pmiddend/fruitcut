#version 140

$$$HEADER$$$

out vec4 frag_color;

void
main()
{
	vec2 normalized = 
		gl_FragCoord.xy/vec2(256.0,256.0);

	vec3 t = 
		vec3(
			texture2D(
				texture,
//				vec2(0.0,1.0) - normalized));
				normalized));

	t = 
		max(
			vec3(0.0,0.0,0.0),
			2.0 * t - 1.0);

	frag_color = 
		vec4(
			t,
			1.0);
}
