#include "CollisionSolution.h"

//Check for collision between every objet
void CSL::Check(){
    int size = OBJMG::objects.size();
    for(int i = 0; i < size; i++){
        for(int j = 0; j < size; j++){
            if(i != j){
                float distanceX = OBJMG::objects[i].GetPos().x - OBJMG::objects[j].GetPos().x;
                float distanceY = OBJMG::objects[i].GetPos().y - OBJMG::objects[j].GetPos().y; 
                if(std::abs(distanceX) < 25 && std::abs(distanceY) < 25){
                    float massA = OBJMG::objects[i].GetMass();
                    float massB = OBJMG::objects[j].GetMass();
                    Vector2f velA = OBJMG::objects[i].GetVelocity();
                    Vector2f velB = OBJMG::objects[j].GetVelocity();
                    if(OBJMG::objects[i].GetMass() > OBJMG::objects[j].GetMass()){
                        Solution(massA, massB, velA, velB, OBJMG::objects[i].GetPos());

                    }
                    else if(OBJMG::objects[i].GetMass() < OBJMG::objects[j].GetMass()){
                        Solution(massA, massB, velA, velB, OBJMG::objects[j].GetPos());
                    }
                    else{
                        Vector2f pos = Vector2f( (OBJMG::objects[i].GetPos().x + OBJMG::objects[j].GetPos().x) / 2, (OBJMG::objects[i].GetPos().y + OBJMG::objects[j].GetPos().y) / 2);
                        Solution(massA, massB, velA, velB, pos);
                    }
                    OBJMG::Delete(i);
                    OBJMG::Delete(j-1);
                    size--;
                }
            }
        }
    }
    
}

//Solution for the emerging collision between two objects
void CSL::Solution(float massA, float massB, Vector2f velA, Vector2f velB, Vector2f pos){
    float mass = massA + massB;
    float velX = (massA * velA.x + massB * velB.x) / mass;
    float velY = (massA * velA.y + massB * velB.y) / mass;

    OBJMG::objects.push_back(Object(mass, Vector2f(pos.x, pos.y), Vector3f(0.f, 1.f, 0.f)));
    OBJMG::objects.back().SetInitialVelocity(Vector2f(velX, velY));
    
}