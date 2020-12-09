#ifndef CLIENT_H
#define CLIENT_H
#include <iostream>
#include <chrono>
#include <thread>
#include <vector>
//#include "clientConnector.hpp"
#include "../../shared/gamestate.hpp" //TODO add in cmake to include <gamestate.hpp>
#include "../../shared/semaphore.hpp"
#include "../../shared/logger.hpp"
#include "render/window.hpp"
#include "render/shader.hpp"
#include "objects/survivor.hpp"
#include "objects/block.hpp"
#include "objects/sceneZero.hpp"

class Client
{
    public:
		Client();
		~Client();

		void run();

	private:
		void loadAssets();
		void createWorld();

		//---------- Callbacks ----------//
		void onKey(int key, int scancode, int action, int mods);
		void onMouse(double xpos, double ypos);
		void onMouseClick(int button, int action, int mods);
		void onDraw(double dt);

		GameState* _state;
		GameObj* _player;
		Semaphore* _sem;

		//ClientConnector _connector;
		std::thread _update_thread;
		Logger* _log;

		// Rendering specific
		Window* _window;
		Shader* _shader;

		// Game specific
		std::vector<Survivor*> _survivors;
		SceneZero* _sceneZero;

};
#endif// CLIENT_H
