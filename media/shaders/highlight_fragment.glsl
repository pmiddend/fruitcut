#version 140

$$$HEADER$$$

out vec4 frag_color;

void
main()
{
	vec2 normalized = 
		gl_FragCoord.xy/texture_size;

	vec3 t = 
		vec3(
			texture(
				tex,
				normalized));

	t = 
		vec3(
			2*t.r*t.r*t.r,
			2*t.g*t.g*t.g,
			2*t.b*t.b*t.b);
	/*
	t = 
		max(
			vec3(0.0,0.0,0.0),
			2.0 * t - 1.0);
	*/

	frag_color = 
		vec4(
			t,
			1.0);
}
