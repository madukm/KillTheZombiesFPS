#ifndef GAME_MESSAGE_H
#define GAME_MESSAGE_H

#include "json.hpp"
#include "game_state.hpp"

using json = nlohmann::json; 

enum message_type
{
        HIT = 0,
        HEAL,
        MOVE,
        PICK,
        NEW_PLAYER
};

class GameMessage
{
    public:

    static const int buf_size = 1000;

	GameMessage(GameObj game_obj):
		_game_obj(game_obj)
	{
	}
	
	static GameMessage from_json(json parsed_obj)
	{
		GameObj gameObj = GameObj::from_json(parsed_obj);

		GameMessage ret(gameObj);

		if(parsed_obj["type"] == HIT){
			ret._type = HIT;
		}
		else if(parsed_obj["type"] == MOVE){
			ret._type = MOVE;
			ret._hitPlayer = parsed_obj["hitPlayer"];
		}

		return ret;
	}
 	
	json to_json()
	{
		json ret;
		ret["type"] = _type;

		if(_type == MOVE){
			ret["game_obj"] = _game_obj.to_json();			
		}
		if(_type == HIT){
			ret["hitPlayer"] = _hitPlayer;
		}

		return ret;
	}

	GameObj _game_obj;
	message_type _type;
	int _hitPlayer;
};

#endif
