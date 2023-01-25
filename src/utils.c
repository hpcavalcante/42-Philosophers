/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hepiment <hepiment@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/19 15:46:59 by hepiment          #+#    #+#             */
/*   Updated: 2023/01/23 22:22:32 by hepiment         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

int	ft_isdigit(int c)
{
	if (c >= '0' && c <= '9')
		return (1);
	return (0);
}

int	ft_atoi(const char *ptr)
{
	int	res;
	int	sign;
	int	i;

	res = 0;
	sign = 1;
	i = 0;
	while (ptr[i] == ' ' || (ptr[i] >= '\t' && ptr[i] <= '\r' ))
		i++;
	if (ptr[i] == '+' || ptr[i] == '-')
	{
		if (ptr[i] == '-')
			sign *= -1;
		i++;
	}
	while (ptr[i] >= '0' && ptr[i] <= '9')
	{
		res = ptr[i] - '0' + (res * 10);
		i++;
	}
	return (res * sign);
}

long	get_time(void)
{
	struct timeval	time;

	gettimeofday(&time, NULL);
	return ((time.tv_sec * 1000) + (time.tv_usec / 1000));
}

void	ft_bzero(void *s, size_t n)
{
	unsigned char	*s_s;
	size_t			i;

	i = 0;
	s_s = (unsigned char *)s;
	while (i < n)
	{
		s_s[i] = '\0';
		i++;
	}
}

void	print_error(char *msg, int type)
{
	printf("Error: %s\n", msg);
	if (type == 1)
		printf("%s\n", USAGE);
	exit (type);
}

void	check_args(int argc, char **argv)
{
	int i;
	int	j;

	i = 1;	
	if (argc < 5)
		print_error("Too few arguments:", 1);
	else if (argc > 6)
		print_error("Wrong number of arguments:", 1);
	while (argv[i])
	{
		j = 0;
		while (argv[i][j])
		{
			if (!ft_isdigit(argv[i][j]))
				print_error("Use only positive integers", 1);
			j++;
		}
		i++;
	}
}