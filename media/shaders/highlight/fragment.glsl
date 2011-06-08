#version 140

$$$HEADER$$$

out vec4 frag_color;

float	luminance(vec3 v)
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

	float gray = 
		luminance(
			t);

	//gray = clamp((gray - threshold)/(1.0-threshold),0.0,1.0);
	gray = clamp(gray*gray*gray - 0.5,0.0,1.0);

	frag_color = 
		vec4(
			gray,gray,gray,
			1.0);
}
