#include "others.hpp"

using namespace std;

extern int tile_size,height,width,agent_num,turn,points[20][20],tiled[20][20],agent_exist[20][20],border;
extern team_info blue, orange;


unsigned Xseed[4];
void initialize_XorShift(unsigned s) {
    for(int i=1; i<=4; i++){
        Xseed[i-1] = s = 1812433253 * (s^(s>>30)) + (unsigned)i;
    }
}
unsigned XorShift(void) {
    unsigned t;
    t=(Xseed[0]^(Xseed[0]<<11));
    Xseed[0]=Xseed[1];
    Xseed[1]=Xseed[2];
    Xseed[2]=Xseed[3];
    return Xseed[3]=(Xseed[3]^(Xseed[3]>>19))^(t^(t>>8));
}

bool is_safe_index(int y, int x) {
    return y >= 0 && x >= 0 && y < height && x < width;
}

int area_tile_score_count(int team_color) {
    struct area_tile_score_count_struct {
        int region[20][20];
        void Init() {rep(i,height) rep(j,width) region[i][j] = 1;}
        void dfs(int y, int x, int __color) {
            if (tiled[y][x] == __color) return;
            region[y][x] = 0;
            int dx[4] = {1,0,-1,0}, dy[4] = {0,1,0,-1};
            rep(i,4) {
                int Y = y+dy[i], X = x+dx[i];
                if (0 <= Y && Y < height && 0 <= X && X < width) {
                    if (region[Y][X]) dfs(Y, X, __color);
                }
            }
        }
        int count(int __color) {
            int tmp_masu[22][22]={},ans=0;
            rep(i,height) rep(j,width) tmp_masu[i+1][j+1] = region[i][j];
            rep1(i,height) rep1(j,width) {
                bool f = true;
                int dx[4] = {1,0,-1,0}, dy[4] = {0,1,0,-1};
                rep(k,4) if (!tmp_masu[i+dy[k]][j+dx[k]]) f = false;
                if (tiled[i-1][j-1] == __color) f = false;
                if (f) ans += abs(points[i-1][j-1]);
            }
            return ans;
        }
    };
    area_tile_score_count_struct atscs;
    atscs.Init();
    rep(i,height) atscs.dfs(i,0,team_color),atscs.dfs(i,width-1,team_color);
    rep(i,width) atscs.dfs(0,i,team_color),atscs.dfs(height-1,i,team_color);
    return atscs.count(team_color);
}


