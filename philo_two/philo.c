#include "philo.h"

void	print_status(t_philo *philo, int action)
{
	
	action != 5 ? pthread_mutex_lock(philo->print) : 0;
	// ft_putstr(ft_itoa(philo->start));
	// ft_putchar(' ');
	ft_putstr(ft_itoa(get_current_time(0)));
	ft_putchar(' ');
	ft_putstr(ft_itoa(philo->name));
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
	action != 5 ? pthread_mutex_unlock(philo->print) : 0;
}

long	get_current_time(int micro)
{
	struct timeval current_time;

	gettimeofday(&current_time, NULL);
	// printf("S: %ld | MS: %ld\n ", current_time.tv_sec, current_time.tv_sec * 100000L + current_time.tv_usec);
	if (!micro)
		return ((current_time.tv_sec * 100000L + current_time.tv_usec) / 1000);
	else
		return (current_time.tv_sec * 100000L + current_time.tv_usec);
}

void	ft_get_fork(t_philo *philo)
{
	pthread_mutex_lock(&philo->mutex[philo->r_fork]);
	print_status(philo, FORK_ACTION);
	pthread_mutex_lock(&philo->mutex[philo->l_fork]);
	print_status(philo, FORK_ACTION);
}

void	ft_drop_fork(t_philo *philo)
{
	pthread_mutex_unlock(&philo->mutex[philo->r_fork]);
	pthread_mutex_unlock(&philo->mutex[philo->l_fork]);
}

void	ft_eat(t_philo *philo)
{
	print_status(philo, EAT_ACTION);
	usleep(philo->eat_time * 1000);
	ft_drop_fork(philo);
	philo->start = get_current_time(1);
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

void	*ft_philo_checker(void *arg)
{
	t_philo	*philo;

	philo = (t_philo*)arg;
	while (1)
	{
		if (philo->eat_num == 0)
		{
			pthread_mutex_lock(philo->print);
			printf("Simulation stops here %d\n", philo->name);
			break;
		}
		if (get_current_time(1) > philo->start + (philo->die_time * 1000))
		{
			pthread_mutex_lock(philo->print);
			philo->die = 1;
			print_status(philo, DIE_ACTION);
			break ;
		}
	}
	finish_simulation(philo->table);
	return (NULL);
}

void	*ft_philo_life(void *arg)
{
	t_philo		*me;
	pthread_t	checker;
	int			life;

	me = (t_philo*)arg;
	life = 1;
	me->action = 1;
	me->start = get_current_time(1);
	pthread_create(&checker, NULL, &ft_philo_checker, (void *)me);
	while (1)
	{
		ft_get_fork(me);
		ft_eat(me);
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
	philo->action = 1;
	philo->head = 0;
	philo->start = 0;
	philo->die = 0;
	philo->next = NULL;
	philo->prev = prev;
	philo->r_fork = name - 1;
	philo->l_fork = 0;
	if (prev)
	{
		philo->l_fork = prev->r_fork;
		prev->next = philo;
	}
	// printf("Philo %d created\n", name);
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

	// printf("----------------- Creating simulation ---------------\n");
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
	// printf("----------------- Starting simulation ---------------\n");
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
	t_philo 	*tmp;
	pthread_t	tids[table->persons];
	int			i;

	i = 0;
	tmp = table->philos;
	while (tmp)
	{
		pthread_create(&tmp->thrd, NULL, &ft_philo_life, (void *)tmp);
		tmp = tmp->next;
		tids[i] = tmp->thrd;
		i++;
		if (tmp->head)
			break ;
		usleep(100);
	}
	i = 0;
	while (i < table->persons)
		pthread_join(tids[i++], NULL);
}

void	finish_simulation(t_table *table)
{
	t_philo	*curr;
	t_philo	*tmp;
	int		i;

	curr = table->philos;
	i = -1;

	while (++i < table->persons)
		pthread_mutex_destroy(&curr->mutex[i]);
	pthread_mutex_destroy(curr->print);
	// ft_del(curr->print);
	// ft_del(curr->mutex);
	// curr->prev->next = NULL;
	// while (curr)
	// {
	// 	pthread_detach(curr->thrd);
	// 	tmp = curr->next;
	// 	ft_del(curr);
	// 	curr = tmp;
	// }
	// ft_del(table);
	exit(0);
}

int		valid_args(int total, char **args)
{
	if (total < 2 || total > 6)
		return (0);
	// if (ft_atoi(args[0]) <= 0)
	// {

	// }
	// if (ft_atoi(args[1]) <= 0)
	// {}
	return (1);
}

int main(int argc, char **argv)
{
	t_table	*table;

	if (!valid_args(argc, ++argv))
	{
		printf("Please specify the required arguments\n");
		return (1);
	}
	table = init_table(argv);
	create_lifes(table);
	return (0);
}