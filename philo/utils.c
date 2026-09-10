/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: imeziane <imeziane@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/02 14:43:10 by imeziane          #+#    #+#             */
/*   Updated: 2026/09/10 12:36:54 by imeziane         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

long	get_time_ms(void)
{
	struct timeval	tv;

	gettimeofday(&tv, NULL);
	return (tv.tv_sec * 1000 + tv.tv_usec / 1000);
}

long	elapsed_since(long start_time)
{
	return (get_time_ms() - start_time);
}

void	precise_sleep(long duration, t_all *data)
{
	long	start;

	start = get_time_ms();
	while (!sim_should_stop(data->philo)
		&& elapsed_since(start) < duration)
		usleep(500);
}

void	print_state(t_philo *philo, const char *msg)
{
	long	timestamp;

	pthread_mutex_lock(&philo->data->print_mutex);
	if (sim_should_stop(philo))
	{
		pthread_mutex_unlock(&philo->data->print_mutex);
		return ;
	}
	timestamp = elapsed_since(philo->data->start_time);
	printf("%ld %d %s\n", timestamp, philo->id, msg);
	pthread_mutex_unlock(&philo->data->print_mutex);
}
