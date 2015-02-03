
#version 330
layout(location = 0) in vec4 position;
layout(location = 1) in vec4 colour;

smooth out vec4 vertex_colour;

uniform mat4 mat_perspective;

void main()
{
   gl_Position = mat_perspective * position;
   vertex_colour = colour;
}
