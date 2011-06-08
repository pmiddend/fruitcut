#version 140

$$$HEADER$$$

in vec2 texcoord_interp;
in vec4 shadow_coord_interp;
out vec4 frag_color;

float 
chebyshevUpperBound(
	vec4 coord)
{
	// We retrive the two moments previously stored (depth and
	// depth*depth)
	vec2 moments = texture2D(shadow_map,coord.xy).rg;

	if (moments.x == 1.0)
		return 0.0;

	// Surface is fully lit. as the current fragment is before the light
	// occluder
	if (coord.z <= moments.x)
		return 1.0;

	// The fragment is either in shadow or penumbra. We now use
	// chebyshev's upperBound to check How likely this pixel is to be
	// lit (p_max)
	float variance = moments.y - (moments.x*moments.x);
	variance = max(variance,0.00002);
	//variance = max(variance,0.002);

	float d = coord.z - moments.x;
	float p_max = variance / (variance + d*d);

	return p_max;
}

void
main()
{
	vec4 real_shadow_coord = 
		shadow_coord_interp / shadow_coord_interp.w * 0.5 + 0.5;

	float shadow = 
		chebyshevUpperBound(
			real_shadow_coord);

	frag_color = 
		vec4(	
			max(0.5,(1.0 - shadow)) * 
			texture(
				tex,
				texcoord_interp).xyz,
			1.0);
}
