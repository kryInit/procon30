#ifndef include_all_header_hpp
#define include_all_header_hpp

#include "others.hpp"

void initialize_for_local();
void print_various();
void print_arrow();

void input_random_action(team_info& team);
void input_exgreedy_action(team_info& team, int my_color, int enemy_color, int N);

void local_transition();

void log_all_values();
void rollback();


#endif /* include_all_header_hpp */
