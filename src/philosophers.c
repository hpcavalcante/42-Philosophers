/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philosophers.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hepiment <hepiment@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/23 19:10:57 by hepiment          #+#    #+#             */
/*   Updated: 2023/01/25 18:47:02 by hepiment         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

t_data	*init_data(int argc, char **argv)
{
	t_data	*data;

	data = malloc(sizeof(t_data));
	data->nb_philo = ft_atoi(argv[1]);
	data->to_die = ft_atoi(argv[2]);
	data->to_eat = ft_atoi(argv[3]) * 1000;
	data->to_sleep = ft_atoi(argv[4]) * 1000;
	if (argc > 5)
		data->nb_meals = ft_atoi(argv[5]);
	else
		data->nb_meals = 0;
	data->satisfied = 0;
	data->done_execution = 0;
	data->philo = malloc(data->nb_philo * sizeof(t_philo));
	ft_bzero(data->philo, sizeof(t_philo));
	data->forks = malloc(data->nb_philo * sizeof(pthread_mutex_t));
	return (data);
}

void	init_philo(t_data *data)
{
	int			i;
	long int	time;

	i = 0;
	time = get_time();
	while (i < data->nb_philo)
	{
		data->philo[i].id = i + 1;
		data->philo[i].left_fork = i;
		data->philo[i].right_fork = i + 1;
		data->philo[i].data = data;
		data->philo[i].last_meal = time;
		data->philo[i].nb_meals = 0;
		if (i + 1 == data->nb_philo)
			data->philo[i].right_fork = 0;
		pthread_mutex_init(&data->forks[i], NULL);
		i++;
	}
}

void	create_threads(t_data *data)
{
	int			i;
	pthread_t	status;

	i = 0;
	while (i < data->nb_philo)
	{
		if (pthread_create(&data->philo[i].threads, NULL, \
		&routine, &data->philo[i]) != 0)
			print_error("At creating threads", 2);
		i++;
	}
	i = -1;
	pthread_create(&status, NULL, &status_routine, &(*data));
	while (++i < data->nb_philo)
		pthread_join(data->philo[i].threads, NULL);
	pthread_join(status, NULL);
}

int	main(int argc, char **argv)
{
	t_data	*data;

	check_args(argc, argv);
	data = init_data(argc, argv);
	init_mutex(data);
	data->start_dinner = get_time();
	init_philo(data);
	create_threads(data);
	usleep(1000);
	destroy_mutex(data);
	free(data->philo);
	free(data->forks);
	free(data);
}
