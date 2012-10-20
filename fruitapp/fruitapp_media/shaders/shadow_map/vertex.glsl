#version 140

$$$HEADER$$$

out vec4 position_interp;

void 
main()
{
	position_interp = mvp * vec4(position,1.0);
	gl_PointSize = float(texcoord);
	// TODO: Can we just say gl_Position = position_interp here?
	gl_Position = mvp * vec4(position,1.0);
}
