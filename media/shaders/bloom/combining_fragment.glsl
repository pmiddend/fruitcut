#version 140

$$$HEADER$$$

out vec4 frag_color;

float	lumRGB(vec3 v)
{
	return dot(v, vec3(0.212, 0.716, 0.072));	
}


void
main()
{
	vec2 inverse_buffer_size = 1.0/texture_size;

	vec2 normalized = 
		gl_FragCoord.xy * inverse_buffer_size;

	normalized = vec2(normalized.x,1.0 - normalized.y);

	const float w = 1.75;
 
	float 
		t = 
			lumRGB(
				texture2D(
					original, 
					normalized + 
					vec2(0.0, -1.0) * w * inverse_buffer_size).xyz),
		l = 
			lumRGB(
				texture2D(
					original, 
					normalized 
						+ vec2(-1.0, 0.0) * w * inverse_buffer_size).xyz),
		r = 
			lumRGB(
				texture2D(
					original, 
					normalized 
						+ vec2(1.0, 0.0) * w * inverse_buffer_size).xyz),
	    b = 
				lumRGB(
				texture2D(
					original, 
					normalized 
						+ vec2(0.0, 1.0) * w * inverse_buffer_size).xyz);
 
	vec2 n = 
		vec2(-(t - b), r - l);
	float 
		nl = 
			length(n);
 
	if (nl < (1.0 / 16.0))
		frag_color = texture2D(original, normalized);
	else
	{
		n *= inverse_buffer_size / nl;

		vec4	o = texture2D(original, normalized),
			t0 = texture2D(original, normalized + n * 0.5) * 0.9,
			t1 = texture2D(original, normalized - n * 0.5) * 0.9,
			t2 = texture2D(original, normalized + n) * 0.75,
			t3 = texture2D(original, normalized - n) * 0.75;

		//frag_color = (o + t0 + t1 + t2 + t3) / 4.3;
		frag_color = (o + t0 + t1 + t2 + t3) / 4.3;
	}


	frag_color += 
		texture2D(
			blurred,
			normalized);

/*

	frag_color = 
		texture2D(
			original,
			normalized)
		+
		texture2D(
			blurred,
			normalized);
*/
}
