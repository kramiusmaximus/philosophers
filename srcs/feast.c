#include "../includes/sauce.h"
extern t_params params;
pthread_mutex_t *forks, check_forks_mutex, pick_up_forks_mutex, check_death_mutex;

int set_the_table(t_params *params) {
	if (!(params->forks = malloc(sizeof(int) * params->number_of_philosophers)))
		return (1);
	for (int i = 0; i < params->number_of_philosophers; i++) {
		params->forks[i] = 1;
	}
	return (0);
}

int wakeup_philosopher(const int *id_ref) {
	int id = *id_ref;
	long wake_up_time, last_meal_time;
	int times_eaten = 0;

	wake_up_time = curr_time_mill();
	last_meal_time = wake_up_time;
	while (!check_stopping_conditions(wake_up_time, last_meal_time, id)) {
		if (picked_up_forks(id, wake_up_time)) {
			last_meal_time = curr_time_mill();
			if (eat(id, wake_up_time, &times_eaten, last_meal_time)) {
				put_down_forks(id);
				break ;
			}
			put_down_forks(id);
			shleep(id, wake_up_time, last_meal_time);
			think(id, wake_up_time, last_meal_time);
		}
	}
	pthread_exit(0);
}

int begin_feast(t_params *params) {
	pthread_t		philosophers[params->number_of_philosophers];
	int				args[params->number_of_philosophers];
	forks = malloc(sizeof(pthread_mutex_t) * (params->number_of_philosophers + 2));
	int				i;

	for (int j = 0; j < params->number_of_philosophers; j++) {
		if (pthread_mutex_init(&forks[j], NULL))
			error_exit("Mutex initiation failed...", 1);
	}
	pthread_mutex_init(&check_forks_mutex, NULL);
	pthread_mutex_init(&pick_up_forks_mutex, NULL);
	pthread_mutex_init(&check_death_mutex, NULL);
	i = 0;
	while (i < params->number_of_philosophers) {
		args[i] = i;
		int rt = pthread_create(&philosophers[i], NULL, &wakeup_philosopher, &args[i]);
		//pthread_detach(philosophers[i]);
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

