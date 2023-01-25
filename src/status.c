/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   status.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hepiment <hepiment@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/23 22:48:18 by hepiment          #+#    #+#             */
/*   Updated: 2023/01/25 18:10:38 by hepiment         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

void	print_status(long int time_now, t_philo *philo, char *status)
{
	pthread_mutex_lock(&philo->data->execution);
	time_now = get_time();
	if (philo->data->done_execution != 1)
		printf("\ttime: %6ld | philo: %3d | status: %s\n"\
		, time_now - philo->data->start_dinner, philo->id, status);
	pthread_mutex_unlock(&philo->data->execution);
}

int	check_status(t_data *data, int i)
{
	if (data->philo[i].last_meal > data->to_die)
	{
		print_status(get_time(), data->philo + i, "\033[31mdied\033[0m");
		return (1);
	}
	return (0);
}

int	all_satisfied(t_data *data)
{
	if (data->satisfied == data->nb_philo)
		return (1);
	return (0);
}
