#include "Window.h"
#include <stb_image.h>


bool Window::IsDarkModeEnabled(){
	DWORD value;
    HKEY hKey;
    if (RegOpenKeyExW(HKEY_CURRENT_USER, L"Software\\Microsoft\\Windows\\CurrentVersion\\Themes\\Personalize", 0, KEY_READ, &hKey) == ERROR_SUCCESS) {
        DWORD dataSize = sizeof(value);
        if (RegQueryValueExW(hKey, L"AppsUseLightTheme", nullptr, nullptr, reinterpret_cast<LPBYTE>(&value), &dataSize) == ERROR_SUCCESS) {
            RegCloseKey(hKey);
            return value == 0;
        }
        RegCloseKey(hKey);
    }
    return false; 
}

void Window::ApplyDarkTitleBar(GLFWwindow* window, bool enableDark){
	HWND hwnd = glfwGetWin32Window(window);
    if (hwnd) {
        BOOL value = enableDark ? TRUE : FALSE;
        DwmSetWindowAttribute(hwnd, DWMWA_USE_IMMERSIVE_DARK_MODE, &value, sizeof(value));
    }
}

void Window::SetWindowIcon(GLFWwindow* window){
	GLFWimage images[1];  

    int width, height, channels;
    unsigned char* pixels = stbi_load("../icon.png", &width, &height, &channels, 4);
    
    if (pixels) {
        images[0].width = width;
        images[0].height = height;
        images[0].pixels = pixels;

        glfwSetWindowIcon(window, 1, images);
        stbi_image_free(pixels);
    } else {
        std::cerr << "Failed to load icon!" << std::endl;
    }
}

void Window::Initialize(){
    glfwInit();

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);

	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	glfwWindowHint(GLFW_SAMPLES, 4);

	glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);

	window = glfwCreateWindow(WINDOW_WIDTH, WINDOW_HEIGHT, "GravitySim", NULL, NULL);
	if (window == NULL){
		std::cout << "Failed to create GLFW window\n";
		glfwTerminate();
		return;
	}
	

	ApplyDarkTitleBar(window, IsDarkModeEnabled());
	SetWindowIcon(window);
	glfwMakeContextCurrent(window);
	glfwSetWindowPos(window, 70, 40);
    gladLoadGL();

	glViewport(0, 0, 1000, 1000);

	bg.SetUp(1000, 1000);
    
	GUI::Initialize(window);
	
}

void Window::Draw(){
    Shader shaderProgram("../shaders/default.vert", "../shaders/default.frag");
	double lastTime = glfwGetTime();
	double currentTime;

	OBJMG::AddObject(1, Vector2f(300.f, 500.f), Vector3f(1.f, 0.f, 0.f));	
	OBJMG::AddObject(1000000000, Vector2f(500.f, 500.f), Vector3f(0.f, 0.f, 1.f));
	OBJMG::objects[0].SetInitialVelocity(Vector2f(0.f, std::sqrt(0.000667430 * 1000000000 / 200)));
	STOP_SIM = 1;
    while (!glfwWindowShouldClose(window)){
		glClearColor(0.0f, 0.0f, 0.f, 1.f);
		glClear(GL_COLOR_BUFFER_BIT);
        shaderProgram.Activate();

		currentTime = glfwGetTime();
		if(!SIM_MODE){
			deltaTime = currentTime - lastTime;
		}
		RUNTIME += currentTime - lastTime;
		lastTime = currentTime;
		
		
		if(!STOP_SIM){
			FRAME_COUNTER++;
			SIM_RUNTIME  += deltaTime;
			if(OBJMG::objects.size() > 1){
				GEN::Step();
			}
			OBJMG::UpdateObjects();
		}
		bg.Draw();
		OBJMG::DrawObjects();
		if(!glfwGetWindowAttrib(window, GLFW_ICONIFIED)){
			GUI::Draw();
		}

        glfwSwapBuffers(window);
	    glfwPollEvents();
    }
	
	bg.Delete();
	OBJMG::DeleteObjects();
    shaderProgram.Delete();
}

void Window::Clean(){
	GUI::Delete();
	glfwDestroyWindow(window);
	glfwTerminate();
}