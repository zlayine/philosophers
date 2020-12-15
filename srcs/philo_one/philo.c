#include "philo.h"

void	print_status(t_philo *philo, int action)
{
	char	*tmp;

	pthread_mutex_lock(philo->print);
	tmp = ft_itoa(get_current_time(0, philo->start_time));
	ft_putstr(tmp);
	ft_del(tmp);
	ft_putchar(' ');
	tmp = ft_itoa(philo->name);
	ft_putstr(tmp);
	ft_del(tmp);
	ft_putchar(' ');
	if (action == FORK_ACTION)
		ft_putstr("has taken a fork");
	else if (action == EAT_ACTION)
		ft_putstr("is eating");
	else if (action == SLEEP_ACTION)
		ft_putstr("is sleeping");
	else if (action == THINK_ACTION)
		ft_putstr("is thinking");
	else if (action == DIE_ACTION)
		ft_putstr("died");
	ft_putchar('\n');
	pthread_mutex_unlock(philo->print);
}

long	get_current_time(int micro, struct timeval start_time)
{
	struct timeval	current_time;
	int				time;

	gettimeofday(&current_time, NULL);
	time = ((current_time.tv_sec - start_time.tv_sec) * 1000000L + current_time.tv_usec) - start_time.tv_usec;
	if (!micro)
		return (time / 1000);
	else
		return (time);
}

void	ft_get_fork(t_philo *philo)
{
	while (philo->r_fork == -1 || philo->l_fork == -1)
		;
	pthread_mutex_lock(&philo->mutex[philo->r_fork]);
	philo->next->l_fork = -1;
	print_status(philo, FORK_ACTION);
	while (philo->r_fork == -1 || philo->l_fork == -1)
		;
	pthread_mutex_lock(&philo->mutex[philo->l_fork]);
	philo->prev->r_fork = -1;
	print_status(philo, FORK_ACTION);
}

void	ft_drop_fork(t_philo *philo)
{
	pthread_mutex_unlock(&philo->mutex[philo->r_fork]);
	philo->next->l_fork = philo->name - 1;
	pthread_mutex_unlock(&philo->mutex[philo->l_fork]);
	if (!philo->head)
		philo->prev->r_fork = philo->name - 2;
	else
		philo->prev->r_fork = philo->table->forks - 1;
}

void	ft_eat(t_philo *philo)
{
	philo->start = get_current_time(1, philo->start_time);
	print_status(philo, EAT_ACTION);
	usleep(philo->eat_time * 1000);
	ft_drop_fork(philo);
	if (philo->eat_num != -1)
		philo->eat_num--;
	ft_sleep(philo);
}

void	ft_sleep(t_philo *philo)
{
	print_status(philo, SLEEP_ACTION);
	usleep(philo->sleep_time * 1000);
	print_status(philo, THINK_ACTION);
}

void	*game_checker(void *arg)
{
	t_table	*table;
	t_philo *philo;
	int		done;

	table = (t_table*)arg;
	philo = table->philos;
	done = 0;
	while (1)
	{
		if (philo->eat_num == 0 && philo->done && philo->die != -1)
		{
			done++;
			philo->die = -1;
		}
		philo = philo->next;
		if (done == table->persons)
			break ;
	}
	pthread_mutex_lock(philo->print);
	finish_simulation(philo->table, philo->die == 1);
	return (NULL);
}

void	*ft_philo_checker(void *arg)
{
	t_philo *philo;
	int		done;

	philo = (t_philo*)arg;
	done = 0;
	while (1)
	{
		if ((philo->eat_num || philo->eat_num == -1) && get_current_time(1, philo->start_time) > philo->start + (philo->die_time * 1000))
		{
			philo->die = 1;
			print_status(philo, DIE_ACTION);
			break ;
		}
	}
	pthread_mutex_lock(philo->print);
	finish_simulation(philo->table, philo->die == 1);
	return (NULL);
}

void	*ft_philo_life(void *arg)
{
	t_philo			*me;
	pthread_t		checker;

	me = (t_philo*)arg;
	me->start = get_current_time(1, me->start_time);
	pthread_create(&checker, NULL, &ft_philo_checker, (void *)me);
	while (1)
	{
		ft_get_fork(me);
		ft_eat(me);
		if (me->eat_num == 0)
		{
			me->done = 1;
			break ;
		}
	}
	return (NULL);
}

t_philo		*init_philo(int name, t_philo *prev, char **args)
{
	t_philo	*philo;

	philo = malloc(sizeof(t_philo));
	philo->die_time = ft_atoi(args[1]);
	philo->eat_time = ft_atoi(args[2]);
	philo->sleep_time = ft_atoi(args[3]);
	philo->eat_num = args[4] ? ft_atoi(args[4]) : -1;
	philo->name = name;
	philo->head = 0;
	philo->start = 0;
	philo->die = 0;
	philo->done = 0;
	philo->next = NULL;
	philo->prev = prev;
	philo->r_fork = name - 1;
	philo->l_fork = 0;
	if (prev)
	{
		philo->l_fork = prev->r_fork;
		prev->next = philo;
	}
	return (philo);
}

void		init_mutex(pthread_mutex_t *mutex, int t)
{
	int		i;

	i = -1;
	while (++i < t)
		pthread_mutex_init(&mutex[i], NULL);
}

t_philo		*create_philos(int total, t_table *table, char **args)
{
	t_philo 		*head;
	t_philo			*tmp;
	pthread_mutex_t	*mutex;
	pthread_mutex_t	*print;
	int		i;

	i = 0;
	head = NULL;
	tmp = NULL;
	mutex = malloc(sizeof(pthread_mutex_t) * total);
	print = malloc(sizeof(pthread_mutex_t));
	init_mutex(mutex, total);
	pthread_mutex_init(print, NULL);
	while (i < total)
	{
		tmp = init_philo(i + 1, tmp, args);
		tmp->table = table;
		tmp->mutex = mutex;
		tmp->print = print;
		if (!head)
			head = tmp;
		i++;
	}
	head->l_fork = tmp->r_fork;
	head->prev = tmp;
	tmp->next = head;
	head->head = 1;
	return (head);
}

t_table		*init_table(char **args)
{
	t_table	*table;
	int		i;

	i = 0;
	table = malloc(sizeof(t_table));
	table->persons = atoi(args[0]);
	table->forks = atoi(args[0]);
	table->philos = create_philos(table->persons, table, args);
	return (table);
}

void	create_lifes(t_table *table)
{
	t_philo 		*tmp;
	pthread_t		tids[table->persons];
	int				i;
	struct timeval	current_time;
	pthread_t		checker;

	i = 0;
	tmp = table->philos;
	gettimeofday(&current_time, NULL);
	while (tmp)
	{
		tmp->start_time = current_time;
		pthread_create(&tmp->thrd, NULL, &ft_philo_life, (void *)tmp);
		tmp = tmp->next;
		tids[i++] = tmp->thrd;
		if (tmp->head)
			break ;
		// usleep(45);
	}
	i = 0;
	while (i < table->persons)
		pthread_join(tids[i++], NULL);
	pthread_create(&checker, NULL, &game_checker, (void *)table);
	pthread_join(checker, NULL);
}

void	finish_simulation(t_table *table, int death)
{
	t_philo	*curr;
	t_philo	*tmp;
	int		i;

	curr = table->philos;
	i = -1;
	while (++i < table->persons)
		pthread_mutex_destroy(&curr->mutex[i]);
	pthread_mutex_destroy(curr->print);
	curr->prev->next = NULL;
	while (curr)
	{
		pthread_detach(curr->thrd);
		tmp = curr->next;
		ft_del(curr);
		curr = tmp;
	}
	ft_del(table);
	ft_putstr("End of simulation: ");
	if (death)
		ft_putstr("one of the philosophers died\n");
	else
		ft_putstr("philosophers reached the eat limit\n");
	exit(0);
}

int		valid_args(int total, char **args)
{
	if (total < 2 || total > 6)
	{
		ft_putstr("Error: please specify the required arguments\n");
		return (0);
	}
	if (ft_atoi(args[0]) <= 0 || ft_atoi(args[1]) <= 0 || ft_atoi(args[2]) <= 0 || ft_atoi(args[3]) <= 0 || ft_atoi(args[0]) <= 0)
	{
		ft_putstr("Error: value of an argument is out of range\n");
		return (0);
	}
	return (1);
}

int main(int argc, char **argv)
{
	t_table	*table;

	if (!valid_args(argc, ++argv))
		return (1);
	table = init_table(argv);
	create_lifes(table);
	return (0);
}