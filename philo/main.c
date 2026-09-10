/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: imeziane <imeziane@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/02 16:20:43 by imeziane          #+#    #+#             */
/*   Updated: 2026/09/10 12:49:17 by imeziane         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

static int	check_death(t_all *data)
{
	int		i;
	int		finished;
	long	elapsed;

	i = 0;
	while (i < data->nb_philo)
	{
		pthread_mutex_lock(&data->philo[i].meal_mutex);
		finished = data->philo[i].finished;
		elapsed = elapsed_since(data->philo[i].last_meal);
		pthread_mutex_unlock(&data->philo[i].meal_mutex);
		if (!finished && elapsed >= data->time_to_die)
		{
			pthread_mutex_lock(&data->end_mutex);
			if (!data->end)
			{
				pthread_mutex_lock(&data->print_mutex);
				printf("%ld %d died\n",
					elapsed_since(data->start_time),
					data->philo[i].id);
				pthread_mutex_unlock(&data->print_mutex);
				data->end = 1;
			}
			pthread_mutex_unlock(&data->end_mutex);
			return (1);
		}
		i++;
	}
	return (0);
}

static int	all_ate_enough(t_all *data)
{
	int	i;
	int	finished;

	if (data->meal_limit == -1)
		return (0);
	i = 0;
	while (i < data->nb_philo)
	{
		pthread_mutex_lock(&data->philo[i].meal_mutex);
		finished = data->philo[i].finished;
		pthread_mutex_unlock(&data->philo[i].meal_mutex);
		if (!finished)
			return (0);
		i++;
	}
	return (1);
}

static void	*monitor(void *arg)
{
	t_all	*data;

	data = (t_all *)arg;
	while (!sim_should_stop(data->philo))
	{
		if (all_ate_enough(data))
		{
			stop_simulation(data);
			break ;
		}
		if (check_death(data))
			break ;
		usleep(500);
	}
	return (NULL);
}

int	main(int argc, char *argv[])
{
	t_all		data;
	int			i;
	pthread_t	monitor_thread;

	if (argc != 5 && argc != 6)
		return (printf("Error: Bad number of arguments\n"), 1);
	data.ac = argc;
	data.av = argv;
	init_data(&data);
	if (parse_args(&data))
		return (1);
	if (init_mutexes(&data) != 0)
		return (printf("Error: mutex init failed\n"), 1);
	if (init_philos(&data) != 0)
	{
		destroy_data(&data);
		return (printf("Error: philo init failed\n"), 1);
	}
	data.start_time = get_time_ms();
	i = 0;
	while (i < data.nb_philo)
	{
		data.philo[i].last_meal = data.start_time;
		if (pthread_create(&data.philo[i].thread, NULL, routine, &data.philo[i])
			!= 0)
		{
			stop_simulation(&data);
			break ;
		}
		i++;
	}
	pthread_mutex_lock(&data.start_mutex);
	data.start = 1;
	pthread_mutex_unlock(&data.start_mutex);
	pthread_create(&monitor_thread, NULL, monitor, &data);
	i = 0;
	while (i < data.nb_philo)
	{
		pthread_join(data.philo[i].thread, NULL);
		i++;
	}
	pthread_join(monitor_thread, NULL);
	destroy_data(&data);
	return (0);
}