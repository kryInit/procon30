#include "others.hpp"

using namespace std;

extern int tile_size,height,width,agent_num,turn,points[20][20],tiled[20][20],agent_exist[20][20],border;
extern team_info blue, orange;


void initialize_to_zero();

void json_field();
void random_field();

void initialize_teams();
int tiles_score_decision();
void generate_tiles(string symmetric);
void generate_agents(string symmetric);
string determine_the_symmetry_of_tiles();
int select_best_border();


void initialize_for_local() {
    initialize_to_zero();
    
    initialize_teams();
//    json_field();  //Unimplemented
    random_field();
}

void json_field() {
    
}
void random_field() {
    rep(i,time(NULL)%100000) XorShift();
    height = width = (int)XorShift()%11 + 10;
    agent_num = (int)XorShift()%6 + 3;
    turn = (int)XorShift()%31 + 30;
    string symmetry=determine_the_symmetry_of_tiles();
    generate_tiles(symmetry);
    generate_agents(symmetry);
    border = select_best_border();
    Window::Resize(tile_size*width+200,tile_size*height+20);
    //    Graphics::SetTargetFrameRateHz(60);
}

void initialize_teams() {
    int z = 0;
    blue.teamID = 1;
    orange.teamID = 2;
    rep(i,8) blue.agents[i].agentID = z, z += XorShift()%10;
    rep(i,8) orange.agents[i].agentID = z, z += XorShift()%10;
}

string determine_the_symmetry_of_tiles() {
    if (height%2 && width%2) return "point_symmetry";
    if (height%2) return "y-axis_symmetry";
    if (width%2) return "x-axis_symmetry";
    
    string randres[3] = {"point_symmetry", "y-axis_symmetry", "x-axis_symmetry"};
    return randres[XorShift()%3];
}
int tiles_score_decision() {
    //nを大きくするとプラスのマスの数が多くなる must n > 0
    int n=3;
    int tmp = (int)XorShift()%33 - 16;
    while(tmp<0) {
        if (!((int)XorShift()%n)) break;
        tmp = (int)XorShift()%33 - 16;
    }
    return tmp;
}
void generate_tiles(string symmetric) {
    if (symmetric == "point_symmetry") rep(i,height) rep(j,width) points[i][j] = points[height-i-1][width-j-1] = tiles_score_decision();
    if (symmetric == "y-axis_symmetry") rep(i,height) rep(j,width/2) points[i][j] = points[i][width-j-1] = tiles_score_decision();
    if (symmetric == "x-axis_symmetry") rep(i,height/2) rep(j,width) points[i][j] = points[height-i-1][j] = tiles_score_decision();
}

void generate_agents(string symmetric) {
    map<pair<int,int>,bool> is_used;
    rep(i,agent_num) {
        pair<int,int> blue_xy,orange_xy;
        blue_xy = mp((int)XorShift()%height, (int)XorShift()%width);
        if (symmetric == "point_symmetry") orange_xy = mp(height-blue_xy.fi-1, width-blue_xy.se-1);
        if (symmetric == "y-axis_symmetry") orange_xy = mp(blue_xy.fi, width-blue_xy.se-1);
        if (symmetric == "x-axis_symmetry") orange_xy = mp(height-blue_xy.fi-1, blue_xy.se);
        
        if (blue_xy == orange_xy) i--;
        else if (!is_used[blue_xy]) {
            tiled[blue_xy.fi][blue_xy.se] = agent_exist[blue_xy.fi][blue_xy.se] = blue.teamID;
            tiled[orange_xy.fi][orange_xy.se] = agent_exist[orange_xy.fi][orange_xy.se] = orange.teamID;
            blue.tilePoint += points[blue_xy.fi][blue_xy.se];
            orange.tilePoint += points[orange_xy.fi][orange_xy.se];
            blue.agents[i].y = blue_xy.fi;
            blue.agents[i].x = blue_xy.se;
            orange.agents[i].y = orange_xy.fi;
            orange.agents[i].x = orange_xy.se;
            is_used[blue_xy] = is_used[orange_xy] = true;
        } else i--;
    }
}

int select_best_border() {
    int memo[33] = {},sum=0,border = (height*width*0.3);
    rep(i,height) rep(j,width) memo[points[i][j]+16]++;
    for(int i=32;i>=0;--i) {
        sum += memo[i];
        if (border <= sum) {
            return i-16;
        }
    }
    return -16;
}

void initialize_to_zero() {
    height = width = agent_num = turn = 0;
    blue.Initialize_to_zero();
    orange.Initialize_to_zero();
    rep(i,20) rep(j,20) points[i][j] = tiled[i][j] = agent_exist[i][j] = 0;
}
