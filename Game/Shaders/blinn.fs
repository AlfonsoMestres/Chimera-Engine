#version 330 core

uniform vec4 newColor;

layout (std140) uniform Matrices
{
    mat4 proj;
    mat4 view;
};

out vec4 color;

uniform vec3  light_pos;
uniform float ambient;
uniform float shininess;
uniform float k_ambient;
uniform float k_diffuse;
uniform float k_specular;

in vec3 position;
in vec3 normal; 

void main()
{
	vec3 normal = normalize(normal);
    vec3 light_dir = normalize(light_pos-position);
    float diffuse = max(0.0, dot(normal, light_dir));
    float specular = 0.0;

    vec3 view_pos = transpose(mat3(view))*(-view[3].xyz);
    vec3 view_dir = normalize(view_pos-position);
	vec3 half_dir = normalize(light_dir + view_dir);
    float sp = max(dot(normal, half_dir), 0.0);

    specular = pow(sp, shininess); 

    
    float intensity = (k_ambient*ambient+k_diffuse*diffuse+k_specular*specular);

    color = vec4(intensity*newColor.x, intensity*newColor.y, intensity*newColor.z, 1.0);
}