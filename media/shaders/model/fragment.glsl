#version 140

$$$HEADER$$$

in vec3 position_interp;
in vec2 texcoord_interp;
in vec3 normal_interp;
in vec3 light_position_interp;
out vec4 frag_color;

void main()
{
	vec3 light_to_point = 
		normalize(
			 light_position_interp - position_interp);

	float intensity = 
		clamp(
			dot(normal_interp,light_to_point),
			ambient_intensity,
			1.0);

	frag_color = 
		intensity * 
		texture2D(
			texture,
			texcoord_interp);
}
