/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: imeziane <imeziane@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/02 14:17:00 by imeziane          #+#    #+#             */
/*   Updated: 2026/09/10 13:07:58 by imeziane         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILO_H
#define PHILO_H

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <sys/time.h>
#include <limits.h>

typedef struct s_all	t_all;

typedef struct s_philo
{
	int				id;
	int				meals;
	int				finished;
	long			last_meal;
	pthread_t		thread;
	pthread_mutex_t	*left_fork;
	pthread_mutex_t	*right_fork;
	pthread_mutex_t	meal_mutex;
	t_all			*data;
}	t_philo;

typedef struct s_all
{
	int				nb_philo;
	int				ac;
	char			**av;
	long			start_time;
	long			time_to_die;
	long			time_to_eat;
	long			time_to_sleep;
	int				time_must_eat;
	int				meal_limit;
	int				end;
	pthread_mutex_t	print_mutex;
	pthread_mutex_t	start_mutex;
	pthread_mutex_t	end_mutex;
	pthread_mutex_t	*forks;
	t_philo			*philo;
}	t_all;

long	ft_atoi_philo(const char *str);
int		parse_args(t_all *data);

void	init_data(t_all *data);
int		init_mutexes(t_all *data);
int		init_philos(t_all *data);
void	destroy_data(t_all *data);

long    get_time_ms(void);
long    elapsed_since(long start_time);
void    precise_sleep(long duration, t_all *data);
void    print_state(t_philo *philo, const char *msg);

int     sim_should_stop(t_philo *philo);
void    stop_simulation(t_all *data);
void    *routine(void *arg);

void    eat_phase(t_philo *philo);
void    release_forks(t_philo *philo);
void    sleep_and_think(t_philo *philo);


#endif