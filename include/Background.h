#ifndef BACKGROUND_CLASS_H
#define BACKGROUND_CLASS_H


#include <vector>
#include "Line.h"
#include "Vector2f.h"

class Background{
    public:
        void SetUp(int width, int height);
        void Draw();
        void Delete();
    private:
        std::vector<Line> m_grid; 
};

#endif
