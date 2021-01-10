#version 450 core

out vec4 color_out;

uniform sampler2D tex;
uniform vec3 color;

void main()
{
	color_out = texture(tex, gl_PointCoord) * vec4(color, 1);
}
