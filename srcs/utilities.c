#include "../includes/sauce.h"

extern t_params params;
pthread_mutex_t *forks;

void error_exit(char *err_msg, int exit_n) {
	printf("%s\n", err_msg);
	exit(exit_n);
}

long curr_time_mill() {
	struct timeval curr_time;
	gettimeofday(&curr_time, NULL);
	return (curr_time.tv_sec * 1000 + curr_time.tv_usec / 1000);
}

int parse_params(char **vargs, t_params *params) {
	params->number_of_philosophers = atoi(vargs[1]);
	params->time_to_die = atoi(vargs[2]);
	params->time_to_eat = atoi(vargs[3]);
	params->time_to_sleep = atoi(vargs[4]);
	params->philosopher_owned = 0;

	if (!(params->forks = fork_init()))
		return (1);
	if (vargs[5]) {
		params->optional_argument_specified = 1;
		params->number_of_times_each_philosopher_must_eat = atoi(vargs[5]);
	}
	else
		params->optional_argument_specified = 0;
	return (0);
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

void lock_forks(int id) {
	pthread_mutex_lock(&forks[id]);
	pthread_mutex_lock(&forks[(id + 1) % params.number_of_philosophers]);
}

void unlock_forks(int id) {
	pthread_mutex_unlock(&forks[id]);
	pthread_mutex_unlock(&forks[(id + 1) % params.number_of_philosophers]);
}

int *fork_init() {
	int *forks;
	int i;

	if (!(forks = malloc(sizeof(int) * params.number_of_philosophers)))
		return (NULL);
	i = 0;
	while (i < params.number_of_philosophers) {
		forks[i] = 1;
		i++;
	}
	return (forks);
}