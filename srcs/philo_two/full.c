#include "philo.h"

char	*get_action(int action)
{
	if (action == FORK_ACTION)
		return ("has taken a fork\n");
	else if (action == EAT_ACTION)
		return ("is eating\n");
	else if (action == SLEEP_ACTION)
		return ("is sleeping\n");
	else if (action == THINK_ACTION)
		return ("is thinking\n");
	else if (action == DIE_ACTION)
		return ("died\n");
	else
		return ("End of simulation: reacthed eat limit\n");
}

void	print_status(t_philo *philo, int action)
{
	char	*tmp;

	sem_wait(philo->print);
	if (philo->die == 2)
		exit (0);
	ft_putnbr(get_current_time(0, philo->start_time));
	ft_putchar(' ');
	if (action != 6)
	{
		ft_putnbr(philo->name);
		ft_putchar(' ');
	}
	ft_putstr(get_action(action));
	action != 5 ? sem_post(philo->print) : 0;
}

void	free_simulation(t_philo *curr)
{
	t_philo	*tmp;
	sem_t	*print;
	char	*name_tmp;
	int		i;

	print = curr->print;
	sem_close(curr->sem);
	sem_unlink("table_sem");
	while (curr)
	{
		name_tmp = ft_strjoin("sem_ph_", ft_itoa(curr->name));
		sem_post(curr->mtphilo);
		sem_wait(curr->mtphilo);
		curr->die = 2;
		sem_post(curr->mtphilo);
		sem_close(curr->mtphilo);
		sem_unlink(name_tmp);
		pthread_detach(curr->checker);
		ft_del(name_tmp);
		tmp = curr->next;
		ft_del(curr);
		curr = NULL;
		curr = tmp;
	}
	i = -1;
	while (++i < 3)
		sem_post(print);
	sem_close(print);
	sem_unlink("print_sem");
}

void	finish_simulation(t_table *table, int death)
{
	t_philo	*curr;
	int		i;

	i = -1;
	curr = table->philos;
	curr->prev->next = NULL;
	while (++i < table->persons)
		sem_post(curr->sem);
	free_simulation(curr);
	sem_close(table->mtdie);
	sem_unlink("sem_game");
	ft_del(table);
	exit(0);
}

t_philo		*init_philo(int name, t_philo *prev, char **args)
{
	t_philo	*philo;
	char	*tmp;

	philo = malloc(sizeof(t_philo));
	philo->die_time = ft_atoi(args[1]);
	philo->eat_time = ft_atoi(args[2]);
	philo->sleep_time = ft_atoi(args[3]);
	philo->eat_num = args[4] ? ft_atoi(args[4]) : -1;
	philo->name = name;
	tmp = ft_strjoin("sem_ph_", ft_itoa(name));
	philo->mtphilo = init_semaphore(1, tmp);
	ft_del(tmp);
	philo->head = 0;
	philo->start = 0;
	philo->die = 0;
	philo->done = 0;
	philo->next = NULL;
	philo->prev = prev;
	if (prev)
		prev->next = philo;
	return (philo);
}

t_philo		*create_philos(int total, t_table *table, char **args)
{
	t_philo			*head;
	t_philo			*tmp;
	sem_t			*sem;
	sem_t			*print;
	int				i;

	i = 0;
	head = NULL;
	tmp = NULL;
	sem = init_semaphore(table->forks, "table_sem");
	print = init_semaphore(1, "print_sem");
	while (i < total)
	{
		tmp = init_philo(i + 1, tmp, args);
		tmp->table = table;
		tmp->sem = sem;
		tmp->print = print;
		if (!head)
			head = tmp;
		i++;
	}
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
	table->end = 0;
	table->mtdie = init_semaphore(1, "sem_game");
	table->philos = create_philos(table->persons, table, args);
	return (table);
}

sem_t		*init_semaphore(int total, char *name)
{
	sem_t	*sem;

	sem_unlink(name);
	sem = sem_open(name, O_CREAT, 0666, total);
	return (sem);
}


void	ft_get_fork(t_philo *philo)
{
	sem_wait(philo->sem);
	sem_wait(philo->sem);
	print_status(philo, FORK_ACTION);
	print_status(philo, FORK_ACTION);
}

void	ft_drop_fork(t_philo *philo)
{
	sem_post(philo->sem);
	sem_post(philo->sem);
}

void	ft_eat(t_philo *philo)
{
	philo->start = get_current_time(1, philo->start_time) + (philo->die_time * 1000);
	sem_wait(philo->mtphilo);
	print_status(philo, EAT_ACTION);
	usleep(philo->eat_time * 1000);
	sem_post(philo->mtphilo);
	if (philo->eat_num != -1)
		philo->eat_num--;
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
		if (philo && philo->eat_num == 0 && philo->done && philo->die != -1)
		{
			done++;
			philo->die = -1;
		}
		if (done == table->persons || !philo)
			break ;
		philo = philo->next;
	}
	if (!philo)
		return (NULL);
	print_status(philo, SIM_OVER);
	table->end = 1;
	sem_post(table->mtdie);
	return (NULL);
}

void	*ft_philo_checker(void *arg)
{
	t_philo *philo;
	sem_t	*die;

	philo = (t_philo*)arg;
	die = philo->table->mtdie;
	while (1)
	{
		sem_wait(philo->mtphilo);
		if (philo->die == 0 && (philo->eat_num || philo->eat_num == -1) && get_current_time(1,
			philo->start_time) > philo->start)
		{
			philo->die = 1;
			print_status(philo, DIE_ACTION);
			sem_post(philo->mtphilo);
			sem_post(die);
			break ;
		}
		sem_post(philo->mtphilo);
		usleep(5);
	}
	return (NULL);
}

void	*ft_philo_life(void *arg)
{
	t_philo		*me;
	pthread_t	checker;

	me = (t_philo*)arg;
	me->start = get_current_time(1, me->start_time) + (me->die_time * 1000);
	pthread_create(&checker, NULL, &ft_philo_checker, (void *)me);
	me->checker = checker;
	while (1)
	{
		ft_get_fork(me);
		ft_eat(me);
		ft_drop_fork(me);
		ft_sleep(me);
		if (me->eat_num == 0)
		{
			me->done = 1;
			break ;
		} else if (me->die)
			break ;
	}
	return (NULL);
}

void	create_lifes(t_table *table)
{
	int				i;
	t_philo			*tmp;
	pthread_t		checker;
	struct timeval	current_time;
	pthread_t		tids[table->persons];

	i = 0;
	tmp = table->philos;
	pthread_create(&checker, NULL, &game_checker, (void *)table);
	pthread_detach(checker);
	sem_wait(table->mtdie);
	gettimeofday(&current_time, NULL);
	while (tmp)
	{
		tmp->start_time = current_time;
		pthread_create(&tmp->thrd, NULL, &ft_philo_life, (void *)tmp);
		usleep(45);
		tmp = tmp->next;
		tids[i++] = tmp->thrd;
		if (tmp->head)
			break ;
	}
	sem_wait(table->mtdie);
	sem_post(table->mtdie);
}

int		main(int argc, char **argv)
{
	t_table	*table;

	if (!valid_args(argc, ++argv))
		return (1);
	table = init_table(argv);
	create_lifes(table);
	finish_simulation(table, !table->end);
	return (0);
}