#include "philo.h"

void	print_status(t_philo *philo, int action)
{
	char	*tmp;

	sem_wait(philo->print);
	if (philo->table->end && action != 5)
		return ;
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
	// action != DIE_ACTION ? sem_post(philo->print) : 0;
	sem_post(philo->print);
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
    sem_wait(philo->sem);
	print_status(philo, FORK_ACTION);
    sem_wait(philo->sem);
	print_status(philo, FORK_ACTION);
	philo->table->forks -= 2;
}

void	ft_drop_fork(t_philo *philo)
{
	sem_post(philo->sem);
	sem_post(philo->sem);
	philo->table->forks += 2;
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
		if (philo && philo->eat_num == 0 && philo->done && philo->die != -1)
		{
			done++;
			philo->die = -1;
		} else if (table->end)
			break ;
		philo = philo->next;
		if (done == table->persons)
			break ;
	}
	finish_simulation(philo->table, table->end > 0);
	return (NULL);
}

// t_philo	*get_philosopher(int name, t_table *table)
// {
// 	t_philo *philo;

// 	philo = table->philos;
// 	while (philo)
// 	{
// 		if (philo->name == name)
// 			break ;
// 		philo = philo->next;
// 	}
// 	return (philo);
// }

void	*ft_philo_checker(void *arg)
{
	t_philo *philo;

	philo = (t_philo*)arg;
	while (philo->table->end == 0)
	{
		if ((philo->eat_num || philo->eat_num == -1) && get_current_time(1, philo->start_time) > philo->start + (philo->die_time * 1000))
		{
			philo->die = 1;
			print_status(philo, DIE_ACTION);
			philo->table->end = !philo->table->end ? philo->name : philo->table->end;
			break ;
		}
	}
	return (NULL);
}

void	*ft_philo_life(void *arg)
{
	t_philo		*me;
	pthread_t	checker;

	me = (t_philo*)arg;
	me->start = get_current_time(1, me->start_time);
	pthread_create(&checker, NULL, &ft_philo_checker, (void *)me);
	me->checker = checker;
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
	if (prev)
		prev->next = philo;
	return (philo);
}

t_philo		*create_philos(int total, t_table *table, char **args)
{
	t_philo 		*head;
	t_philo			*tmp;
	sem_t			*sem;
	sem_t			*print;
	int		i;

	i = 0;
	head = NULL;
	tmp = NULL;
	// if total = 0 may sigfault
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
	table->philos = create_philos(table->persons, table, args);
	return (table);
}

void	create_lifes(t_table *table)
{
	int				i;
	t_philo 		*tmp;
	pthread_t		checker;
	struct timeval	current_time;
	pthread_t		tids[table->persons];

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
	pthread_create(&checker, NULL, &game_checker, (void *)table);
	table->checker = checker;
	pthread_join(checker, NULL);
}

void	finish_simulation(t_table *table, int death)
{
	t_philo	*curr;
	t_philo	*tmp;
	sem_t	*print;

	int		i;

	i = -1;
	curr = table->philos;
	curr->prev->next = NULL;
	print = curr->print;
	while (table->forks < table->persons)
	{
    	sem_post(curr->sem);
		table->forks++;
	}
    sem_post(curr->print); 
    sem_close(curr->print); 
	sem_close(curr->sem);
	sem_unlink("table_sem");
	sem_unlink("print_sem");
	while (curr)
	{
		curr->die = 1;
		pthread_detach(curr->thrd);
		pthread_detach(curr->checker);
		tmp = curr->next;
		ft_del(curr);
		curr = tmp;
	}
	pthread_detach(table->checker);
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

sem_t	*init_semaphore(int total, char *name)
{
	sem_t	*sem;

	sem_unlink(name);
	sem = sem_open(name, O_CREAT, 0777, total);
	return (sem);
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