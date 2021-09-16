#include "../includes/sauce.h"

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

int	check_death_condition(long last_food) {
	long curr_time = curr_time_mill();

	if (curr_time - last_food >= params.time_to_die)
		return (1);
	return (0);
}

int die(long tod, int id) {
	printf("[%-5ld ms] Philosopher %d: has died\n", tod, id + 1);
	pthread_mutex_lock(&mutexes[params.number_of_philosophers]);
	params.philosopher_owned = 1;
	pthread_mutex_unlock(&mutexes[params.number_of_philosophers]);
	return (1);
}

int check_plague_condition() {
	int ret;

	ret = 0;
	pthread_mutex_lock(&mutexes[params.number_of_philosophers]);
	if (params.philosopher_owned)
		ret = 1;
	pthread_mutex_unlock(&mutexes[params.number_of_philosophers]);
	return (ret);
}

int check_stopping_conditions(long wake_up_time, long last_meal_time, int id) {
	long curr = curr_time_mill();
	if (check_plague_condition())
		return (1);
	if (check_death_condition(last_meal_time))
		return (die(curr - wake_up_time, id));
	return (0);
}

void annex_forks(int id) {
	int left_index = id - 1 < 0 ? params.number_of_philosophers - 1 : id - 1;

	pthread_mutex_lock(&mutexes[id]);
	pthread_mutex_lock(&mutexes[left_index]);
}

void liberate_forks(int id) {
	int left_index = id - 1 < 0 ? params.number_of_philosophers - 1 : id - 1;

	pthread_mutex_unlock(&mutexes[id]);
	pthread_mutex_unlock(&mutexes[left_index]);
}

int pick_up_forks(int id, long wake_up_time, long last_meal_time) {
	int left_index = id - 1 < 0 ? params.number_of_philosophers - 1 : id - 1;

	if (check_stopping_conditions(wake_up_time, last_meal_time, id))
		return (1);
	if (params.forks[id]) {
		params.forks[id] = 0;
		printf("[%-5ld ms] Philosopher %d: has picked up his right fork\n", curr_time_mill() - wake_up_time, id + 1);
	}
	if (params.forks[left_index]) {
		params.forks[left_index] = 0;
		printf("[%-5ld ms] Philosopher %d: has picked up his left fork\n", curr_time_mill() - wake_up_time, id + 1);
	}
	return (0);
}

void put_down_forks(int id) {
	int left_index = id - 1 < 0 ? params.number_of_philosophers - 1 : id - 1;

	params.forks[id] = 1;
	params.forks[left_index];
}

int finish_eating(int id, long wake_up_time) {
	printf("[%-5ld ms] Philosopher %d: is full....\n", curr_time_mill() - wake_up_time, id + 1);
	return (1);
}

int eat(int id, long wake_up_time, int *times_eaten, long last_meal_time) {
	*times_eaten += 1;

	if (check_stopping_conditions(wake_up_time, last_meal_time, id))
		return (1);
	printf("[%-5ld ms] Philosopher %d: is eating\n", curr_time_mill() - wake_up_time, id + 1);
	if ((usleep(1000 * params.time_to_eat)))
		error_exit("usleep failed", 1);
	if (params.optional_argument_specified && *times_eaten >= params.number_of_times_each_philosopher_must_eat) {
		return (finish_eating(id, wake_up_time));
	}
	return (0);
}

void shleep(int id, long wake_up_time, long last_meal_time) {
	if (check_stopping_conditions(wake_up_time, last_meal_time, id))
		pthread_exit(NULL);
	printf("[%-5ld ms] Philosopher %d: is sleeping\n", curr_time_mill() - wake_up_time, id + 1);
	if ((usleep(1000 * params.time_to_sleep)))
		error_exit("usleep failed", 1);
}

void think(int id, long wake_up_time, long last_meal_time) {
	if (check_stopping_conditions(wake_up_time, last_meal_time, id))
		pthread_exit(NULL);
	printf("[%-5ld ms] Philosopher %d: is thinking\n", curr_time_mill() - wake_up_time, id + 1);
	if ((usleep(1000 * params.time_to_sleep)))
		error_exit("usleep failed", 1);
}

int wakeup_philosopher(const int *id_ref) {
	int id = *id_ref;
	long wake_up_time, last_meal_time;
	int times_eaten = 0;

	wake_up_time = curr_time_mill();
	last_meal_time = wake_up_time;
	while (1) {
		annex_forks(id);
		if (pick_up_forks(id, wake_up_time, last_meal_time)) {
			liberate_forks(id);
			break ;
		}
		last_meal_time = curr_time_mill();
		if (eat(id, wake_up_time, &times_eaten, last_meal_time)) {
			liberate_forks(id);
			break ;
		}
		put_down_forks(id);
		liberate_forks(id);
		shleep(id, wake_up_time, last_meal_time);
		think(id, wake_up_time, last_meal_time);
	}
	pthread_exit(0);
}

int begin_feast(t_params *params) {
    pthread_t		*philosophers[params->number_of_philosophers + 1];
    int				args[params->number_of_philosophers];
    mutexes = malloc(sizeof(pthread_mutex_t) * params->number_of_philosophers);
    int				i;

    for (int j = 0; j < params->number_of_philosophers + 1; j++) {
		if (pthread_mutex_init(&(mutexes[j]), NULL))
			error_exit("Mutex initiation failed...", 1);
    }
    i = 0;
    while (i < params->number_of_philosophers) {
    	args[i] = i;
    	int rt = pthread_create( &philosophers[i], NULL, &wakeup_philosopher, &args[i]);
		pthread_detach(&philosophers[i]);
		if (rt)
			error_exit("Thread creation failed", 1);
    	i++;
    }

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