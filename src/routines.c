/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   routines.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hepiment <hepiment@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/23 22:46:50 by hepiment          #+#    #+#             */
/*   Updated: 2023/01/25 18:19:51 by hepiment         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

void	*status_routine(void *param)
{
	t_data	*data;

	data = param;
	if (data->nb_philo == 1)
		return (NULL);
	while (1)
	{
		if (check_death(data) || check_satisfaction(data))
			break ;
		usleep(1000);
	}
	return (NULL);
}

void	*one_philo(t_philo *philo)
{
	pthread_mutex_lock(&philo->data->forks[philo->left_fork]);
	pthread_mutex_lock(&philo->data->eating);
	philo->last_meal = get_time();
	pthread_mutex_unlock(&philo->data->eating);
	print_status(get_time(), philo, "has taken a fork");
	pthread_mutex_unlock(&philo->data->forks[philo->left_fork]);
	usleep(philo->data->to_die * 1000);
	print_status(get_time(), philo, "\033[31mdied\033[0m");
	return (NULL);
}

void	eating(t_philo *philo)
{
	long int	time;

	pthread_mutex_lock(&philo->data->forks[philo->left_fork]);
	pthread_mutex_lock(&philo->data->forks[philo->right_fork]);
	time = get_time();
	pthread_mutex_lock(&philo->data->eating);
	philo->is_eating = 1;
	philo->last_meal = get_time();
	pthread_mutex_unlock(&philo->data->eating);
	print_status(time, philo, "has taken a fork");
	print_status(time, philo, "has taken a fork");
	print_status(time, philo, "is eating...");
	usleep(philo->data->to_eat);
	pthread_mutex_lock(&philo->data->eating);
	philo->nb_meals++;
	if (philo->nb_meals == philo->data->nb_meals)
		philo->data->satisfied++;
	pthread_mutex_unlock(&philo->data->eating);
	pthread_mutex_unlock(&philo->data->forks[philo->left_fork]);
	pthread_mutex_unlock(&philo->data->forks[philo->right_fork]);
}

void	*routine(void *param)
{
	t_philo	*philo;

	philo = param;
	if (philo->data->nb_philo == 1)
		return (one_philo(philo));
	if (philo->id % 2 == 0)
		usleep(5000);
	while (1)
	{
		pthread_mutex_lock(&philo->data->execution);
		if (philo->data->done_execution == 1)
		{
			pthread_mutex_unlock(&philo->data->execution);
			break ;
		}
		pthread_mutex_unlock(&philo->data->execution);
		eating(philo);
		print_status(get_time(), philo, "is sleeping");
		usleep(philo->data->to_sleep);
		print_status(get_time(), philo, "is thinking");
	}
	return (NULL);
}
