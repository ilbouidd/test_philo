/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   actions.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: imeziane <imeziane@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/09 19:19:56 by imeziane          #+#    #+#             */
/*   Updated: 2026/09/10 13:06:26 by imeziane         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void	eat_phase(t_philo *philo)
{
	pthread_mutex_lock(&philo->meal_mutex);
	philo->last_meal = get_time_ms();
	philo->meals++;
	pthread_mutex_unlock(&philo->meal_mutex);
	print_state(philo, "is eating");
	precise_sleep(philo->data->time_to_eat, philo->data);
}

void	release_forks(t_philo *philo)
{
	pthread_mutex_unlock(philo->left_fork);
	pthread_mutex_unlock(philo->right_fork);
}

void	sleep_and_think(t_philo *philo)
{
	if (sim_should_stop(philo))
		return ;
	print_state(philo, "is sleeping");
	precise_sleep(philo->data->time_to_sleep, philo->data);
	if (sim_should_stop(philo))
		return ;
	print_state(philo, "is thinking");
}