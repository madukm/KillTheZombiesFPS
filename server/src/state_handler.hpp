/* handles incoming json stuff, and modifies
 * gamestate accordingly.
 * */

#include "../../shared/gamestate.hpp"

void handle_game_message(GameState &state, const json &message)
{
    if (message["type"] == 0)
    {
        int hitter_id = message["hitter_id"];
        int hitted_id = message["hitted_id"];

        state.players[hitted_id].decrease(state.players[hitter_id]);
    }
}


