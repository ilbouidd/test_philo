/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: imeziane <imeziane@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/02 16:28:58 by imeziane          #+#    #+#             */
/*   Updated: 2026/09/10 12:44:38 by imeziane         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "philo.h"

void	init_data(t_all *data)
{
	data->nb_philo = (int)ft_atoi_philo(data->av[1]);
	data->time_to_die = ft_atoi_philo(data->av[2]);
	data->time_to_eat = ft_atoi_philo(data->av[3]);
	data->time_to_sleep = ft_atoi_philo(data->av[4]);
	data->time_must_eat = 0;
	data->start_time = 0;
	data->end = 0;
	data->start = 0;
	data->forks = NULL;
	data->philo = NULL;
	if (data->ac == 6)
	{
		data->time_must_eat = (int)ft_atoi_philo(data->av[5]);
		data->meal_limit = data->time_must_eat;
	}
	else
		data->meal_limit = -1;
}

int	init_mutexes(t_all *data)
{
	int	i;

	if (pthread_mutex_init(&data->print_mutex, NULL) != 0)
		return (1);
	if (pthread_mutex_init(&data->end_mutex, NULL) != 0)
	{
		pthread_mutex_destroy(&data->print_mutex);
		return (1);
	}
	if (pthread_mutex_init(&data->start_mutex, NULL) != 0)
	{
		pthread_mutex_destroy(&data->print_mutex);
		pthread_mutex_destroy(&data->end_mutex);
		return (1);
	}
	data->forks = malloc(sizeof(pthread_mutex_t) * data->nb_philo);
	if (!data->forks)
	{
		pthread_mutex_destroy(&data->print_mutex);
		pthread_mutex_destroy(&data->end_mutex);
		pthread_mutex_destroy(&data->start_mutex);
		return (1);
	}
	i = 0;
	while (i < data->nb_philo)
	{
		if (pthread_mutex_init(&data->forks[i], NULL) != 0)
		{
			while (--i >= 0)
				pthread_mutex_destroy(&data->forks[i]);
			free(data->forks);
			pthread_mutex_destroy(&data->print_mutex);
			pthread_mutex_destroy(&data->end_mutex);
			pthread_mutex_destroy(&data->start_mutex);
			return (1);
		}
		i++;
	}
	return (0);
}

int	init_philos(t_all *data)
{
	int	i;

	data->philo = malloc(sizeof(t_philo) * data->nb_philo);
	if (!data->philo)
		return (1);
	i = 0;
	while (i < data->nb_philo)
	{
		data->philo[i].id = i + 1;
		data->philo[i].meals = 0;
		data->philo[i].finished = 0;
		data->philo[i].last_meal = 0;
		data->philo[i].data = data;
		data->philo[i].left_fork = &data->forks[i];
		data->philo[i].right_fork
			= &data->forks[(i + 1) % data->nb_philo];
		if (pthread_mutex_init(&data->philo[i].meal_mutex, NULL) != 0)
		{
			while (--i >= 0)
				pthread_mutex_destroy(&data->philo[i].meal_mutex);
			free(data->philo);
			return (1);
		}
		i++;
	}
	return (0);
}

void	destroy_data(t_all *data)
{
	int	i;

	i = 0;
	while (i < data->nb_philo)
	{
		pthread_mutex_destroy(&data->philo[i].meal_mutex);
		i++;
	}
	free(data->philo);
	i = 0;
	while (i < data->nb_philo)
	{
		pthread_mutex_destroy(&data->forks[i]);
		i++;
	}
	free(data->forks);
	pthread_mutex_destroy(&data->print_mutex);
	pthread_mutex_destroy(&data->end_mutex);
	pthread_mutex_destroy(&data->start_mutex);
}