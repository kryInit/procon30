#ifndef structure_hpp
#define structure_hpp
//defineや構造体の定義、いろいろな場所で使う関数などの定義を行なっている

#include <Siv3D.hpp>
#include "picojson.h"
#include <fstream>

using namespace std;

#define int long long
#define fi first
#define se second
#define rep(i,n) for(int i=0;i<(int)(n);++i)
#define rep1(i,n) for(int i=1;i<=(int)(n);++i)
#define rep11(i,n) for(int i=1;i<(int)(n);++i)
#define repo(i,o,n) for(int i=o;i<(int)(n);++i)
#define repm(i,n) for(int i=(int)(n)-1;i>=0;--i)
#define all(v) (v).begin(),(v).end()
#define rall(v) (v).rbegin(),(v).rend()
#define pb(n) push_back(n)
#define mp make_pair
#define MOD 1000000007
#define INF 922337203685477580

struct agent_info{
    int agentID,x,y,next_x, next_y;
    string next_command;
    pair<int,int> xy() {return mp(y, x);}
    pair<int,int> next_xy(){return mp(next_y, next_x);}
    void Initialize_to_zero() {
        agentID = x = y = next_x = next_y = 0;
        next_command.clear();
    }
    void Initialize_next_command() {
        next_x = next_y = 0;
        next_command.clear();
    }
    void input_next_command(int Y, int X, string COMMAND) {
        next_y = Y;
        next_x = X;
        next_command = COMMAND;
    }
};

struct team_info {
    int teamID,tilePoint,areaPoint;
    agent_info agents[8];
    
    void Initialize_to_zero() {
        teamID = tilePoint = areaPoint = 0;
        rep(i,8) agents[i].Initialize_to_zero();
    }
    void Initialize_all_next_command() {
        rep(i,8) agents[i].Initialize_next_command();
    }
};
struct action_command {
    int agentID,dx,dy,turn,apply;
    string type;
};


unsigned XorShift(void);
bool is_safe_index(int y, int x);
int area_tile_score_count(int team_color);


#endif /* structure_hpp */
