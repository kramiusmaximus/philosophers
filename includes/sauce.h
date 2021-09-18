#ifndef PHILOSOPHERS_SAUCE_H
#define PHILOSOPHERS_SAUCE_H
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <string.h>
#include <sys/time.h>
#include <unistd.h>

typedef struct  s_params {
    int         number_of_philosophers;
    int       time_to_die;
    int       time_to_eat;
    int       time_to_sleep;
    int         optional_argument_specified;
    int         number_of_times_each_philosopher_must_eat;
    int         *forks;
    int			philosopher_owned;
}               t_params;

int set_the_table(t_params *params);
int wakeup_philosopher(const int *id_ref);
int begin_feast(t_params *params);
int	check_death_condition(long last_food);
int check_plague_condition();
int check_stopping_conditions(long wake_up_time, long last_meal_time, int id);
void error_exit(char *err_msg, int exit_n);
long curr_time_mill();
int parse_params(char **vargs, t_params *params);
int valid_arguments(t_params *params);
void lock_forks(int id);
void unlock_forks(int id);
int *fork_init();
void put_down_forks(int id);
int picked_up_forks(int id, long wake_up_time);
int finish_eating(int id, long wake_up_time);
int eat(int id, long wake_up_time, int *times_eaten, long last_meal_time);
void shleep(int id, long wake_up_time, long last_meal_time);
void think(int id, long wake_up_time, long last_meal_time);
int die(long tod, int id);
int forks_available(int id);
void pick_up_forks(int id, long wake_up_time);
int milli_sleep(int milli_sec);

#endif
