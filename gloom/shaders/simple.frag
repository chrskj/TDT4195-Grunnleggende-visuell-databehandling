#version 430 core

in layout(location=1) vec4 fragmentColor;
in layout(location=2) vec3 fragmentNormal;
in layout(location=3) vec3 fragmentviewPos;
in layout(location=4) vec3 fragmentPos;

out vec4 color;

void main()
{
    vec3 lightColor = vec3(1.0f, 1.0f, 1.0f);
    float ambientStrength = 0.1f;
    vec3 ambient = ambientStrength * lightColor;

    vec3 norm = normalize(fragmentNormal);
    vec3 lightDir = normalize(vec3(-0.8f, 0.5f, -0.6f));
    float diff = max(dot(norm, lightDir), 0.0f);
    vec3 diffuse = diff * lightColor;


    float specularStrength = 0.5f;
    vec3 viewDir = normalize(fragmentviewPos - fragmentPos);
    vec3 reflectDir = reflect(-lightDir, norm);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0f), 32);
    vec3 specular = specularStrength * spec * lightColor;  
    
    vec4 result = vec4(ambient + diffuse + specular, 1.0f) * fragmentColor;
    //vec4 result = vec4(diffuse, 1.0f) * fragmentColor;
    color = result;
    //color = vec4(fragmentNormal, 1.0f);
}
