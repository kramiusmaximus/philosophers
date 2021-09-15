#include "sauce.h"

t_params    params;
pthread_mutex_t *mutexes;

void error_exit(char *err_msg, int exit_n) {
    printf("%s\n", err_msg);
    exit(exit_n);
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

int parse_params(char **vargs, t_params *params) {
    params->number_of_philosophers = atoi(vargs[1]);
    params->time_to_die = atoi(vargs[2]);
    params->time_to_eat = atoi(vargs[3]);
    params->time_to_sleep = atoi(vargs[4]);
    params->philosopher_owned = 0;

    //initiate forks
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

int prepare_feast(t_params *params) {
    int *forks;

    if (!forks == malloc(sizeof(int) * params->number_of_philosophers))
        return (1);
    return (0);
}

long curr_time_mill() {
	struct timeval curr_time;
	gettimeofday(&curr_time, NULL);
	return (curr_time.tv_sec * 1000 + curr_time.tv_usec / 1000);
}

int	check_death_condition(long wake_up_time) {
	long curr_time = curr_time_mill();

	if (curr_time - wake_up_time >= params.time_to_die)
		return (1);
	return (0);
}

int wakeup_philosopher(const int *id_ref) {
	int id = *id_ref;
	int left_index = id - 1 < 0 ? params.number_of_philosophers - 1 : id - 1;
	long wake_up_time, curr_time, last_meal_time;
	int right_fork_picked_up = 0;
	int left_fork_picked_up = 0;

	wake_up_time = curr_time_mill();
	while (!params.philosopher_owned) {// not dead
		pthread_mutex_lock(&mutexes[id]);
		pthread_mutex_lock(&mutexes[left_index]);
		if (params.forks[id]) {
			// pick up fork
			params.forks[id] = 0;
			curr_time = curr_time_mill();
			pthread_mutex_lock(&mutexes[params.number_of_philosophers]);
			if (check_death_condition(wake_up_time)) {
				params.philosopher_owned = 1;
				printf("[%-5ld ms] Philosopher %d: has died\n", curr_time - wake_up_time, id + 1);
				break ;
			}
			pthread_mutex_unlock(&mutexes[params.number_of_philosophers]);
			printf("[%-5ld ms] Philosopher %d: has picked up his right fork\n", curr_time - wake_up_time, id + 1);
		}
		if (params.forks[left_index])
		{
			// pick up fork
			params.forks[left_index] = 0;
			curr_time = curr_time_mill();
			printf("[%-5ld ms] Philosopher %d: has picked up his left fork\n", curr_time - wake_up_time, id + 1);
		}
		// eat
		last_meal_time = curr_time_mill();
		printf("[%-5ld ms] Philosopher %d: is eating\n", last_meal_time - wake_up_time, id + 1);
		if ((usleep(1000 * params.time_to_eat)))
			error_exit("usleep failed", 1);
		params.forks[id] = 1;
		params.forks[left_index] = 1;
		pthread_mutex_unlock(&mutexes[id]);
		pthread_mutex_unlock(&mutexes[left_index]);

		// sleep
		curr_time = curr_time_mill();
		printf("[%-5ld ms] Philosopher %d: is sleeping\n", curr_time - wake_up_time, id + 1);
		if ((usleep(1000 * params.time_to_sleep)))
			error_exit("usleep failed", 1);
		// think
		curr_time = curr_time_mill();
		printf("[%-5ld ms] Philosopher %d: is thinking\n", curr_time - wake_up_time, id + 1);
		if ((usleep(1000 * params.time_to_sleep)))
			error_exit("usleep failed", 1);
	}
	return (0);
}

int begin_feast(t_params *params) {
    pthread_t		*philosophers[params->number_of_philosophers + 1];
    int				args[params->number_of_philosophers];
    mutexes = malloc(sizeof(pthread_mutex_t) * params->number_of_philosophers);
    int				i;

    for (int j = 0; j < params->number_of_philosophers + 1; j++) {
		if (pthread_mutex_init(&(mutexes[j]), NULL))
			error_exit("Mutex initiation failed...", 1); //error
    }
    // start threads
    i = 0;
    while (i < params->number_of_philosophers) {
    	args[i] = i;
    	int rt = pthread_create( &philosophers[i], NULL, &wakeup_philosopher, &args[i]);
		pthread_detach(&philosophers[i]);
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