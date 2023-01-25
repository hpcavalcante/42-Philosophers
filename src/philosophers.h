/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philosophers.h                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hepiment <hepiment@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/17 20:34:22 by hepiment          #+#    #+#             */
/*   Updated: 2023/01/25 14:22:49 by hepiment         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILOSOPHERS_H
# define PHILOSOPHERS_H

# include <stdlib.h>
# include <stdio.h>
# include <unistd.h>
# include <pthread.h>
# include <stdbool.h>
# include <limits.h>
# include <sys/time.h>

# define USAGE "Usage: ./philo number_of_philos ms_to_die ms_eat ms_sleep (have_to_eat)"

typedef struct s_data	t_data;

typedef struct s_philo
{
	int 			id;
	int				right_fork;
	int				left_fork;
	int				is_eating;
	long int		last_meal;
	int				nb_meals;
	int				died;
	pthread_t		threads;
	t_data			*data;
	
} t_philo;

typedef struct s_data
{
	int 			nb_philo;
	int				to_die;
	int				to_eat;
	int				to_sleep;
	int				nb_meals;
	int				satisfied;
	int				done_execution;
	long int		start_dinner;
	t_philo			*philo;
	pthread_t		status_checker;
	pthread_mutex_t	execution;
	pthread_mutex_t	eating;
	pthread_mutex_t *forks;
	
} t_data;

void	lock_forks(t_philo *philo);
void	unlock_forks(t_philo *philo);
void	*routine(void *param);
void	*status_routine(void *param);
int		check_status(t_data *data, int i);
int		all_satisfied(t_data *data);
void	init_mutex(t_data *data);
void	destroy_mutex(t_data *data);

/* Utils */
int		ft_isdigit(int c);
int		ft_atoi(const char *ptr);
void	ft_bzero(void *s, size_t n);
long	get_time(void);
void	check_args(int argc, char **argv);
void	print_status(long int time_now, t_philo *philo, char *status);
void	print_error(char *msg, int type);

#endif