#include "client.hpp"
#include "render/mesh.hpp"
#include "render/texture.hpp"
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/string_cast.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <set>
#include <functional>


Client::Client()
{
    _window = new Window("Kill the Zombies");
	_window->onKey = [this](const int key, const int scancode, const int action, const int mods) { onKey(key, scancode, action, mods); };
	_window->onMouse = [this](double xpos, double ypos) { onMouse(xpos, ypos); };
	_window->onMouseClick = [this](int button, int action, int mods) { onMouseClick(button, action, mods); };
	_window->onDraw = [this](double dt) { onDraw(dt); };
	_window->init();

	_camera  = nullptr;
	_camera = new Camera(1200/900.f);

	_log = new Logger("Client");

	_shader = new Shader();
	_shader->createFromFiles("../shaders/vert.glsl", "../shaders/frag.glsl");

	_ui = new UI(_shader);
    //_ui->setPlayers(_players);

    // Connection bootstraping
    //TODO add config file.
	_clientConnector = new ClientConnector(1338, "18.218.11.188");
    _this_player_id = _clientConnector->send_init_sequence("maoe");
    _player = new GameObj();

    printf("GOT SOCKET: %d\n", _clientConnector->get_socket());

    //Setting local player game obj
    _player->set_name("maoe");
    _player->set_id(_this_player_id);
}

Client::~Client() //Join threads.
{
	if(_window != nullptr)
	{
		delete _window;
		_window = nullptr;
	}

	if(_camera != nullptr)
	{
		delete _camera;
		_camera = nullptr;
	}

	if(_log != nullptr)
	{
		delete _log;
		_log = nullptr;
	}

	if(_ui != nullptr)
	{
		delete _ui;
		_ui = nullptr;
	}

	if(_shader != nullptr)
	{
		delete _shader;
		_shader = nullptr;
	}

	if(_clientConnector != nullptr)
	{
		delete _clientConnector;
		_clientConnector = nullptr;
	}

	for(auto& s : _players)
	{
		if(s.second != nullptr)
		{
			delete s.second;
			s.second = nullptr;
		}
	}

	for(auto& z : _zombies)
	{
		if(z != nullptr)
		{
			delete z;
			z = nullptr;
		}
	}

	if(Survivor::mesh != nullptr)
	{
		delete Survivor::mesh;
		Survivor::mesh = nullptr;
	}

	if(Survivor::texture != nullptr)
	{
		delete Survivor::texture;
		Survivor::texture = nullptr;
	}

	if(Block::mesh != nullptr)
	{
		delete Block::mesh;
		Block::mesh = nullptr;
	}

	//if(Block::texture != nullptr)
	//{
	//	delete Block::texture;
	//	Block::texture = nullptr;
	//}

	if(_sceneZero != nullptr)
	{
		delete _sceneZero;
		_sceneZero = nullptr;
	}
    /*
        should delete threads.
    */
}

void Client::run()
{
    loadAssets();
    createWorld();
	_window->loop();
}

void Client::loadAssets()
{
	Survivor::mesh = new Mesh("survivor.obj");
	Survivor::texture = new Texture("survivor.png");
	Zombie::mesh = new Mesh("survivor.obj");
	Zombie::texture = new Texture("zombie.png");
	Block::mesh = new Mesh("block.obj");
	//Block::texture = new Texture("brick.png");
}

void Client::createWorld()
{
    //Notify server that a new player is in.
	_sceneZero = new SceneZero(_shader);
	_camera->setSceneBlocks(_sceneZero->getBlocks());

    //Pass to player map <int, object*>
	_players[_this_player_id] = new Survivor(_shader, {0,0,0}, {0,0,0}, {0.5,0.5,0.5});
    //_players[_this_player_id] = new Survivor(_shader, {0,0,0}, {0,0,0}, {0.5,0.5,0.5});
	_camera->setPlayer((Survivor*)_players[_this_player_id]);
	_zombies.push_back(new Zombie(_shader, {0,1,2}, {0,0,0}, {0.5,0.5,0.5}));

    _ui->setZombies(_zombies);
}

void Client::messageSender() //This is a thread
{
    GameMessage sent_message;

    while(1)
    {
		//------------ Send 

        //Dequeue from messages.
        if(!message_queue.empty())
        {
            //Do this stuff
            //_clientConnector.
        }
        else
        {
            sent_message._type = MOVE;
            sent_message._game_obj = *_player;
        }
        _clientConnector->send_game_message(sent_message.to_json());

		//------------ Receive 
        json j_state = _clientConnector->receive_game_state(); 
		if(j_state.empty())
			continue;

        GameState received_state = GameState::from_json(j_state);
        //_this_player_id = GameState._player_id; //Setting self id.

        std::set<int> local_id_players;

        for (auto player : _players)
        {
            local_id_players.insert(player.first);
        }

        // Update positions and rotations of everybody
        for (auto [key, player] : received_state.players)
        {
            // Add spawned entities.            
            if (local_id_players.count(player.get_id()) == 0)
            {
                _players[player.get_id()] = new Survivor(_shader);
            }
            else local_id_players.erase(player.get_id());

            Survivor* local_player = (Survivor*)_players[player.get_id()];

            local_player->setPosition(player.get_position());
            local_player->setRotation(player.get_rotation());
            local_player->setScale(player.get_scale());
			std::cout << glm::to_string(local_player->getScale()) << std::endl;
            local_player->setId(player.get_id());
            local_player->setHealth(player.get_health());
            local_player->setPower(player.get_power());
            local_player->setName(player.get_name());

            local_player->setFly(player.get_fly());
            local_player->setFront(player.get_front());
            local_player->setMovingForward(player.get_moving_forward());
            local_player->setMovingLeft(player.get_moving_left());
        }

        //Check here if i was not killed.

        // Remove dead entities.
        for (int dead_player_id : local_id_players)
        {
            Object *dead_player_ptr = _players[dead_player_id];
            delete dead_player_ptr;
            _players.erase(dead_player_id);
        }
    }
}

void Client::stateReceiver()
{
    while(1)
    {
        // Receive state from server and update on client.
        // Parse game state
    }
}


//------------------------------//
//--------- Callbacks ----------//
//------------------------------//
void Client::onKey(int key, int scancode, int action, int mods)
{
	_camera->updateOnKey(key, scancode, action, mods);

	if(action == GLFW_RELEASE)
		return;

	switch(key)
	{
		case GLFW_KEY_SPACE:
			_camera->printInfo();
			break;
		case GLFW_KEY_ESCAPE:
			_window->close();
			break;
	}
}

void Client::onMouse(double xpos, double ypos)
{
	_camera->updateOnMouse(xpos/_window->getWidth(), ypos/_window->getHeight());
}

bool intersectTriangle(
    glm::vec3 rDir, glm::vec3 rOrigin, glm::vec3 A, glm::vec3 B, glm::vec3 C
	) 
{
	glm::vec3 E1 = B-A;
	glm::vec3 E2 = C-A;
	glm::vec3 N = glm::cross(E1,E2);
	float det = -glm::dot(rDir, N);
	float invdet = 1.0/det;
	glm::vec3 AO  = rOrigin - A;
	glm::vec3 DAO = glm::cross(AO, rDir);
	float u =  glm::dot(E2,DAO) * invdet;
	float v = -glm::dot(E1,DAO) * invdet;
	float t =  glm::dot(AO,N)  * invdet;
	return (det >= 1e-6 && t >= 0.0 && u >= 0.0 && v >= 0.0 && (u+v) <= 1.0);
}

void Client::onMouseClick(int button, int action, int mods)
{
	// Mouse click -> shot
	if(action == GLFW_PRESS && button == GLFW_MOUSE_BUTTON_1)
	{
		glm::vec3 rayDir = _camera->getFront();
		glm::vec3 rayOrig = _camera->getPosition();
		float distance = 999999;

		std::vector<glm::vec3> vertices = {
			glm::vec3(1.000000, 2.000000, -1.000000),
			glm::vec3(1.000000, -2.000000, -1.000000),
			glm::vec3(1.000000, 2.000000, 1.000000),
			glm::vec3(1.000000, -2.000000, 1.000000),
			glm::vec3(-1.000000, 2.000000, -1.000000),
			glm::vec3(-1.000000, -2.000000, -1.000000),
			glm::vec3(-1.000000, 2.000000, 1.000000),
			glm::vec3(-1.000000, -2.000000, 1.000000)};

		struct Indice
		{
			int v0;
			int v1;
			int v2;
		};

		std::vector<Indice> indices = {
			{5, 3, 1},
			{3, 8, 4},
			{7, 6, 8},
			{2, 8, 6},
			{1, 4, 2},
			{5, 2, 6},
			{5, 7, 3},
			{3, 7, 8},
			{7, 5, 6},
			{2, 4, 8},
			{1, 3, 4},
			{5, 1, 2}
		};

		for(auto& zombie : _zombies)
		{
			glm::mat4 model = zombie->getModelMat();

			bool intersection = false;

			// Check intersection with box triangles
			for(auto face : indices)
			{
				glm::vec3 v0 = glm::vec3(model*glm::vec4(vertices[face.v0-1], 1));
				glm::vec3 v1 = glm::vec3(model*glm::vec4(vertices[face.v1-1], 1));
				glm::vec3 v2 = glm::vec3(model*glm::vec4(vertices[face.v2-1], 1));

				if(intersectTriangle(rayDir, rayOrig, v0, v1, v2))
				{
					intersection = true;
					break;
				}
			}

			if(intersection)// TODO send message to server
				zombie->setHealth(std::max(zombie->getHealth()-0.1f, 0.0f));
		}

	}
}

void Client::onDraw(double dt)
{
	//_clientConnector->updateServerState(_survivors);

	_camera->update(dt);
	_player->set_position(_camera->getPosition()-glm::vec3(0,0.8,0));
	_player->set_rotation(_camera->getRotation());
	_player->set_scale({0.5,0.5,0.5});
	_player->set_fly(_camera->getFly());
	_player->set_front(_camera->getFront());
	_player->set_moving_forward(_camera->getMovingForward());
	_player->set_moving_left(_camera->getMovingLeft());

	// Clear window
	glClearColor(0.5f,0.5f,0.8f,1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// Populate view and projection matrices
	glm::mat4 view = glm::lookAt(glm::vec3(15,15,15), glm::vec3(0,0,0), glm::vec3(0,1,0));
	glm::mat4 projection = glm::perspective(
			glm::radians(60.0f),
			1200.0f/900,
			0.1f,
			1000.0f
		);

	// View matrix
	glUniformMatrix4fv(_shader->getViewLocation(), 1, GL_FALSE, _camera->getView());

	// Projection matrix
	glUniformMatrix4fv(_shader->getProjectionLocation(), 1, GL_FALSE, _camera->getProjection());

	// Use shader 0
	_shader->useShader();
	_shader->useOnlyTexture();
	_shader->setProcessLight(true);

	_sceneZero->draw();

	for(auto& p : _players)
	{
		if(((Survivor*)p.second)->getId() == _player->get_id())
			continue;

		((Survivor*)p.second)->move(dt);
		p.second->draw();
	}

	for(auto &z : _zombies)
	{
		z->draw();
	}

	_shader->setProcessLight(false);
	_ui->draw();
	
    // Swap buffers
	_window->swapBuffers();
}
