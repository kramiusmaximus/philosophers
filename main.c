#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include "sauce.h"

t_params    params;

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
    return (0);
}

int prepare_feast(t_params *params) {
    int *forks;

    if (!forks == malloc(sizeof(int) * params->number_of_philosophers))
        return (1);
    return (0);
}

int wakeup_philosopher(const int *id_ref) {
	int id = *id_ref;

	// wake up
	printf("Philosopher %d: I woke up!!\n", id + 1);

	if (1) {// not dead
		// think
		printf("Philosopher %d: I'm now thinking very hard about the human condition and the many contradictions of man\n", id + 1);

		//lock mutex here

		// eat
		printf("Philosopher %d: I'm gonna eat now. Zeus!!!! These persian kid's kidney's taste phenomenal!\n", id + 1);

		//unlock mutex hear

		// sleep
		printf("Philosopher %d: ...zZZZZzzZZZZZzzz...\n", id + 1);
	}
	return (0);
}

int begin_feast(t_params *params) {
    pthread_mutex_t mutexes[params->number_of_philosophers / 2 + (params->number_of_philosophers % 2 == 1 ? 1 : 0)];
    pthread_t		*philosophers[params->number_of_philosophers];
    int				args[params->number_of_philosophers];
    int				i;

    // start threads
    i = 0;
    while (i < params->number_of_philosophers) {
    	args[i] = i;
    	int rt = pthread_create( &philosophers[i], NULL, &wakeup_philosopher, &args[i]);

		if (rt)
			error_exit("Thread creation failed", 1);
    	i++;  // do I need to manually delete threads in case of an error??
    }

    // wait on threads
    i = 0;
    while (i < params->number_of_philosophers) {
    	pthread_join(philosophers[i], NULL);
    	i++;
    }
    return (0);
}

int main(int nargs, char **vargs) {
    if (nargs < 5)
        error_exit("Incorrect number of arguments entered. Enter 4 arguments please, 5 with optional argument.", 1);
    if (parse_params(vargs, &params))
        error_exit("An error occured whilst allocating heap memory with malloc... we're soorryyyyyy", 1);
    if (!valid_arguments(&params))
        error_exit("One or more arguments entered is invalid", 1);
    if (prepare_feast(&params))
        error_exit("Malloc failed!", 1);
    begin_feast(&params);
}