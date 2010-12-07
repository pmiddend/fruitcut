#version 140

$$$HEADER$$$

out vec4 frag_color;

float
average(
	vec4 v)
{
	return 
}

void
main()
{
	//vec2 normalized = 
	//	gl_FragCoord.xy/texture_size;

	mat3 sobel_x = 
		mat3(
			-1.0,0.0,1.0,
			-2.0,0.0,2.0,
			-1.0,0.0,1.0);

	mat3 sobel_y = 
		mat3(
			-1.0,-2.0,-1.0,
			0.0,0.0,0.0,
			1.0,2.0,1.0);

	float x = 0.0, y = 0.0;

	for (int i = 0; i < 3; ++i)
	{
		for (int j = 0; j < 3; ++j)
		{
			vec2 modified_coord = 
				gl_FragCoord.xy + 
				vec2(
					float(i) - 1.0,
					float(j) - 1.0);

			float pixel_average = 
				average(
					texture2D(
						screen,
						modified_coord / texture_size));

			x += 
				sobel_x[i][j] * 
				pixel_average;

			y += 
				sobel_y[i][j] * 
				pixel_average;
		}
	}

	frag_color = 
		vec4(
			x,
			y,
			0.0,
			0.0);
}
