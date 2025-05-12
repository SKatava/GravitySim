#version 430  

struct EntityData {
    float posX;
    float posY;
    float velX;
    float velY;
    float forceX;
    float forceY;
    float mass;
    float padding; 
};

layout(std430, binding = 0) buffer Bodies {
    EntityData entities[];
};

out vec3 fragColor;

void main(){
    uint i = gl_VertexID;
    float x = (entities[i].posX / 400) - 1.f; 
    float y = 1.f - (entities[i].posY / 400); 
    gl_Position = vec4(x, y, 0, 1.0);
    //gl_Position = vec4(0, 0, 0, 1.0);
    fragColor = vec3(1.0, 1.0, 1.0);
    gl_PointSize = 1.f;
}
