#include "json.hpp"
#include "gamestate.hpp"
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
	GameMessage(){};
	GameMessage(int idPlayer):
		game_obj(idPlayer)
	{}
	
	int getIdPlayer(){ return game_obj.get_id(); }

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
	}

private:
	GameObj game_obj;
	message_type _type;
	unsigned int _hitPlayer;
};
