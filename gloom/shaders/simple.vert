#version 430 core

in layout(location=0) vec3 position;
in layout(location=1) vec4 vertexColor;

uniform layout(location=2) mat4 trans_mat;

out layout(location=1) vec4 fragmentColor;

void main()
{
    vec4 model_pos = vec4(position, 1.0f);
    mat4 model_mat = mat4(trans_mat);
    vec4 world_pos = model_mat*model_pos;
    gl_Position = world_pos;

    fragmentColor = vertexColor;
}
