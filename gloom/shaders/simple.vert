#version 430 core

in layout(location=0) vec3 position;
in layout(location=1) vec4 vertexColor;
uniform layout(location=2) mat4 model_mat;
uniform layout(location=3) mat4 projection_mat;

out layout(location=1) vec4 fragmentColor;

void main()
{
    vec4 object_pos = vec4(position, 1.0f);
    
	mat4 model_mat = mat4(model_mat);
    vec4 world_pos = model_mat*object_pos;

	mat4 projection_mat = mat4(projection_mat);
    gl_Position = projection_mat*world_pos;

    fragmentColor = vertexColor;
}
