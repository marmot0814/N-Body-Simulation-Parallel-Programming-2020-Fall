#version 450 core

in vec3 position_in;

uniform mat4 mvp;

void main()
{
	vec4 pos = mvp * vec4(position_in, 1.0f);
	gl_Position = pos;
	gl_PointSize = 50 * (1/pos.z);
}
