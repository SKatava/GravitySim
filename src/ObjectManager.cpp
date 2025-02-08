#include "ObjectManager.h"

std::vector<Object> OBJMG::objects; 

//Add object to the manager
void OBJMG::AddObject(float mass, Vector2f pos, Vector3f color){
    objects.push_back(Object(mass, pos, color));
}

//Draw every object
void OBJMG::DrawObjects(){
    for(Object item : objects){
        item.Draw();
    }
}

//Delete every object
void OBJMG::DeleteObjects(){
    for(Object& item : objects){
        item.Delete();
    }
}

//Delete object at the specific index
void OBJMG::Delete(int index){
    objects[index].Delete();
    objects.erase(objects.begin() + index);
}

//Update VBO(positions) of every object
void OBJMG::UpdateObjects(){
    for(Object& item : objects){
        item.Update();
    }
}


