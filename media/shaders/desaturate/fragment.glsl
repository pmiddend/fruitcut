#version 140

$$$HEADER$$$

out vec4 frag_color;

float	luminosity(vec3 v)
{
	// Calculate luminance according to the YUV coefficients
	return dot(v, vec3(0.212, 0.716, 0.072));	
}

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

	frag_color = 
		vec4(
			factor * t + (1.0-factor) * luminosity(t),
			1.0);
}
