#ifndef CLIENT_H
#define CLIENT_H

#include <iostream>
#include <chrono>
#include <thread>
#include <vector>

#include "clientConnector.hpp"
#include "../../shared/game_state.hpp" //TODO add in cmake to include <gamestate.hpp>
#include "../../shared/game_message.hpp"
#include "../../shared/semaphore.hpp"
#include "../../shared/logger.hpp"
#include "render/window.hpp"
#include "render/shader.hpp"
#include "render/camera.hpp"
#include "render/ui.hpp"
#include "objects/survivor.hpp"
#include "objects/zombie.hpp"
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

        // ???
		GameState* _state;
		GameObj* _player;
		Semaphore* _sem;

		ClientConnector* _clientConnector;
		std::thread _update_thread;
		Logger* _log;

		// Rendering specific
		Window* _window;
		Shader* _shader;
		UI* _ui;

		// Game specific
		Camera* _camera;
		SceneZero* _sceneZero;
		
        std::unordered_map<int, Object*> _players;
        std::vector<Zombie*> _zombies;

        // Server connection
        void messageSender();
        void stateReceiver();

        std::thread sender_thread;
        std::thread receiver_thread;

        std::queue<GameMessage> message_queue;
        int _this_player_id;
};
#endif// CLIENT_H
