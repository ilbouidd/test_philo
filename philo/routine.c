/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   routine.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: imeziane <imeziane@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/03 19:35:45 by imeziane          #+#    #+#             */
/*   Updated: 2026/09/10 13:50:36 by imeziane         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

static void	one_philo_routine(t_philo *philo)
{
	pthread_mutex_lock(philo->left_fork);
	print_state(philo, "has taken a fork");
	precise_sleep(philo->data->time_to_die, philo->data);
	pthread_mutex_unlock(philo->left_fork);
}

static int	philo_reached_limit(t_philo *philo)
{
	int	meals;

	pthread_mutex_lock(&philo->meal_mutex);
	meals = philo->meals;
	if (philo->data->meal_limit != -1
		&& meals >= philo->data->meal_limit)
		philo->finished = 1;
	pthread_mutex_unlock(&philo->meal_mutex);
	return (philo->finished);
}

static void	pick_forks(t_philo *philo)
{
	if (philo->data->nb_philo == 1)
	{
		pthread_mutex_lock(&philo->data->forks[0]);
		print_state(&philo->data->philo[0], "has taken a fork");
		precise_sleep(philo->data->time_to_die, philo->data);
		print_state(&philo->data->philo[0], "died");
		stop_simulation(philo->data);
		pthread_mutex_unlock(&philo->data->forks[0]);
		return ;
	}
	if (philo->left_fork < philo->right_fork)
	{
		pthread_mutex_lock(philo->left_fork);
		print_state(philo, "has taken a fork");
		pthread_mutex_lock(philo->right_fork);
		print_state(philo, "has taken a fork");
	}
	else
	{
		pthread_mutex_lock(philo->right_fork);
		print_state(philo, "has taken a fork");
		pthread_mutex_lock(philo->left_fork);
		print_state(philo, "has taken a fork");
	}
}

static void	wait_for_start(t_philo *philo)
{
	while (1)
	{
		pthread_mutex_lock(&philo->data->start_mutex);
		if (philo->data->start)
		{
			pthread_mutex_unlock(&philo->data->start_mutex);
			break ;
		}
		pthread_mutex_unlock(&philo->data->start_mutex);
		usleep(100);
	}
}

void	*routine(void *arg)
{
	t_philo	*philo;

	philo = (t_philo *)arg;
	wait_for_start(philo);
	if (philo->data->nb_philo == 1)
	{
		one_philo_routine(philo);
		return (NULL);
	}
	usleep((philo->id - 1) * (philo->data->time_to_eat
			+ philo->data->time_to_sleep) * 1000 / philo->data->nb_philo);
	while (!sim_should_stop(philo))
	{
		pick_forks(philo);
		eat_phase(philo);
		release_forks(philo);
		if (philo_reached_limit(philo))
			break ;
		sleep_and_think(philo);
	}
	return (NULL);
}

int	sim_should_stop(t_philo *philo)
{
	int	value;

	pthread_mutex_lock(&philo->data->end_mutex);
	value = philo->data->end;
	pthread_mutex_unlock(&philo->data->end_mutex);
	return (value);
}

void	stop_simulation(t_all *data)
{
	pthread_mutex_lock(&data->end_mutex);
	data->end = 1;
	pthread_mutex_unlock(&data->end_mutex);
}