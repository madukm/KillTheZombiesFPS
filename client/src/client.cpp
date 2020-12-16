#include "client.hpp"
#include "render/mesh.hpp"
#include "render/texture.hpp"
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

Client::Client()
    //: _sem(1), _connector(ClientConnector(state, 42069, "127.0.0.1", sem))
{
	//update_thread = std::thread(&ClientConnector::update, &connector);
	
	_clientConnector = new ClientConnector();
	_clientConnector->connectToServer();

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

	loadAssets();
	createWorld();
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

	for(auto& survivor : _survivors)
	{
		if(survivor != nullptr)
		{
			delete survivor;
			survivor = nullptr;
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

	if(Block::texture != nullptr)
	{
		delete Block::texture;
		Block::texture = nullptr;
	}

	if(_sceneZero != nullptr)
	{
		delete _sceneZero;
		_sceneZero = nullptr;
	}
}


void Client::run()
{
	_window->loop();
}

void Client::loadAssets()
{
	Survivor::mesh = new Mesh("survivor.obj");
	Survivor::texture = new Texture("survivor.png");
	Block::mesh = new Mesh("block.obj");
	Block::texture = new Texture("brick.png");
}

void Client::createWorld()
{
	_survivors.push_back(new Survivor(_shader));
	_survivors.push_back(new Survivor(_shader));
	_sceneZero = new SceneZero(_shader);
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

void Client::onMouseClick(int button, int action, int mods)
{
	//if(action == GLFW_PRESS)
	//	_log->log("Mouse button pressed", DEBUG);
	//else
	//	_log->log("Mouse button released", DEBUG);
}

void Client::onDraw(double dt)
{
	_clientConnector->updateServerState(_survivors);

	_camera->update(dt);
	_survivors[0]->setPosition(_camera->getPosition()-glm::vec3(0,1.5,0));
	_survivors[0]->setRotation(-_camera->getRotation());

	// Clear window
	glClearColor(1.0f,0.7f,0.7f,1.0f);
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

	_sceneZero->draw();

	for(auto& s : _survivors)
	{
		s->draw();
	}

	_ui->draw();

	//while(true)
	//{
	//	//mutx.lock();
	//	//client_log.log("Ondraw", logger::log_type::DEBUG);
	//	//printf("Ondraw %d\n", state.a);
	//	sem.down();
	//	printf("Ondraw %d\n", state.a);
	//	state.a += 10;
	//	sem.up();
	//	std::this_thread::sleep_for(std::chrono::milliseconds(100));
	//}
	
	// Swap buffers
	_window->swapBuffers();
}
