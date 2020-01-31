#include "Main.hpp"
#include "others.hpp"

using namespace std;

int height,width,agent_num,turn,points[20][20],tiled[20][20],agent_exist[20][20],border=-16,tile_size=50;
team_info blue,orange;

int startAtUnixTime, startTimeMilli, nowTimeMilli, nextTurnTimeMilli, intervalMillis, turnMillis, startedAtUnixTime, max_turn;
bool is_played, is_sent;

void input_action() {
    //ここでいろいろと入力を受け取ると良い
    input_exgreedy_action(orange, orange.teamID, blue.teamID, 6);
//    input_random_action(orange);
}

void Main() {
    bool is_server = true;
    Console.open();
    Graphics::SetTargetFrameRateHz(30);
    initialize_XorShift(time(NULL));
    if (is_server) initialize_for_sever();
    else initialize_for_local();
    border = select_best_border();
    input_action();
    
    Window::Resize(tile_size*width+200,tile_size*height+20);
     
    while (System::Update())
    {
        print_various();
        
        if (KeyUp.down()) border = min(17LL, border+1);
        if (KeyDown.down()) border = max(-17LL, border-1);
        if ((KeyCommand.pressed() || KeyControl.pressed()) && KeyZ.down()) rollback();
        if ((KeyCommand.pressed() || KeyControl.pressed()) && KeyR.down()) {
            initialize_for_local();
            input_action();
        }
        if (turn > 0) print_arrow();
        
        if (is_server) {
            if (turn > 0) {
                nowTimeMilli = get_time_milli();
                if (!is_played) {
                    cout << "__play start__________________________________" << endl;
                    input_action();
                    cout << "__play end____________________________________" << endl;
                    is_played = true;
                }
                if (is_played && !is_sent && nowTimeMilli >= nextTurnTimeMilli - intervalMillis - 800) {
                    cout << "senting..." << endl;
                    send_to_server();
                    cout << "server sent!" << endl;
                    is_sent = true;
                }
                if (is_played && is_sent && nowTimeMilli > nextTurnTimeMilli) {
                    cout << "get! : " << nowTimeMilli << endl;
                    all_turns_update();
                    cout << startTimeMilli + (turnMillis + intervalMillis) * (max_turn - turn + 1) - nextTurnTimeMilli << endl;
                    cout << max_turn << " " << turn << " " << endl;
                    cout << endl;
                    nextTurnTimeMilli = startTimeMilli + (turnMillis + intervalMillis) * (max_turn - turn + 1);
                    is_played = false;
                    is_sent = false;
                }
            }
        }
        else {
            if (KeyEnter.down()) {
                if (turn > 0) {
                    log_all_values();
                    local_transition();
                }
                if (turn > 0) input_action();
            }
        }
        
        
        ClearPrint();
    }
}



