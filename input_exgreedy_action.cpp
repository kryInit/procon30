# include "others.hpp"

using namespace std;

extern int tile_size,height,width,agent_num,turn,points[20][20],tiled[20][20],agent_exist[20][20],border;
extern team_info blue, orange;

void calculate_distance_from_enemies(int y, int x, vector<vector<int>>& dfe, int now, int my_color);
bool check_next_enemies(int k, int agentID, team_info& team, int my_color, int enemy_color);
bool check_nearby_tiles(int k, int agentID, team_info& team, int my_color, int enemy_color, int dfe);

int calculation_of_expected_value(int y, int x, vector<vector<int>>& memo, int depth, int my_color, int enemy_color);

void input_exgreedy_action(team_info& team, int my_color, int enemy_color, int N) {
    vector<vector<int>> memo(height,vector<int>(width)), dfe(height,vector<int>(width));
    
    priority_queue<pair<int,pair<int,pair<int,pair<int,int>>>>> pq;
    rep(i,height) rep(j,width) memo[i][j] = tiled[i][j], dfe[i][j] = INF;
    rep(i,height) rep(j,width) if (agent_exist[i][j] == enemy_color) calculate_distance_from_enemies(i,j,dfe,0,my_color);
    
    
    rep(k,agent_num) {
        int agentID = k;
        if (check_next_enemies(k,agentID,team,my_color,enemy_color)) continue;
        
        
        
        int dx[3]={1,0,-1}, dy[3] = {1,0,-1},cnt=0,ty = team.agents[k].y,tx = team.agents[k].x,kthdfe=1000;
        
        
        rep(i,3) rep(j,3) {
            int Y = ty, X = tx;
            Y += dy[i];
            X += dx[j];
            if (Y < 0 || Y >= height || X < 0 || X >= width) continue;
            if (i == 1 && j == 1) kthdfe = min(kthdfe, dfe[Y][X]);
            else if (dfe[Y][X] != INF) kthdfe = min(kthdfe, dfe[Y][X]+1);
        }
        cout << "ID : " << team.agents[k].agentID << " " << kthdfe << endl;
        
        if (check_nearby_tiles(k,agentID,team,my_color,enemy_color,kthdfe)) continue;
        
        kthdfe = min(kthdfe, N);
        
        vector<pair<int,pair<int,pair<int,int>>>> vt;
        
        rep(i,3) rep(j,3) {
            if (i == 1 && j == 1) continue;
            int Y = ty, X = tx, hoge=0;
            Y += dy[i];
            X += dx[j];
            if (Y < 0 || Y >= height || X < 0 || X >= width) continue;
            if (agent_exist[Y][X]) continue;
            if (tiled[Y][X] == my_color) hoge = max(0LL,-points[Y][X]);
            else hoge = points[Y][X];
            vt.pb(mp(hoge, mp(agentID,mp(Y,X))));
        }
        sort(rall(vt));
        for(int i=0;i<vt.size() && (vt[i].fi >= border || i < 4);++i) {
            int Y = vt[i].se.se.fi, X = vt[i].se.se.se, depth = kthdfe, res=0, agentID = vt[i].se.fi;
            bool is_move=false;
            if (memo[Y][X] == my_color) {
                if (points[Y][X] < 0) {
                    int tmp1, tmp2;
                    memo[Y][X] = 0;
                    tmp1 = calculation_of_expected_value(ty,tx,memo,depth-1,my_color,enemy_color)-points[Y][X];
                    memo[Y][X] = my_color;
                    tmp2 = calculation_of_expected_value(Y,X,memo,depth-1,my_color,enemy_color)-points[Y][X];
                    if (res < tmp1) {
                        res = tmp1;
                        is_move = false;
                    }
                    if (res < tmp2) {
                        res = tmp2;
                        is_move = true;
                    }
                } else {
                    res = calculation_of_expected_value(Y,X,memo,depth-1,my_color,enemy_color);
                    is_move = true;
                }
            } else if (memo[Y][X] == enemy_color) {
                memo[Y][X] = 0;
                if (points[Y][X] < 0) res = max(res, calculation_of_expected_value(Y,X,memo,depth-1,my_color,enemy_color) + 2*points[Y][X]);
                else res = max(res, calculation_of_expected_value(ty,tx,memo,depth-1,my_color,enemy_color) + points[Y][X]);
                memo[Y][X] = enemy_color;
                is_move = false;
            } else {
                int tmp1=0, tmp2=0;
                tmp1 = calculation_of_expected_value(Y,X,memo,depth-1,my_color,enemy_color);
                memo[Y][X] = my_color;
                tmp2 = calculation_of_expected_value(Y,X,memo,depth-1,my_color,enemy_color) + points[Y][X];
                memo[Y][X] = 0;
                res = max(tmp1,tmp2);
                is_move = true;
            }
            pq.push(mp(res, mp(agentID, mp((int)is_move, mp(Y,X)))));
        }
    }
    cout << endl;
    set<int> st;
    set<pair<int,int>> stp;
    while(!pq.empty() && st.size() != agent_num) {
        int i = pq.top().se.fi, is_move = pq.top().se.se.fi;
        auto tmp = pq.top().se.se.se;
        
        cout << pq.top().fi << " " << i << " " << tmp.fi << " " << tmp.se << endl;
        
        pq.pop();
        if (st.find(i) == st.end() && stp.find(tmp) == stp.end()) {
            st.insert(i);
            stp.insert(tmp);
            if (is_move) team.agents[i].input_next_command(tmp.fi, tmp.se, "move");
            else team.agents[i].input_next_command(tmp.fi, tmp.se, "remove");
        }
    }
    cout << endl;
}

int calculation_of_expected_value(int y, int x, vector<vector<int>>& memo, int depth, int my_color, int enemy_color) {
    if (depth <= 0) return 0;
    int res=0,dx[3]={1,0,-1}, dy[3] = {1,0,-1}, tmp = memo[y][x];
    vector<pair<int,pair<int,int>>> vt;
    
    
    rep(i,3) rep(j,3) {
        if (i == 1 && j == 1) continue;
        int Y = y, X = x;
        Y += dy[i];
        X += dx[j];
        if (Y < 0 || Y >= height || X < 0 || X >= width) continue;
        if (agent_exist[Y][X]) continue;
        vt.pb(mp(points[Y][X], mp(Y,X)));
    }
    sort(rall(vt));
    for(int i=0;i<vt.size() && (vt[i].fi >= border || i < 3);++i) {
        int Y = vt[i].se.fi, X = vt[i].se.se;
        if (memo[Y][X] == my_color) {
            if (points[Y][X] < 0) {
                int tmp1, tmp2;
                memo[Y][X] = 0;
                tmp1 = calculation_of_expected_value(y,x,memo,depth-1,my_color,enemy_color);
                memo[Y][X] = my_color;
                tmp2 = calculation_of_expected_value(Y,X,memo,depth-1,my_color,enemy_color);
                res = max(res, max(tmp1,tmp2) + -points[Y][X]);
            } else res = max(res,calculation_of_expected_value(Y,X,memo,depth-1,my_color,enemy_color));
        } else if (memo[Y][X] == enemy_color) {
            memo[Y][X] = 0;
            if (points[Y][X] < 0) res = max(res, calculation_of_expected_value(Y,X,memo,depth-1,my_color,enemy_color) + 2*points[Y][X]);
            else res = max(res, calculation_of_expected_value(y,x,memo,depth-1,my_color,enemy_color) + points[Y][X]);
            memo[Y][X] = enemy_color;
        } else {
            int tmp1=0, tmp2=0;
            tmp1 = calculation_of_expected_value(Y,X,memo,depth-1,my_color,enemy_color);
            memo[Y][X] = my_color;
            tmp2 = max(res,calculation_of_expected_value(Y,X,memo,depth-1,my_color,enemy_color) + points[Y][X]);
            memo[Y][X] = 0;
            res = max(res, max(tmp1, tmp2));
        }
    }
    return res*0.7;
}

void calculate_distance_from_enemies(int y, int x, vector<vector<int>>& dfe, int now, int my_color) {
    int dx[3]={1,0,-1}, dy[3] = {1,0,-1};
    if (dfe[y][x] <= now) return;
    dfe[y][x] = now;
    rep(i,3) rep(j,3) {
        if (i == 1 && j == 1) continue;
        int Y=y, X=x;
        Y += dy[i];
        X += dx[j];
        if (Y < 0 || Y >= height || X < 0 || X >= width) continue;
        if (points[Y][X] < border || agent_exist[Y][X] == my_color) continue;
        calculate_distance_from_enemies(Y,X,dfe,now+1,my_color);
    }
}

bool check_next_enemies(int k, int agentID, team_info& team, int my_color, int enemy_color) {
    //相手のいるマスに1ターンで行ける時、対象が確実に動くと予想でき、かつそのマスの得点が高ければそのマスを消すようにしておく
    //下のコードでは、対象が移動でしか得点を獲得できないときかつマスの得点が高い時に行動を実行している
    
    int dx[3]={1,0,-1}, dy[3] = {1,0,-1},now_max=-20;
    pair<int,int> pm={};
    bool f=false;
    rep(i,3) rep(j,3) {
        if (i == 1 && j == 1) continue;
        int Y = team.agents[k].y, X = team.agents[k].x;
        Y += dy[i];
        X += dx[j];
        if (Y < 0 || Y >= height || X < 0 || X >= width) continue;
        if (now_max < points[Y][X] && agent_exist[Y][X] != my_color) {
            now_max = points[Y][X];
            if (agent_exist[Y][X] == enemy_color) pm = mp(Y,X), f=true;
        }
    }
    if (f && now_max == points[pm.fi][pm.se] && now_max > 0) {
        int cnt=0,ddx[5]={2,1,0,-1,-2}, ddy[5]={2,1,0,-1,-2},ec=0;
        rep(i,3) rep(j,3) {
            if (i == 1 && j == 1) continue;
            int Y = pm.fi, X = pm.se;
            Y += dy[i];
            X += dx[j];
            if (Y < 0 || Y >= height || X < 0 || X >= width) continue;
            if (tiled[Y][X] == my_color && !agent_exist[Y][X] && points[Y][X] > 0) cnt++;
            if (tiled[Y][X] == enemy_color && points[Y][X] < 0) cnt++;
        }
        rep(i,5) rep(j,5) {
            if (i==2 && j == 2) continue;
            int Y = pm.fi, X = pm.se;
            Y += ddy[i];
            X += ddx[j];
            if (Y < 0 || Y >= height || X < 0 || X >= width) continue;
            if (agent_exist[Y][X] == my_color) ec++;
        }
        cout << agentID << " " << team.agents[k].y << " " << team.agents[k].x << " " << cnt << " " << ec << endl;
        cout << "hi " << pm.fi << " " << pm.se << endl;
        if (cnt == 0 && ec <= 1) {
            team.agents[k].input_next_command(pm.fi, pm.se, "remove");
            return true;
        }
    }
    return false;
}

bool check_nearby_tiles(int k, int agentID, team_info& team, int my_color, int enemy_color, int dfe) {
    if (dfe < 6) return false;
    int dx[3]={1,0,-1}, dy[3] = {1,0,-1},tmp=100,x,y;
    rep(i,3) rep(j,3) {
        if (i == 1 && j == 1) continue;
        int Y = team.agents[k].y, X = team.agents[k].x;
        Y += dy[i];
        X += dx[j];
        if (tiled[Y][X] == my_color && tmp > points[Y][X]) {
            tmp = points[Y][X];
            x = X;
            y = Y;
        }
    }
    if (tmp < 0 && -tmp >= border) {
        team.agents[k].input_next_command(y, x, "remove");
        return true;
    }
    
    return false;
}


