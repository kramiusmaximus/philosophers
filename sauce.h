#ifndef PHILOSOPHERS_SAUCE_H
#define PHILOSOPHERS_SAUCE_H

typedef struct  s_params {
    int         number_of_philosophers;
    float       time_to_die;
    float       time_to_eat;
    float       time_to_sleep;
    int         optional_argument_specified;
    int         number_of_times_each_philosopher_must_eat;
    int         forks[];
}               t_params;

#endif
