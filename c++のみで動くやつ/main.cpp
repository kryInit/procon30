#include <bits/stdc++.h>
#include "main.h"

using namespace std;

int height,width,agent_num,turn,points[20][20],tiled[20][20],agent_exist[20][20],border=-16,tile_size=50;
team_info blue,orange;

signed main() {
    initialize_for_local();
    cout << "blue   teamID " << blue.teamID << endl;
    cout << "orange teamID " << orange.teamID << endl;
    rep(i,height) {
        rep(j,width) printf("%4d", points[i][j]);
        cout << endl;
    }
    cout << endl;
    while(turn!=0) {
        rep(i,height) {
            rep(j,width) cout << tiled[i][j] << " ";
            cout << endl;
        }
        cout << endl;
        cout << "blue   tile score " << blue.tilePoint << endl;
        cout << "blue   area score " << blue.areaPoint << endl;
        cout << "orange tile score " << orange.tilePoint << endl;
        cout << "orange area score " << orange.areaPoint << endl;
        cout << "rest turn " << turn << endl;
        cout << endl;

        input_random_action(blue);
        input_random_action(orange);
        local_transition();
    }

    rep(i,height) {
        rep(j,width) cout << tiled[i][j] << " ";
        cout << endl;
    }
    cout << endl;
    cout << "blue   tile score " << blue.tilePoint << endl;
    cout << "blue   area score " << blue.areaPoint << endl;
    cout << "orange tile score " << orange.tilePoint << endl;
    cout << "orange area score " << orange.areaPoint << endl;
    cout << "rest turn " << turn << endl;
    cout << endl;

}



