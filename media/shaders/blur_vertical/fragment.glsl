#version 140

$$$HEADER$$$

out vec4 frag_color;

float distributions[5] = float[](0.0280412,0.233924,0.474425,0.233924,0.0280412);

void
main()
{
	vec3 sum = vec3(0.0,0.0,0.0);
	float coord = gl_FragCoord.y - 2.0;
	for (int i = 0; i < 5; ++i)
	{
		sum += 
			distributions[i] * vec3(
				texture(
					tex,
					vec2(
						gl_FragCoord.x,
						clamp(coord,1.0,texture_size.y-1.0))/texture_size));
		coord += 1.0;
	}

	frag_color = 
		vec4(
			sum,
			1.0);
}
