#include "../include/Background.h"

//Create the background(stored in m_grid vector)
void Background::SetUp(int width, int height){
    m_grid.clear();
    for(int i = 0; i < width/10; i++){
        m_grid.push_back(Line(Vector2f(-1 + 0.2 * i, -1), Vector2f(-1 + 0.2 * i, 1)));
        m_grid.back().SetColor(0.2, 0.2, 0.2);
        m_grid.back().Update();
    }
    for(int i = 0; i < height; i++){
        m_grid.push_back(Line(Vector2f(-1, -1 + 0.2 * i), Vector2f(width, -1 + 0.2 * i)));
        m_grid.back().SetColor(0.2, 0.2, 0.2);
        m_grid.back().Update();
    }
}

//Draw the Background with the m_grid vector
void Background::Draw(){
    for(int i = 0; i < m_grid.size(); i++){
        m_grid[i].Draw();
    }
}

//Delete the m_grid vector containing all data
void Background::Delete(){
    for(int i = 0; i < m_grid.size(); i++){
        m_grid[i].Delete();
    }
}
