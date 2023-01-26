/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mutex.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hepiment <hepiment@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/23 21:19:21 by hepiment          #+#    #+#             */
/*   Updated: 2023/01/25 18:10:52 by hepiment         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

void	init_mutex(t_data *data)
{
	pthread_mutex_init(&data->eating, NULL);
	pthread_mutex_init(&data->execution, NULL);
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
