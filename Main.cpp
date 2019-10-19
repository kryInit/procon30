#include "Main.hpp"
#include "others.hpp"

using namespace std;

int height,width,agent_num,turn,points[20][20],tiled[20][20],agent_exist[20][20],border=-16,tile_size=50;
team_info blue,orange;

void Main() {
    Console.open();
    Graphics::SetTargetFrameRateHz(30);
    initialize_for_local();
    input_exgreedy_action(blue, blue.teamID, orange.teamID, 6);
    input_random_action(orange);
    while (System::Update())
    {
        if ((KeyCommand.pressed() || KeyControl.pressed()) && KeyR.down()) {
            initialize_for_local();
            input_exgreedy_action(blue, blue.teamID, orange.teamID, 6);
            input_random_action(orange);
        }
        if ((KeyCommand.pressed() || KeyControl.pressed()) && KeyZ.down()) rollback();
        if (KeyUp.down()) border = min(17LL, border+1);
        if (KeyDown.down()) border = max(-17LL, border-1);
        
        print_various();
        
        if (turn > 0) print_arrow();
        
        if (KeyEnter.down()) {
            
            if (turn > 0) {
                log_all_values();
                local_transition();
            }
            if (turn > 0) {
                //ここで行動を入力すると良さそう
                input_exgreedy_action(blue, blue.teamID, orange.teamID, 6);
                input_random_action(orange);
            }
        }
        
        ClearPrint();
    }
}



