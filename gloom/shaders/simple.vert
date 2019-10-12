#version 430 core

in layout(location=0) vec3 position;
in layout(location=1) vec4 vertexColor;
in layout(location=2) vec3 normal;
uniform layout(location=3) mat4 model;
uniform layout(location=4) mat4 view;
uniform layout(location=5) mat4 projection;
uniform layout(location=6) vec3 viewPos;

out layout(location=1) vec4 fragmentColor;
out layout(location=2) vec3 fragmentNormal;
out layout(location=3) vec3 fragmentviewPos;
out layout(location=4) vec3 fragmentPos;

void main()
{
    gl_Position = projection * view * model * vec4(position, 1.0f);

    fragmentColor = vertexColor;
    fragmentNormal = normalize(mat3(model) * normal);
    fragmentviewPos = viewPos;
    fragmentPos = vec3(view * model * vec4(position, 1.0f));
}
