#include "others.h"

using namespace std;

extern int tile_size,height,width,agent_num,turn,points[20][20],tiled[20][20],agent_exist[20][20],border;
extern team_info blue, orange;

void input_random_action(team_info& team) {
    int dxy[3] = {1,0,-1};
    rep(i,agent_num) {
        int y = team.agents[i].y, x = team.agents[i].x, Y, X;
        Y = y + dxy[XorShift()%3];
        X = x + dxy[XorShift()%3];
        while(!is_safe_index(Y,X)) {
            Y = y + dxy[XorShift()%3];
            X = x + dxy[XorShift()%3];
        }
        if (tiled[Y][X] && (tiled[Y][X] != team.teamID || points[Y][X] < 0)) team.agents[i].input_next_command(Y, X, "remove");
        else team.agents[i].input_next_command(Y, X, "move");
    }
}
