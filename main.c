#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include "sauce.h"

void error_exit(char *err_msg, int exit_n) {
    printf("%s\n", err_msg);
    exit(exit_n);
}

int give_birth_to_philosopher() {

}

int valid_arguments(t_params *params) {
    if (params->number_of_philosophers < 1 &&
        params->time_to_die < 0 &&
        params->time_to_eat < 0 &&
        params->time_to_sleep < 0)
        return (0);
    if (params->optional_argument_specified &&
        params->number_of_times_each_philosopher_must_eat < 0)
        return (0);
    return (1);
}

int parse_params(char **vargs, t_params *params) {
    params->number_of_philosophers = atoi(vargs[1]);
    params->time_to_die = atof(vargs[2]);
    params->time_to_eat = atof(vargs[3]);
    params->time_to_sleep = atof(vargs[4]);
    if (vargs[5]) {
        params->optional_argument_specified = 1;
        params->number_of_times_each_philosopher_must_eat = atoi(vargs[5]);
    }
    else
        params->optional_argument_specified = 0;
}

int main(int nargs, char **vargs) {
    t_params    params;
    if (nargs < 5)
        error_exit("Incorrect number of arguments entered. Enter 4 arguments please, 5 with optional argument.", 1)
    if (parse_params(vargs, &params))
        error_exit("An error occured whilst allocating heap memory with malloc... we're soorryyyyyy")
    if (!valid_arguments(&params))
        error_exit("One or more arguments entered is invalid", 1)

}