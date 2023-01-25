/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mutex.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hepiment <hepiment@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/23 21:19:21 by hepiment          #+#    #+#             */
/*   Updated: 2023/01/25 14:02:56 by hepiment         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

void	init_mutex(t_data *data)
{
	pthread_mutex_init(&data->eating, NULL);
	pthread_mutex_init(&data->execution, NULL);
}

void	lock_forks(t_philo *philo)
{
	if (philo->id == philo->data->nb_philo)
	{
		pthread_mutex_lock(&philo->data->forks[philo->left_fork]);
		pthread_mutex_lock(&philo->data->forks[philo->right_fork]);
	}
	else
	{
		pthread_mutex_lock(&philo->data->forks[philo->right_fork]);
		pthread_mutex_lock(&philo->data->forks[philo->left_fork]);
	}
}

void	unlock_forks(t_philo *philo)
{
	if (philo->id % 2 == 0)
	{
		pthread_mutex_unlock(&philo->data->forks[philo->left_fork]);
		pthread_mutex_unlock(&philo->data->forks[philo->right_fork]);
	}
	else
	{
		pthread_mutex_unlock(&philo->data->forks[philo->right_fork]);
		pthread_mutex_unlock(&philo->data->forks[philo->left_fork]);
	}
}

void	destroy_mutex(t_data *data)
{
	int	i;

	i = -1;
	pthread_mutex_destroy(&data->eating);
	pthread_mutex_destroy(&data->execution);
	i = -1;
	while (++i < data->nb_philo)
		pthread_mutex_destroy(&data->forks[i]);
}