#ifndef include_all_header_hpp
#define include_all_header_hpp

#include "others.hpp"


void initialize_for_local();


void print_various();
void print_arrow();

void local_transition();

void log_all_values();
void rollback();

void input_random_action(team_info& team);
void input_exgreedy_action(team_info& team, int my_color, int enemy_color, int N);

void initialize_for_sever();
void all_turns_update();
void send_to_server();
int get_time_milli();


#endif /* include_all_header_hpp */
