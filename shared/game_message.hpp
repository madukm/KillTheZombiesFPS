#ifndef GAME_MESSAGE_H
#define GAME_MESSAGE_H

#include "json.hpp"
#include "game_state.hpp"
#include "../client/src/render/camera.hpp"

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

    static const int buf_size = 1000;

	GameMessage(){};
	GameMessage(int idPlayer):
		game_obj(idPlayer)
	{}
	
	static GameMessage from_json(json parsed_obj)
	{
		GameMessage ret;
		if(parsed_obj["type"] == HIT){
			ret._type = HIT;
			ret.game_obj.from_json(parsed_obj);
		}
		else if(parsed_obj["type"] == MOVE){
			ret._type = MOVE;
			ret.game_obj.from_json(parsed_obj);
			ret._hitPlayer = parsed_obj["hitPlayer"];
		}

		return ret;
	}
 	
	json to_json()
	{
		json ret;
		ret["type"] = _type;
		if(_type == MOVE){
			ret = game_obj.to_json();
		}
		if(_type == HIT){
			ret["hitPlayer"] = _hitPlayer;
		}

		return ret;
	}

	GameObj game_obj;
	message_type _type;
	unsigned int _hitPlayer;
};

#endif
