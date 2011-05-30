#version 140

$$$HEADER$$$

out vec2 texcoord_interp;

void
main()
{
	gl_Position = mvp * vec4(position.xy,0.0,1.0);
	texcoord_interp = texcoord;
}
