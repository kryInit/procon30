#include "others.hpp"

using namespace std;

extern int tile_size,height,width,agent_num,turn,points[20][20],tiled[20][20],agent_exist[20][20],border;
extern team_info blue, orange;

struct all_values{
    int _points[20][20], _tiled[20][20], _agent_exist[20][20];
    team_info _blue, _orange;
};

vector<all_values> LOG;

void log_all_values() {
    all_values tmp;
    rep(i,height) rep(j,width) {
        tmp._points[i][j] = points[i][j];
        tmp._tiled[i][j] = tiled[i][j];
        tmp._agent_exist[i][j] = agent_exist[i][j];
    }
    tmp._blue = blue;
    tmp._orange = orange;
    LOG.pb(tmp);
}
void rollback() {
    if (LOG.empty()) return;
    turn++;
    rep(i,height) rep(j,width) {
        points[i][j] = LOG.back()._points[i][j];
        tiled[i][j] = LOG.back()._tiled[i][j];
        agent_exist[i][j] = LOG.back()._agent_exist[i][j];
    }
    blue = LOG.back()._blue;
    orange = LOG.back()._orange;
    LOG.pop_back();
}
