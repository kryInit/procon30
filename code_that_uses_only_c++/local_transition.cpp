#include "others.h"

using namespace std;

extern int tile_size,height,width,agent_num,turn,points[20][20],tiled[20][20],agent_exist[20][20],border;
extern team_info blue, orange;

void run_all_action();
bool run_command(team_info& team, map<pair<int,int>,int> conflict_judgement);
void update_score();

void local_transition() {
    run_all_action();
    update_score();
    --turn;
}

void run_all_action() {
    bool FINISH = false;
    while(!FINISH) {
        FINISH = true;
        map<pair<int,int>,int> conflict_judgement;
        rep(i,agent_num) {
            conflict_judgement[blue.agents[i].xy()]++;
            conflict_judgement[orange.agents[i].xy()]++;
            if (!blue.agents[i].next_command.empty()) conflict_judgement[blue.agents[i].next_xy()]++;
            if (!orange.agents[i].next_command.empty()) conflict_judgement[orange.agents[i].next_xy()]++;
        }
        if (run_command(blue, conflict_judgement)) FINISH = false;
        if (run_command(orange, conflict_judgement)) FINISH = false;
    }
    blue.Initialize_all_next_command();
    orange.Initialize_all_next_command();
}

bool run_command(team_info& team, map<pair<int,int>,int> conflict_judgement) {
    bool result = false;
    rep(i,agent_num) {
        int y = team.agents[i].y, x = team.agents[i].x, Y = team.agents[i].next_y, X = team.agents[i].next_x;
        if (conflict_judgement[mp(Y,X)] == 1 && is_safe_index(Y,X) && !agent_exist[Y][X]) {
            if (team.agents[i].next_command == "move" && !(tiled[Y][X] && tiled[Y][X] != team.teamID)) {
                agent_exist[y][x] = 0;
                tiled[Y][X] = agent_exist[Y][X] = team.teamID;
                team.agents[i].y = team.agents[i].next_y;
                team.agents[i].x = team.agents[i].next_x;
                team.agents[i].next_command.clear();
                result = true;
            }
            if (team.agents[i].next_command == "remove" && tiled[y][x]) {
                tiled[Y][X] = 0;
                team.agents[i].next_command.clear();
                result = true;
            }
        }
    }
    return result;
}


void update_score() {
    int blue_tile_points=0, orange_tile_points=0;
    rep(i,height) rep(j,width) {
        if (tiled[i][j] == blue.teamID) blue_tile_points += points[i][j];
        if (tiled[i][j] == orange.teamID) orange_tile_points += points[i][j];
    }
    blue.tilePoint = blue_tile_points;
    orange.tilePoint = orange_tile_points;
    blue.areaPoint = area_tile_score_count(blue.teamID);
    orange.areaPoint = area_tile_score_count(orange.teamID);
}
