#include "../includes/sauce.h"

extern t_params params;
pthread_mutex_t *forks, check_forks_mutex, pick_up_forks_mutex, check_death_mutex;

int forks_available(int id) {
	int ret;

	if (params.number_of_philosophers == 1)
		return (0);
	pthread_mutex_lock(&pick_up_forks_mutex);
	ret = params.forks[id] && params.forks[(id + 1) % params.number_of_philosophers] ? 1 : 0;
	return (ret);
}

int	check_death_condition(long last_food) {
	long curr_time = curr_time_mill();

	if (curr_time - last_food >= params.time_to_die)
		return (1);
	return (0);
}

int check_plague_condition() {
	int ret;

	ret = 0;
	pthread_mutex_lock(&check_death_mutex);
	if (params.philosopher_owned)
		ret = 1;
	pthread_mutex_unlock(&check_death_mutex);
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

