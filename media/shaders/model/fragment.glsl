#version 140

$$$HEADER$$$

in vec3 position_interp;
in vec2 texcoord_interp;
in vec3 normal_interp;
in vec3 light_position_interp;
out vec4 frag_color;

void main()
{
	vec3 
		light_to_point = 
			normalize(
				 light_position_interp - position_interp),
		to_eye = 
			normalize(
				-position_interp),
		reflected = 
			normalize(
				-reflect(
					light_to_point,
					normal_interp));

	// Diffuse
	const float diffuse = 0.50;

	// Both for specular
	const float shininess = 40.0;
	const float specular = 0.5;

	float total_component = 
		// Ambient
		ambient_intensity + 
		// Diffuse
		clamp(
			diffuse * 
			max(
				dot(
					normal_interp,
					light_to_point),
				0.0),
			0.0,
			1.0) +
		// Specular
		clamp(
			specular *
			pow(
				max(
					dot(
						reflected,
						to_eye),
					0.0),
				shininess),
			0.0,
			1.0);

	frag_color = 
		total_component *
		texture2D(
			texture,
			texcoord_interp);
}
