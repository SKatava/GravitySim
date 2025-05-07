#ifndef BHTREE_CLASS_H
#define BHTREE_CLASS_H

#include <vector>
#include "Vector2d.h"

struct Node{
    double centerX;//8
    double centerY;//8
    double size; // 8
    double cmX; //8
    double cmY; //8
    double totalMass;//8
    int children[4];//4
    int entityIndex;//4
    // Total: 56
    
    Node() = default;
    Node(double new_centerX, double new_centerY, double new_size);
};

class BHTree{
    public:
        std::vector<Node> nodes;
        int rootIndex;

        BHTree();

        int CreateNode(double centerX, double centerY, double size);
        void RootNodeSetup();
        int GetQuadrant(Node node, Vector2d pos);
        Vector2d GetQuadCenter(Node node, int quad);
        void InsertEntity(int nodeIndex, int entityIndex);
        void UpdateMassAndCoM(int nodeIndex);
        void ComputeForces(int nodeIndex, int entityIndex, float theta, double& forceX, double& forceY);
        void PrintTree(int nodeIndex, int depth = 0);
    private:
};

#endif
