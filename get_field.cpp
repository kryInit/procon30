#include "others.hpp"

using namespace std;


picojson::value actions_json_make();
void send_json_to_server(picojson::value data);
void exist_update();
void field_update(picojson::value fld_info);
void agents_update(picojson::value fld_info);
picojson::value get_match_info();
picojson::value get_field_info();

extern int tile_size,height,width,agent_num,turn,points[20][20],tiled[20][20],agent_exist[20][20],border;
extern team_info blue, orange;


string token = "kr";
string port_num, matchID, status;
int matchID_n, myteamID, enemyteamID;
extern int startAtUnixTime, startTimeMilli, nowTimeMilli, nextTurnTimeMilli, intervalMillis, turnMillis, startedAtUnixTime, max_turn;

void initialize_for_sever() {
    printf("ポート番号 (Port Number) => ");
    cin >> port_num;
    printf("マッチID (Match ID) => ");
    cin >> matchID_n;
    matchID = to_string(matchID_n);
    
    //これから行う全試合の情報の中からmatchIDが等しいものを取り出して情報を得る
    picojson::value match_info = get_match_info();
    picojson::value::array match_array = match_info.get<picojson::value::array>();
    for (picojson::value& match_data : match_array) {
        int ID = (int)(match_data.get<picojson::object>()["id"].get<double>());
        if (ID == matchID_n) {
            intervalMillis = (int)(match_data.get<picojson::object>()["intervalMillis"].get<double>());
            turnMillis = (int)(match_data.get<picojson::object>()["turnMillis"].get<double>());
            myteamID = (int)(match_data.get<picojson::object>()["teamID"].get<double>());
            turn = (int)(match_data.get<picojson::object>()["turns"].get<double>());
            max_turn = turn;//max_turnはここで保存 仮想通信ではv_get_field_info_2()のInit()の後に更新される
        }
    }
    
    cout << "matchID : " << matchID << endl;
    cout << "intervalMillis : " << intervalMillis << endl;
    cout << "turnMillis : " << turnMillis << endl;
    cout << "myteamID : " << myteamID << endl;
    cout << endl;
    
    //競技開始後、フィールド情報の取得と初期化更新
    picojson::value fld_info;
    fld_info = get_field_info();
    
    //width, height, startedAtUnixTime, turn
    width = (int)(fld_info.get<picojson::object>()["width"].get<double>());
    height = (int)(fld_info.get<picojson::object>()["height"].get<double>());
    startedAtUnixTime = (int)(fld_info.get<picojson::object>()["startedAtUnixTime"].get<double>());
    turn = (int)(fld_info.get<picojson::object>()["turn"].get<double>());
    cout << "width : " << width << endl;
    cout << "height : " << height << endl;
    cout << "startedAtUnixTime : " << startedAtUnixTime << endl;
    cout << "turn : " << turn << endl;
    cout << endl;
    
    startAtUnixTime = startedAtUnixTime;
    startTimeMilli = startAtUnixTime * 1000;
    nextTurnTimeMilli = startTimeMilli + turnMillis + intervalMillis;
    
    //points[][]
    int i, j;
    i = 0, j = 0;
    cout << "points[][]" << endl;
    picojson::value::array arrays_array = fld_info.get<picojson::object>()["points"].get<picojson::value::array>();
    for (picojson::value& val_array : arrays_array) {
        picojson::value::array point_array = val_array.get<picojson::value::array>();
        for (picojson::value& point : point_array) {
            points[i][j] = (int)point.get<double>();
            cout << points[i][j] << " ";
            j++;
        }
        cout << endl;
        i++;
        j = 0;
    }
    cout << endl;
    
    //tiled[][], turn
    field_update(fld_info);

    //agent, orange, blue, enemyteamID
    agents_update(fld_info);
    enemyteamID = blue.teamID;
    
    //ID変換の処理を消去
    
    exist_update();
    
    //ログをとるのを消去
}

void all_turns_update() {
    picojson::value fld_info = get_field_info();
    

    field_update(fld_info);
    agents_update(fld_info);
    //actions_updateを消去
    
    //ID変換の処理を消去

    exist_update();
    
    //ログをとるのを消去
}

void send_to_server() {
    picojson::value sendData = actions_json_make();
    send_json_to_server(sendData);
}

//送信用のjsonデータをas.tmp_actions_logから作成
picojson::value actions_json_make() {
    picojson::value::array agent_data_list;
    
    rep(i,agent_num) {
        picojson::object agent_data;
        if (orange.agents[i].next_command.empty()) {
            agent_data.insert(make_pair("dy", picojson::value((double)0)));
            agent_data.insert(make_pair("dx", picojson::value((double)0)));
            agent_data.insert(make_pair("type", picojson::value("stay")));
            agent_data.insert(make_pair("agentID", picojson::value((double)orange.agents[i].agentID)));
        } else {
            agent_data.insert(make_pair("dy", picojson::value((double)(orange.agents[i].next_y - orange.agents[i].y))));
            agent_data.insert(make_pair("dx", picojson::value((double)(orange.agents[i].next_x - orange.agents[i].x))));
            agent_data.insert(make_pair("type", picojson::value(orange.agents[i].next_command)));
            agent_data.insert(make_pair("agentID", picojson::value((double)orange.agents[i].agentID)));
        }
        agent_data_list.push_back(picojson::value(agent_data));
    }
    
    picojson::object actions_data;
    actions_data.insert(make_pair("actions", picojson::value(agent_data_list)));
    
    return picojson::value(actions_data);
}

void send_json_to_server(picojson::value data) {
    cout << "sending json to server" << endl;
    FILE* pp;
    FILE* fp;
    fp = fopen("send.json", "w");
    string s = data.serialize();
    fprintf(fp, "%s", s.c_str());
    fclose(fp);
    string command = "curl -H \"Authorization: " + token + "\" -H \"Content-Type: application/json\" -X POST http://localhost:" + port_num + "/matches/" + matchID + "/action -d @send.json";
    if ((pp = popen(command.c_str(), "r")) == NULL) {
        cout << "send command is failed" << endl;
    }
    char buf[204800];
    fgets(buf, sizeof(buf), pp);
    pclose(pp);
    printf("result_message : %s\n", buf);
}

void exist_update() {
    for (int i = 0; i < height; i++) {
        for (int j = 0; j < width; j++) {
            agent_exist[i][j] = 0;
        }
    }
    for (int i = 0; i < agent_num; i++) {
        agent_exist[blue.agents[i].y][blue.agents[i].x] = blue.teamID;
        agent_exist[orange.agents[i].y][orange.agents[i].x] = orange.teamID;
    }
    cout << "agent_exist[][]" << endl;
    for (int i = 0; i < height; i++) {
        for (int j = 0; j < width; j++) {
            cout << agent_exist[i][j] << " ";
        }
        cout << endl;
    }
    cout << endl;
}

//turn = max_turn-turnをしてこちら側の減るターンの仕様に合わせる
void field_update(picojson::value fld_info) {
    cout << "filed_update------------------------------------" << endl;
    turn = max_turn - (int)(fld_info.get<picojson::object>()["turn"].get<double>());
    cout << "turn : " << turn << endl;
    int i = 0, j = 0;
    cout << "tiled_server[][]" << endl;
    picojson::value::array arrays_array = fld_info.get<picojson::object>()["tiled"].get<picojson::value::array>();
    for (picojson::value& val_array : arrays_array) {
        picojson::value::array tile_array = val_array.get<picojson::value::array>();
        for (picojson::value& tile : tile_array) {
            tiled[i][j] = (int)tile.get<double>();
            cout << tiled[i][j] << " ";
            j++;
        }
        cout << endl;
        i++;
        j = 0;
    }
    cout << endl;
}

void agents_update(picojson::value fld_info) {
    //エージェント情報の更新
    int i, j;
    team_info team[2];
    picojson::value::array teams_array = fld_info.get<picojson::object>()["teams"].get<picojson::value::array>();
    for (i = 0; i < 2; i++) {
        picojson::object team_obj = teams_array[i].get<picojson::object>();
        team[i].teamID = (int)(team_obj["teamID"].get<double>());
        team[i].areaPoint = (int)(team_obj["areaPoint"].get<double>());
        team[i].tilePoint = (int)(team_obj["tilePoint"].get<double>());
        picojson::value::array team_agents_array = team_obj["agents"].get<picojson::value::array>();
        j = 0;
        for (picojson::value& team_agents_val : team_agents_array) {
            picojson::object team_agents_obj = team_agents_val.get<picojson::object>();
            team[i].agents[j].agentID = (int)(team_agents_obj["agentID"].get<double>());
            //x,y逆で0-indexにする
            team[i].agents[j].y = (int)(team_agents_obj["y"].get<double>()) - 1;
            team[i].agents[j].x = (int)(team_agents_obj["x"].get<double>()) - 1;
            j++;
        }
    }

    //agent
    agent_num = j;

    //orange, blue
    for (i = 0; i < 2; i++) {
        if (team[i].teamID == myteamID) {
            orange = team[i];
        }
        else {
            blue = team[i];
        }
    }
}


int get_time_milli() {
    timespec ts;
    if (timespec_get(&ts, TIME_UTC) == 0) {
        cout << "get_time_milli failed" << endl;
        return 1;
    }
    int now = (int)ts.tv_sec * 1000 + (int)ts.tv_nsec / 1000000;

    return now;

}

picojson::value get_match_info() {
    cout << "getting match info" << endl;
    FILE* pp;
    string command_to_get_info = "curl -H \"Authorization: " + token + "\" http://localhost:" + port_num + "/matches";
    cout << command_to_get_info << endl;
    if ((pp = popen(command_to_get_info.c_str(), "r")) == NULL) {
        cout << "get match is failed" << endl;
    }
    char buf[204800];
    fgets(buf, sizeof(buf), pp);
    pclose(pp);

    picojson::value val;
    string str(buf);
    cout << str << endl;
    string err = picojson::parse(val, str);
    if (!err.empty()) {
        cerr << err << endl;
    }

    return val;
}

picojson::value get_field_info() {
    cout << "getting field info" << endl;
    //コマンドを送信し、結果をpicojson::value型の結果(パースされ扱いやすくなったやつ)で返す
    FILE* pp;
    string command_to_get_info = "curl -H \"Authorization: " + token + "\" http://localhost:" + port_num + "/matches/" + matchID;
    if ((pp = popen(command_to_get_info.c_str(), "r")) == NULL) {
        cout << "get command is failed" << endl;
    }
    char buf[204800];
    fgets(buf, sizeof(buf), pp);
    pclose(pp);

    string str(buf);
    picojson::value val;
    string err = picojson::parse(val, str);
    if (!err.empty()) {
        cerr << err << endl;
    }
    
    return val;
}
