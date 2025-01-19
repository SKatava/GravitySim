#include "Window.h"



void Window::Initialize(){
    glfwInit();

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);

	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	glfwWindowHint(GLFW_SAMPLES, 4);

	glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);
	//glfwWindowHint(GLFW_MAXIMIZED, GLFW_TRUE);

	window = glfwCreateWindow(WINDOW_WIDTH, WINDOW_HEIGHT, "Gravity Sim", NULL, NULL);
	if (window == NULL){
		std::cout << "Failed to create GLFW window\n";
		glfwTerminate();
		return;
	}
	
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