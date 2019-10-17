#include "others.hpp"

using namespace std;

extern int tile_size,height,width,agent_num,turn,points[20][20],tiled[20][20],agent_exist[20][20],border;
extern team_info blue, orange;


void print_tile(int i, int j);
void print_agent(int i, int j);
void print_score(int i, int j);
void print_information();


void print_various() {
    rep(i,height) rep(j,width) {
        print_tile(i,j);
        print_agent(i,j);
        print_score(i,j);
    }
    print_information();
}

void print_tile(int i, int j) {
    static Color blue_tile_color = Color(0,153,204), orange_tile_color = Color(255,215,0);
    static vector<vector<Rect>> R(20,vector<Rect>(20));
    static bool FIRST = true;
    
    if (FIRST) {rep(i,20) rep(j,20) R[i][j].set(j*tile_size+10, i*tile_size+10, tile_size, tile_size);FIRST = false;}
    
    if (tiled[i][j] || points[i][j] >= border) {
        if(tiled[i][j] == blue.teamID) R[i][j].draw(blue_tile_color);
        else if (tiled[i][j] == orange.teamID) R[i][j].draw(orange_tile_color);
        else R[i][j].draw(Palette::White);
    }
    R[i][j].drawFrame(1,0,Palette::Black);
}

void print_agent(int i, int j) {
    if (!agent_exist[i][j]) return;
    
    static Color blue_agent_color = Color(153,204,255), orange_agent_color = Color(255,153,102);
    Circle tmp(j*tile_size+tile_size/2+10,i*tile_size+tile_size/2+10, tile_size*0.45);
    
    if (agent_exist[i][j] == blue.teamID) tmp.draw(blue_agent_color);
    if (agent_exist[i][j] == orange.teamID) tmp.draw(orange_agent_color);
    tmp.drawFrame(1,0,Palette::Black);
}

void print_score(int i, int j) {
    static Font font(24);
    
    if (tiled[i][j] || points[i][j] >= border) font(points[i][j]).draw(Arg::topCenter = Vec2(j*tile_size+36, i*tile_size+18), Palette::Black);
    else font(points[i][j]).draw(Arg::topCenter = Vec2(j*tile_size+36, i*tile_size+18), Palette::White);
}

void print_information() {
    static vector<pair<String,int>> Printmemo(10);
    static Font font(24), score_font(30);
    
    
    Printmemo[0] = mp(U"blue score", blue.tilePoint);
    Printmemo[1] = mp(U"area score", blue.areaPoint);
    Printmemo[2] = mp(U"orange score", orange.tilePoint);
    Printmemo[3] = mp(U"area score", orange.areaPoint);
    Printmemo[4] = mp(U"res turn", turn);
    
    rep(i,5) {
        font(Printmemo[i].fi).draw(Arg::topCenter = Vec2(tile_size*width+100,i*90+30), Palette::White);
        score_font(Printmemo[i].se).draw(Arg::topCenter = Vec2(tile_size*width+100,i*90+70), Palette::White);
    }
}
