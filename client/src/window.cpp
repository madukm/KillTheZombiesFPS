//------------------------------------//
// Author: Breno Cunha Queiroz        //
// Date: 2020-11-17                   //
// SCC0650 - Computação Gráfica (2020)//
//------------------------------------//
#include "window.hpp"
#include "defines.hpp"
#include "helpers/log.hpp"

Window::Window(std::string name):
	_name(name)
{
}

Window::~Window()
{

}

void Window::init()
{
	Log::verbose("Window", "Initializing window...");

    // Check if glfw was initialized
    if(!glfwInit())
    {
		Log::error("Window", "GLFW initialization failed!");
        glfwTerminate();
		exit(1);
    }

    //---------- GLFW config ----------//
	glfwWindowHint(GLFW_SAMPLES, 4);
    const GLFWvidmode* mode = glfwGetVideoMode(glfwGetPrimaryMonitor());
	_width = mode->width;
	_height = mode->height;

    //---------- Create window ----------//
    _window = glfwCreateWindow(_width, _height, _name.c_str(), glfwGetPrimaryMonitor(), nullptr);

	// Check if window was created
    if(_window == nullptr)
    {
		Log::error("Window", "Failed to create main window!");
        glfwTerminate();
        exit(1);
    }

    //---------- Config context ----------//
    // Set to draw to this window
    glfwMakeContextCurrent(_window);

    //---------- Callbacks ----------//
	glfwSetWindowUserPointer(_window, this);
	glfwSetInputMode(_window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	glfwSetKeyCallback(_window, keyCallback);
	glfwSetCursorPosCallback(_window, mouseCallback);
	glfwSetMouseButtonCallback(_window, mouseClickCallback);

	//---------- GLEW ----------//
	if(glewInit())
	{
		Log::error("Window", "Glew initialization failed!");
		glfwDestroyWindow(_window);
		glfwTerminate();
		exit(1);
	}

	//---------- OpenGL global state ----------//
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_MULTISAMPLE);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	Log::success("Window", "Initialized");
}

void Window::loop()
{
	_time = glfwGetTime();
	// Run in loop until the window is closed
	while(!glfwWindowShouldClose(_window))
	{
        glfwPollEvents();
		double lastTime = _time;
		_time = glfwGetTime();
		double dt = _time-lastTime;

		if(onDraw)
			onDraw(dt);
    }
}

void Window::close()
{
	if(_window != nullptr)
	{
		glfwSetWindowShouldClose(_window, GL_TRUE);
	}
}

float Window::getRatio()
{
	return _width/(float)_height;
}

void Window::swapBuffers()
{
	glfwSwapBuffers(_window);
}

//---------- Callbacks ----------//
void Window::keyCallback(GLFWwindow* window, const int key, const int scancode, const int action, const int mods)
{
	Window* const _this = static_cast<Window*>(glfwGetWindowUserPointer(window));
	if(_this->onKey)
	{
		_this->onKey(key, scancode, action, mods);
	}
}

void Window::mouseCallback(GLFWwindow* window, double xpos, double ypos)
{
	Window* const _this = static_cast<Window*>(glfwGetWindowUserPointer(window));
	if(_this->onMouse)
	{
		_this->onMouse(xpos, ypos);
	}
}

void Window::mouseClickCallback(GLFWwindow* window, int button, int action, int mods)
{
	Window* const _this = static_cast<Window*>(glfwGetWindowUserPointer(window));
	if(_this->onMouseClick)
	{
		_this->onMouseClick(button, action, mods);
	}
}
