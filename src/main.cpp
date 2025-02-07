#define STB_IMAGE_IMPLEMENTATION  
#include "stb_image.h"


#include "Window.h"

int main(){
    Window window;
    window.Initialize();
    window.Draw();
    window.Clean();
    return 0;
}