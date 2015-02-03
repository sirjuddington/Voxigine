
#version 330

smooth in vec4 vertex_colour;

out vec4 outputColor;

void main()
{
   outputColor = vertex_colour;
}
