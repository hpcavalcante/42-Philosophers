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

void	*status_routine(void *param)
{
	t_data	*data;

	data = param;
	if (data->nb_philo == 1)
		return (NULL);
	while (1)
	{
		if (get_time() - data->philo->last_meal > data->to_die)
		{
			print_status(get_time(), data->philo, "\033[31mdied\033[0m");
			pthread_mutex_lock(&data->execution);
			data->done_execution = 1;
			pthread_mutex_unlock(&data->execution);	
			break ;	
		}
		if (data->satisfied == data->nb_meals && data->nb_meals > 0)
		{
			pthread_mutex_lock(&data->execution);
			data->done_execution = 1;
			pthread_mutex_unlock(&data->execution);	
			break ;	
		}
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
	lock_forks(philo);
	pthread_mutex_lock(&philo->data->eating);
	philo->is_eating = 1;
	philo->last_meal = get_time();
	pthread_mutex_unlock(&philo->data->eating);
	print_status(get_time(), philo, "has taken a fork");
	print_status(get_time(), philo, "has taken a fork");
	print_status(get_time(), philo, "is eating...");
	usleep(philo->data->to_eat);
	pthread_mutex_lock(&philo->data->eating);
	philo->nb_meals++;
	if (philo->nb_meals >= philo->data->nb_meals)
		philo->data->satisfied++;
	pthread_mutex_unlock(&philo->data->eating);
	unlock_forks(philo);
}

void	*routine(void *param)
{
	t_philo	*philo;

	philo = param;
	if (philo->data->nb_philo == 1)
		return (one_philo(philo));
	while (1)
	{
		if (philo->id % 2 == 0)
			usleep(1000);
		eating(philo);
		print_status(get_time(), philo, "is sleeping");
		usleep(philo->data->to_sleep);
		print_status(get_time(), philo, "is thinking");
		pthread_mutex_lock(&philo->data->execution);
		if (philo->data->done_execution == 1)
		{
			pthread_mutex_unlock(&philo->data->execution);
			break ;
		}
		pthread_mutex_unlock(&philo->data->execution);
	}
	return (NULL);
}

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

void	print_status(long int time_now, t_philo *philo, char *status)
{
	pthread_mutex_lock(&philo->data->execution);
	if (philo->data->done_execution != 1)
		printf("\ttime: %6ld | philo: %3d | status: %s\n", time_now - philo->data->start_dinner, philo->id, status);
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
t_data	*init_data(int argc, char **argv)
{
	t_data *data;
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
	int	i;

	i = 0;
	while (i < data->nb_philo)
	{
		data->philo[i].id = i + 1;
		data->philo[i].left_fork = i;
		data->philo[i].right_fork = i + 1;
		data->philo[i].data = data;
		data->philo[i].last_meal = get_time();
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
		if (pthread_create(&data->philo[i].threads, NULL, &routine, &data->philo[i]) != 0)
			print_error("At creating threads", 2);
		i++;
	}
	i = -1;
	pthread_create(&status, NULL, &status_routine, &(*data));
	while (++i < data->nb_philo)
		pthread_join(data->philo[i].threads, NULL);
	pthread_join(status, NULL);
}

int main(int argc, char **argv)
{
	t_data 			*data;

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
