#version 330

layout (location = 0) in vec3 position;
layout (location = 1) in vec3 color;

out vec3 fragColor;

void main(){
    gl_Position = vec4(position, 1.f);
    fragColor = color;
    gl_PointSize = 1.f;
}
