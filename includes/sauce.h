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

#endif
