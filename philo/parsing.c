/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: imeziane <imeziane@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/25 11:55:35 by imeziane          #+#    #+#             */
/*   Updated: 2026/09/10 12:12:03 by imeziane         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

long	ft_atoi_philo(const char *str)
{
	size_t	i;
	long	res;

	if (!str || str[0] == '\0')
		return (-1);
	i = 0;
	res = 0;
	while (str[i])
	{
		if (str[i] < '0' || str[i] > '9')
			return (-1);
		res = res * 10 + (str[i] - '0');
		if (res > INT_MAX)
			return (-1);
		i++;
	}
	return (res);
}

int	parse_args(t_all *data)
{
	if (data->nb_philo <= 0)
		return (printf("Error: bad number of philosphers\n"), 1);
	if (data->time_to_die <= 0)
		return (printf("Error: bad time to die\n"), 1);
	if (data->time_to_eat <= 0)
		return (printf("Error: bad time to eat\n"), 1);
	if (data->time_to_sleep <= 0)
		return (printf("Error: bad time to sleep\n"), 1);
	if (data->ac == 6 && data->time_must_eat <= 0)
		return (printf("Error: bad must eat value\n"), 1);
	return (0);
}
