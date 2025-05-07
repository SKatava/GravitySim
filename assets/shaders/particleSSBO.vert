#version 430  

struct EntityData {
    double posX;
    double posY;
    double velX;
    double velY;
    double forceX;
    double forceY;
    double mass;
    double padding; 
};

layout(std430, binding = 0) buffer Bodies {
    EntityData entities[];
};

out vec3 fragColor;

void main(){
    uint i = gl_VertexID;
    double x = (entities[i].posX / 400) - 1.f; 
    double y = 1.f - (entities[i].posY / 400); 
    gl_Position = vec4(x, y, 0, 1.0);
    //gl_Position = vec4(0, 0, 0, 1.0);
    fragColor = vec3(1.0, 1.0, 1.0);
    gl_PointSize = 1.f;
}
