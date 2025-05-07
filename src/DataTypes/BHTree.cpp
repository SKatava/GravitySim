#include "BHTree.h"
#include "Entities.h"
#include <iostream>
#include <algorithm>
#include <cmath>
#include <string>
#include <cassert>

#define G 6.6743e-11

Node::Node(double new_centerX, double new_centerY, double new_size) : centerX(new_centerX), centerY(new_centerY), size(new_size), cmX(0), cmY(0), totalMass(0), entityIndex(-1){
    std::fill(std::begin(children), std::end(children), -1); 
}

BHTree::BHTree(){
    rootIndex = CreateNode(0, 0, 0);
    nodes.reserve(Entities::entities.size() * 4);
    RootNodeSetup();
}

void BHTree::RootNodeSetup(){
    if(Entities::entities.empty()){
        return;
    }

    double minX = std::numeric_limits<double>::max();
    double maxX = std::numeric_limits<double>::lowest();
    double minY = std::numeric_limits<double>::max();
    double maxY = std::numeric_limits<double>::lowest();

    for (Entity& e : Entities::entities) {
        Vector2d pos = e.GetPosition();
        minX = std::min(minX, pos.x);
        maxX = std::max(maxX, pos.x);
        minY = std::min(minY, pos.y);
        maxY = std::max(maxY, pos.y);
    }

    double width = maxX - minX;
    double height = maxY - minY;
    nodes[rootIndex].size = std::max(width, height); 
    nodes[rootIndex].centerX = (minX + maxX) / 2; 
    nodes[rootIndex].centerY = (minY + maxY) / 2; 
}

int BHTree::CreateNode(double centerX, double centerY, double size){
    nodes.emplace_back(centerX, centerY, size);
    return nodes.size()-1;
}


int BHTree::GetQuadrant(Node node, Vector2d pos){
    if(pos.x > node.centerX && pos.y < node.centerY) return 0;
    else if(pos.x < node.centerX && pos.y < node.centerY) return 1;
    else if(pos.x < node.centerX && pos.y > node.centerY) return 2;
    else return 3;
    
}

Vector2d BHTree::GetQuadCenter(Node node, int quad){
    Vector2d center(0,0);
    double offset = node.size / 4;
    if(quad == 0){
        center.x = node.centerX + offset;
        center.y = node.centerY - offset;
    }
    else if(quad == 1){
        center.x = node.centerX - offset;
        center.y = node.centerY - offset;
    }
    else if(quad == 2){
        center.x = node.centerX - offset;
        center.y = node.centerY + offset;
    }
    else{
        center.x = node.centerX + offset;
        center.y = node.centerY + offset;
    }
    return center;
}


void BHTree::UpdateMassAndCoM(int nodeIndex){
    if(nodeIndex == -1) return;

    Node& node = nodes[nodeIndex];
    if(node.entityIndex != -1){
        node.totalMass = Entities::entities[node.entityIndex].GetMass();
        node.cmX = Entities::entities[node.entityIndex].GetPosition().x;
        node.cmY = Entities::entities[node.entityIndex].GetPosition().y;
        return;
    }

    double totalMass = 0;
    double cmXsum = 0;
    double cmYsum = 0;

    for(int i = 0; i < 4; i++){
        int childIndex = node.children[i];
        if(childIndex != -1){
            totalMass += nodes[childIndex].totalMass;
            cmXsum += nodes[childIndex].cmX * nodes[childIndex].totalMass;
            cmYsum += nodes[childIndex].cmY * nodes[childIndex].totalMass;
        }
    }

    node.totalMass = totalMass;
    if(node.totalMass > 0){
        node.cmX = cmXsum / totalMass;
        node.cmY = cmYsum / totalMass;
    } 
    else{
        node.cmX = node.centerX;
        node.cmY = node.centerY;
    }
    
}

void BHTree::InsertEntity(int nodeIndex, int entityIndex){
    assert(nodeIndex >= 0 && nodeIndex < nodes.size()); 
    Node& node = nodes[nodeIndex];
    Entity& entity = Entities::entities[entityIndex];

    Vector2d position = entity.GetPosition();
    if(node.entityIndex == -1 && node.totalMass == 0){
        node.entityIndex = entityIndex;
        node.cmX = entity.GetPosition().x;
        node.cmY = entity.GetPosition().y;
        node.totalMass = entity.GetMass();
        return;
    }
    else if(node.entityIndex == -1){
        int quad = GetQuadrant(node, position);
        if(node.children[quad] == -1){
            node.children[quad] = CreateNode(GetQuadCenter(node,quad).x, GetQuadCenter(node,quad).y, node.size/2);
        }
        InsertEntity(node.children[quad], entityIndex);
    }
    else{
        int oldEntityIndex = node.entityIndex;
        node.entityIndex = -1;
        
        InsertEntity(nodeIndex,  oldEntityIndex);
        InsertEntity(nodeIndex,  entityIndex);
    }
    UpdateMassAndCoM(nodeIndex);
}


void BHTree::PrintTree(int nodeIndex, int depth){
    if (nodeIndex == -1) return;

    Node& node = nodes[nodeIndex];

    std::string indent(depth * 2, ' '); 

    std::cout << indent << "Node " << nodeIndex
        << " | Center: (" << node.centerX << ", " << node.centerY << ")"
        << " | Size: " << node.size
        << " | Total Mass: " << node.totalMass
        << " | CoM: (" << node.cmX << ", " << node.cmY << ")";

    if (node.entityIndex != -1) {
        std::cout << " | Entity: " << node.entityIndex;
    }

    std::cout << std::endl;

    for (int i = 0; i < 4; i++) {
        if (node.children[i] != -1) {
            PrintTree(node.children[i], depth + 1);
        }
    }
}

void BHTree::ComputeForces(int nodeIndex, int entityIndex, float theta, double& forceX, double& forceY){
   if(nodeIndex == -1) return;

   Node& node = nodes[nodeIndex];
   Entity& entity = Entities::entities[entityIndex];

   if(node.entityIndex == entityIndex) return;
    
   double distX = node.cmX - entity.GetPosition().x;
   double distY = node.cmY - entity.GetPosition().y;

   double distSqr = distX * distX + distY * distY;
   double dist = std::sqrt(distSqr);

   if(dist < 1e-5) return;

    double s = node.size;

    if((s/dist) < theta){
        double f =  G * entity.GetMass() * node.totalMass / distSqr;
        forceX +=f*(distX/dist);
        forceY +=f*(distY/dist);
    }
    else{
        for(int i = 0; i < 4; i++){
            if(node.children[i] != -1){
                ComputeForces(node.children[i], entityIndex, theta, forceX, forceY);
            }
        }
    }
}





