#version 430 core

in layout(location=0) vec3 position;
in layout(location=1) vec4 vertexColor;

out layout(location=1) vec4 fragmentColor;

void main()
{
    gl_Position = vec4(position, 1.0f);
    fragmentColor = vertexColor;
}
