#include "../../shared/json.hpp"
#include "render/camera.hpp"

using json = nlohmann::json; 

enum message_type
{
        HIT,
        HEAL,
        MOVE,
        PICK
};

class GameMessage
{
public:
	GameMessage(){};
	GameMessage(unsigned int idPlayer) :
		_idPlayer(idPlayer)
	{}
	
	unsigned int getIdPlayer(){ return _idPlayer; }

	json getMoveMessageJson(message_type m, std::tuple<float, float, float> coord)//, Camera &camera)
	{
		json moveMessageJson;
		moveMessageJson["type"] = m;
		moveMessageJson["type"]["idPlayer"] = _idPlayer;  
		moveMessageJson["type"]["coord"] = coord; 
		return moveMessageJson;		
	}

	json getHitMessageJson(message_type m, unsigned int hitPlayer)
	{
		json hitMessageJson;
	 	hitMessageJson["type"] = m;
	 	hitMessageJson["type"]["idPlayer"] = _idPlayer; 
	 	hitMessageJson["type"]["hitPlayer"] = hitPlayer; 
		return hitMessageJson;
	}

private:
	unsigned int _idPlayer;
	
};
