//------------------------------------//
// Author: Breno Cunha Queiroz        //
// Date: 2020-11-17                   //
// SCC0650 - Computação Gráfica (2020)//
//------------------------------------//
#ifndef WINDOW_H
#define WINDOW_H
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <functional>

class Window
{
	public:
		Window(std::string name);
		~Window();

		void init();
		void loop();
		void close();
		void swapBuffers();

		//---------- Getters and Setters ----------//
		float getRatio();
		float getWidth() const { return _width; };
		float getHeight() const { return _height; };

		//---------- Callbacks ----------//
		std::function<void(int key, int scancode, int action, int mods)> onKey;
		std::function<void(double xpos, double ypos)> onMouse;
		std::function<void(int button, int action, int mods)> onMouseClick;
		std::function<void(double dt)> onDraw;

	private:
		static void keyCallback(GLFWwindow* window, const int key, const int scancode, const int action, const int mods);
		static void mouseCallback(GLFWwindow* window, double xpos, double ypos);
		static void mouseClickCallback(GLFWwindow* window, int button, int action, int mods);

		GLFWwindow* _window;
		std::string _name;
		int _width;
		int _height;

		double _time;
};

#endif// WINDOW_H
