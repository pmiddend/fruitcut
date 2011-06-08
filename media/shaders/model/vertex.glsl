#version 140

$$$HEADER$$$

out vec2 texcoord_interp;

void main()
{
	gl_Position = mvp * vec4(position,1.0);
	texcoord_interp = texcoord;
}
