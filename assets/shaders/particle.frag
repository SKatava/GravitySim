#version 330 core

in vec3 fragColor;

out vec4 FragColor;

void main(){
    float dist = length(gl_PointCoord - vec2(0.5));

    if(dist > 0.5){
        discard;
    }

    FragColor = vec4(fragColor, 1.0f);
}
