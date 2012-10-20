#version 140

$$$HEADER$$$

out vec4 frag_color;

float	lumRGB(vec3 v)
{
	// Calculate luminance according to the YUV coefficients
	return dot(v, vec3(0.212, 0.716, 0.072));	
}


void
main()
{
	const float scale = 0.5;

	vec2 inverse_buffer_size = 1.0/texture_size;

	vec2 normalized = 
		gl_FragCoord.xy * inverse_buffer_size;

	const float w = 0.8;
 
	float 
		t = 
			lumRGB(
				texture(
					tex, 
					normalized + 
					vec2(0.0, -1.0) * w * inverse_buffer_size).xyz),
		l = 
			lumRGB(
				texture(
					tex, 
					normalized 
						+ vec2(-1.0, 0.0) * w * inverse_buffer_size).xyz),
		r = 
			lumRGB(
				texture(
					tex, 
					normalized 
						+ vec2(1.0, 0.0) * w * inverse_buffer_size).xyz),
		b = 
			lumRGB(
			texture(
				tex, 
				normalized 
					+ vec2(0.0, 1.0) * w * inverse_buffer_size).xyz),
		tl = 
			lumRGB(
			texture(
				tex, 
				normalized 
					+ vec2(-1.0, -1.0) * w * inverse_buffer_size).xyz),
		bl = 
			lumRGB(
			texture(
				tex, 
				normalized 
					+ vec2(-1.0, 1.0) * w * inverse_buffer_size).xyz),
		br = 
			lumRGB(
			texture(
				tex, 
				normalized 
					+ vec2(1.0, 1.0) * w * inverse_buffer_size).xyz),
		tr = 
			lumRGB(
			texture(
				tex, 
				normalized 
					+ vec2(1.0, -1.0) * w * inverse_buffer_size).xyz);

	float sum0 = tr + t + br;
	float sum1 = tl + b + bl;
	float sum2 = tl + l + tr;
	float sum3 = bl + r + br;

	float first = (sum1 - sum0);
	float second = (sum2 - sum3);

	vec2 Normal = 
		vec2( 
			first, 
			second) * inverse_buffer_size * scale;

	vec4 Scene0 = texture(tex,normalized.xy );
	vec4 Scene1 = texture(tex,normalized.xy + Normal.xy );
	vec4 Scene2 = texture(tex,normalized.xy - Normal.xy );
	vec4 Scene3 = texture(tex,normalized.xy + vec2(Normal.x, -Normal.y) * 0.5 );
	vec4 Scene4 = texture(tex,normalized.xy - vec2(Normal.x, -Normal.y) * 0.5 );

	// Final color
	frag_color = (Scene0 + Scene1 + Scene2 + Scene3 + Scene4) * 0.2;
}
