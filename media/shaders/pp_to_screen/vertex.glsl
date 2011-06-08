#version 140

$$$HEADER$$$

out vec2 screen_pos;

void
main()
{
	screen_pos = 
		position.xy;
	gl_Position = 
		vec4(
			position.xy,
			0.0,
			1.0);
}
