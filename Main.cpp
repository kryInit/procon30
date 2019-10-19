#include "Main.hpp"
#include "others.hpp"

using namespace std;

int height,width,agent_num,turn,points[20][20],tiled[20][20],agent_exist[20][20],border=-16,tile_size=50;
team_info blue,orange;

void input_action() {
    //ここでいろいろと入力を受け取ると良い
    input_exgreedy_action(blue, blue.teamID, orange.teamID, 6);
    input_random_action(orange);
}

void Main() {
    Console.open();
    Graphics::SetTargetFrameRateHz(30);
    initialize_XorShift(time(NULL));
    initialize_for_local();
    input_action();
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
        if (KeyEnter.down()) {
            if (turn > 0) {
                log_all_values();
                local_transition();
            }
            if (turn > 0) input_action();
        }
        
        ClearPrint();
    }
}



