#include "GravityEngine.h"

void GEN::Step(){
    CSL::Check();
    for(int i = 0; i < OBJMG::objects.size(); i++){
        for(int j = i+1; j < OBJMG::objects.size(); j++){
            float distanceX = OBJMG::objects[j].GetPos().x - OBJMG::objects[i].GetPos().x;
            float distanceY = OBJMG::objects[j].GetPos().y - OBJMG::objects[i].GetPos().y; 
                
            float distance = std::sqrt(distanceX*distanceX + distanceY*distanceY);

            float force = G * OBJMG::objects[i].GetMass() * OBJMG::objects[j].GetMass() / (distance * distance);
                
            float forceX = force * (distanceX / distance);
            float forceY = force * (distanceY / distance);

            if(distance == 0){
                forceX = 0.f;
                forceY = 0.f;
            }

            OBJMG::objects[i].AddForce(Vector2f(forceX, forceY));
            OBJMG::objects[j].AddForce(Vector2f(-forceX, -forceY));
            
        }
    }
}