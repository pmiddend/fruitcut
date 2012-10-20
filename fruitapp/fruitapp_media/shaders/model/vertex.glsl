#version 140

$$$HEADER$$$

out vec3 position_interp;
out vec3 light_position_interp;
out vec2 texcoord_interp;
out vec3 normal_interp;

void main()
{
	gl_Position = mvp * vec4(position,1.0);
	position_interp = vec3(mv * vec4(position,1.0));
	light_position_interp = vec3(world * vec4(light_position,1.0));
	texcoord_interp = texcoord;
	normal_interp = normalize(vec3(normal_matrix * vec4(normal,1.0)));
}
