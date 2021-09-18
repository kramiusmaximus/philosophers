#include "../includes/sauce.h"

extern t_params params;
pthread_mutex_t *forks, check_forks_mutex, pick_up_forks_mutex, check_death_mutex;

void put_down_forks(int id) {
	pthread_mutex_lock(&check_forks_mutex);
	params.forks[id] = 1;
	params.forks[(id + 1) % params.number_of_philosophers] = 1;
	pthread_mutex_unlock(&check_forks_mutex);
	unlock_forks(id);
}

void pick_up_forks(int id, long wake_up_time) {
	lock_forks(id);
	printf("[%-5ld ms] Philosopher %d: has picked up а fork\n", curr_time_mill() - wake_up_time, id + 1);
	params.forks[(id) % params.number_of_philosophers] = 0;
	printf("[%-5ld ms] Philosopher %d: has picked up а left fork\n", curr_time_mill() - wake_up_time, id + 1);
	params.forks[(id + 1) % params.number_of_philosophers] = 0;
}

int picked_up_forks(int id, long wake_up_time) {
	if (forks_available(id)) {
		pick_up_forks(id, wake_up_time);
		pthread_mutex_unlock(&pick_up_forks_mutex);
		return (1);
	}
	pthread_mutex_unlock(&pick_up_forks_mutex);
	//printf("philosopher %d says forks unavailable\n", id);
	return (0);
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
}

int die(long tod, int id) {
	printf("[%-5ld ms] Philosopher %d: has died\n", tod, id + 1);
	pthread_mutex_lock(&forks[params.number_of_philosophers]);
	params.philosopher_owned = 1;
	pthread_mutex_unlock(&forks[params.number_of_philosophers]);
	return (1);
}
