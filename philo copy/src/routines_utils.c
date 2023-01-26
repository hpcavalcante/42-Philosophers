/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   routines_utils.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hepiment <hepiment@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/25 18:19:27 by hepiment          #+#    #+#             */
/*   Updated: 2023/01/25 18:19:54 by hepiment         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

int	check_death(t_data *data)
{
	long int	time;

	pthread_mutex_lock(&data->eating);
	time = get_time();
	if (time - data->philo->last_meal > data->to_die)
	{
		print_status(time, data->philo, "\033[31mdied\033[0m");
		pthread_mutex_lock(&data->execution);
		data->done_execution = 1;
		pthread_mutex_unlock(&data->execution);
		pthread_mutex_unlock(&data->eating);
		return (1);
	}
	pthread_mutex_unlock(&data->eating);
	return (0);
}

int	check_satisfaction(t_data *data)
{
	pthread_mutex_lock(&data->eating);
	if (data->satisfied == data->nb_philo && data->nb_meals > 0)
	{
		pthread_mutex_lock(&data->execution);
		data->done_execution = 1;
		pthread_mutex_unlock(&data->execution);
		pthread_mutex_unlock(&data->eating);
		return (1);
	}
	pthread_mutex_unlock(&data->eating);
	return (0);
}
