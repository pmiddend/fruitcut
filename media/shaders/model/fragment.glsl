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
	//const float diffuse = 0.50;

	// Both for specular
	//const float shininess = 40.0;
	//const float specular = 0.5;

	const vec4 ambient_color = vec4(1.0,1.0,1.0,1.0);

	vec4 total_component = 
		ambient_intensity * ambient_color + 
		clamp(
			diffuse_coefficient * 
			max(
				dot(
					normal_interp,
					light_to_point),
				0.0),
			0.0,
			1.0) * diffuse_color +
		// Specular
		clamp(
			specular_coefficient *
			pow(
				max(
					dot(
						reflected,
						to_eye),
					0.0),
				specular_shininess),
			0.0,
			1.0) * specular_color;

	frag_color = 
		total_component *
		texture2D(
			texture,
			texcoord_interp);
}
