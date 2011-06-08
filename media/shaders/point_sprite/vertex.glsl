in float dim;

$$$HEADER$$$

void main()
{
	gl_Position = mvp * gl_Vertex;
	gl_PointSize = dim;
	gl_FrontColor = gl_Color;
}
