#version 330 core

out vec4 FragColor;

float near = 0.1; 
float far  = 100.0;

void main() 
{
    float z = gl_FragCoord.z * 2 - 1;
    float linear_depth = (2.0 * near * far) / (far + near - z * (far - near)) / far;
    FragColor = vec4(vec3(linear_depth), 1.0);
}