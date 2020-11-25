typedef struct
{
    enum //substituido por union.
    {
        HIT,
        HEAL,
        MOVE,
        PICK
    } m_type; //message type.

    //message payload
    union 
    {
        int hit_player; //
        std::tuple<float, float, float> coord;
    }

    unsigned int who; //Bodie who has 'suffered'.
    int meta; //Metadata
} game_message;
/*
 * {
 *  "hit":{
 *      "playerid" : 
 *  }
 *  "move":{
 *      "coord" : [x, y, z]
 *  }
 *
 * }
 *
 *
 * */

