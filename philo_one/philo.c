#include "philo.h"

void	print_status(t_philo *philo, int action)
{
	pthread_mutex_lock(philo->print);
	printf("%d ", get_current_time());
	printf("%d ", philo->name);
	if (action == FORK_ACTION)
		printf("has taken a fork");
	else if (action == EAT_ACTION)
		printf("is eating");
	else if (action == SLEEP_ACTION)
		printf("is sleeping");
	else if (action == THINK_ACTION)
		printf("is thinking");
	else if (action == DIE_ACTION)
		printf("died");
	printf("\n");
	pthread_mutex_unlock(philo->print);

}

int		get_current_time()
{
	struct timeval current_time;

	gettimeofday(&current_time, NULL);
	return (current_time.tv_sec * 1000);
}

int		life_checker(t_philo *philo, int *time)
{
	int	death;

	death = get_current_time() - philo->start >= philo->die_time;
	if (death && philo->action != EAT_ACTION)
		return (0);
	death = get_current_time() - philo->start + *time >= philo->die_time;
	if (death && philo->action == THINK_ACTION)
		*time = philo->die_time - get_current_time() - philo->start;
	else if (death && philo->action == SLEEP_ACTION)
		return (-1);
	return (1);
}

int		ft_do_action(t_philo *philo)
{
	int time;

	// printf("%d UPTIME: %d\n", philo->name, get_current_time() - philo->start);
	time = 0;
	if (philo->action == EAT_ACTION)
		time = philo->eat_time;
	else if (philo->action == SLEEP_ACTION)
		time = philo->sleep_time;
	else if (philo->action == THINK_ACTION)
		time = philo->think_time;
	if (!life_checker(philo, &time))
		philo->action = 5;
	print_status(philo);
	if (philo->action != 5)
		usleep(time * 1000);
	else
		return (-1);
	if (philo->action == EAT_ACTION)
	{
		// philo->l_fork->state = 0;
		// philo->r_fork->state = 0;
		philo->start = get_current_time();
		// printf("LF %d RF %d\n", philo->l_fork->state, philo->r_fork->state);
	}
	return (1);
}

int		ft_action(t_philo *philo)
{
	int status;

	status = 1;
	if (philo->action == FORK_ACTION)
	{
		pthread_mutex_lock(&g_lock);
		// printf("%d LF %d RF %d\n", philo->name, philo->l_fork->state, philo->r_fork->state);
		// if (!philo->l_fork->state && !philo->r_fork->state)
		// {
			// printf("LF %d RF %d\n", philo->l_fork->state, philo->r_fork->state);
			print_status(philo);
		// }
		else
		{
			status = 0;
			// printf("----------- no fork for %d\n", philo->name);
		}
		pthread_mutex_unlock(&g_lock); 
	}
	else
		status = ft_do_action(philo);
	return (status);
}

void	ft_get_fork(t_philo *philo)
{
	pthread_mutex_lock(philo->r_fork);
	print_status(philo, FORK_ACTION);
	pthread_mutex_lock(philo->l_fork);
	print_status(philo, FORK_ACTION);
}

void	ft_release_fork(t_philo *philo)
{
	pthread_mutex_unlock(philo->r_fork);
	pthread_mutex_unlock(philo->l_fork);
}

void	ft_eat(t_philo *philo)
{
	print_status(philo, EAT_ACTION);
	philo->start = get_current_time();
	usleep(philo->eat_time * 1000);
}

void	ft_sleep(t_philo *philo)
{
	print_status(philo, SLEEP_ACTION);
	usleep(philo->sleep_time * 1000);
	print_status(philo, THINK_ACTION);
}

void	*ft_philo_life(void *arg)
{
	t_philo *me;
	int		life;

	me = (t_philo*)arg;
	life = 1;
	me->action = 1;
	me->start = get_current_time();
	setup_philo_checker();
	while (life)
	{
		ft_get_fork(me);
		ft_eat(me);
		ft_release_fork(me);
		ft_sleep(me);
		life = check_philo(me);
	}
	return (NULL);
}

t_philo		*init_philo(int name, t_philo *prev, char **args, pthread_mutex_t *mutex)
{
	t_philo	*philo;

	philo = malloc(sizeof(t_philo));
	philo->die_time = ft_atoi(args[1]);
	philo->eat_time = ft_atoi(args[2]);
	philo->sleep_time = ft_atoi(args[3]);
	philo->eat_num = ft_atoi(args[4]);
	philo->name = name;
	philo->action = 1;
	philo->head = 0;
	philo->start = 0;
	philo->next = NULL;
	philo->prev = prev;
	philo->r_fork = &mutex[name - 1];
	philo->l_fork = NULL;
	if (prev)
	{
		philo->l_fork = prev->r_fork;
		prev->next = philo;
	}
	printf("Philo %d created\n", name);
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
	pthread_mutex_t	mutex[total];
	pthread_mutex_t	print;
	int		i;

	i = 0;
	head = NULL;
	tmp = NULL;
	init_mutex(mutex, total);
	pthread_mutex_init(&print, NULL);
	while (i < total)
	{
		tmp = init_philo(i + 1, tmp, args, mutex);
		tmp->print = &print;
		if (!head)
			head = tmp;
		i++;
	}
	head->l_fork = tmp->r_fork;
	head->prev = tmp;
	tmp->next = head;
	head->head = 1;
	printf("----------------- Starting simulation ---------------\n");
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
	t_philo *tmp;
	int		i;

	i = 0;
	tmp = table->philos;
	while (tmp)
	{
		pthread_create(&(tmp->thrd), NULL, &ft_philo_life, (void *)tmp);
		pthread_join(tmp->thrd, NULL);
		tmp = tmp->next;
		i++;
		if (tmp->head)
			break ;
		usleep(50);
	}
	// i = 0;
	// while (i < table->persons)
		// pthread_join(table->tid[i++], NULL);
}

void	finish_simulation(t_table *table)
{
	t_philo	*tmp;
	int		i;

	// table->philos->prev->next = NULL;
	// while (table->philos)
	// {
	// 	tmp = table->philos->next;
	// 	ft_del(table->philos);
	// 	table->philos = tmp;
	// }
	// i = -1;
	// while (++i < table->persons)
	// 	pthread_detach(table->tid[i]);
	// error caused here
	// ft_del(table->tid);
	// ft_del(table);
	// pthread_mutex_destroy(&g_lock);
	exit(0);
}

int		check_arguments(int total, char **args)
{
	if (total < 2 || total > 6)
		return (0);
	// check values of args here
}

int main(int argc, char **argv)
{
	t_table	*table;

	if (!check_arguments(argc, ++argv))
	{
		printf("Please specify the required arguments\n");
		return (1);
	}
	table = init_table(argv);
	create_lifes(table);
	return (0);
}