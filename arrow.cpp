#include "others.hpp"

using namespace std;

extern int tile_size,height,width,agent_num,turn,points[20][20],tiled[20][20],agent_exist[20][20],border;
extern team_info blue, orange;

void get_arrow();
void print_determined_action(team_info team);


void print_arrow() {
    get_arrow();
    print_determined_action(blue);
    print_determined_action(orange);
}

void get_arrow() {
    // before/after == before/after compression
    static pair<int,int> before;
    static string command;
    static Color action_color;
    static bool while_dragging = false;
    if (MouseL.down()) command = "move", action_color = Color(0,102,204);
    if (MouseR.down()) command = "remove", action_color = Color(255,102,51);
    
    if (!command.empty() && !while_dragging) {
        int i,j;
        pair<int,int> after;
        before = mp(Cursor::Pos().y, Cursor::Pos().x);
        after = mp((before.fi-10)/tile_size, (before.se-10)/tile_size);
        before = mp(after.fi*tile_size + tile_size/2+10, after.se*tile_size + tile_size/2+10);
        i = after.fi;
        j = after.se;
        if (is_safe_index(i,j) && agent_exist[i][j]) while_dragging = true;
        else command.clear();
    }
    if ((MouseL.pressed() || MouseR.pressed()) && while_dragging) Line(before.se, before.fi, Cursor::Pos().x, Cursor::Pos().y).drawArrow(5,Vec2(10,10), action_color);
    if ((MouseR.up() || MouseL.up()) && while_dragging) {
        pair<int,int> agent_location, tmp;
        agent_location = mp((before.fi-10)/tile_size, (before.se-10)/tile_size);
        tmp = mp((Cursor::Pos().y-10)/tile_size, (Cursor::Pos().x-10)/tile_size);
        if (abs(agent_location.fi - tmp.fi) > 1 || abs(agent_location.se - tmp.se) > 1) {
            command.clear();
            return;
        }
        pair<int,int> after;
        after = mp((Cursor::Pos().y-10)/tile_size, (Cursor::Pos().x-10)/tile_size);
        if (is_safe_index(after.fi, after.se)) {
            rep(i,agent_num) {
                if (blue.agents[i].xy() == agent_location) {
                    blue.agents[i].next_y = after.fi;
                    blue.agents[i].next_x = after.se;
                    blue.agents[i].next_command = command;
                }
                if (orange.agents[i].xy() == agent_location) {
                    orange.agents[i].next_y = after.fi;
                    orange.agents[i].next_x = after.se;
                    orange.agents[i].next_command = command;
                }
            }
        }
        while_dragging = false;
        command.clear();
    }
}

void print_determined_action(team_info team) {
    rep(i,agent_num) {
        if (!team.agents[i].next_command.empty()) {
            int C = tile_size/2 + 10;
            Color action_color;
            pair<int,int> before, after;
            before = mp(team.agents[i].y*tile_size+C, team.agents[i].x*tile_size+C);
            after = mp(team.agents[i].next_y*tile_size+C, team.agents[i].next_x*tile_size+C);
            if (team.agents[i].next_command == "move") action_color = Color(0,102,204);
            if (team.agents[i].next_command == "remove") action_color = Color(255,102,51);
            Line(before.se, before.fi,after.se,after.fi).drawArrow(5,Vec2(10,10), action_color);
        }
    }
}
