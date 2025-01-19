#include "ObjectManager.h"

std::vector<Object> OBJMG::objects; 
std::vector<const char*> OBJMG::c_ObjectList;
std::vector<std::string> OBJMG::s_ObjectList;


void OBJMG::AddObject(float mass, Vector2f pos, Vector3f color){
    objects.push_back(Object(mass, pos, color));
    UpdateObjectList();
}

void OBJMG::DrawObjects(){
    for(Object item : objects){
        item.Draw();
    }
}

void OBJMG::DeleteObjects(){
    for(Object& item : objects){
        item.Delete();
    }
}

void OBJMG::Delete(int index){
    objects[index].Delete();
    objects.erase(objects.begin() + index);
    UpdateObjectList();
}

void OBJMG::UpdateObjects(){
    for(Object& item : objects){
        item.Update();
    }
}

void OBJMG::UpdateObjectList(){
    s_ObjectList.clear();
    c_ObjectList.clear();

    s_ObjectList.reserve(objects.size()); 
    c_ObjectList.reserve(objects.size());

    for(int i = 0; i < objects.size(); i++){
        s_ObjectList.push_back(std::to_string(objects[i].GetID()));
        c_ObjectList.push_back(s_ObjectList.back().c_str());
    }
}
