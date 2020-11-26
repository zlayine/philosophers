#include "philo.h"

void	print_status(t_philo *philo)
{
	printf("%d ", get_current_time());
	printf("%d ", philo->name);
	if (philo->action == FORK_ACTION)
		printf("has taken a fork");
	else if (philo->action == EAT_ACTION)
		printf("is eating");
	else if (philo->action == SLEEP_ACTION)
		printf("is sleeping");
	else if (philo->action == THINK_ACTION)
		printf("is thinking");
	else if (philo->action == DIE_ACTION)
		printf("died");
	printf("\n");
}

int		get_current_time()
{
	struct timeval current_time;

	gettimeofday(&current_time, NULL);
	return (current_time.tv_sec * 1000);
}

int		ft_do_action(t_philo *philo)
{
	int time;

	time = 0;
	if (philo->action == EAT_ACTION)
		time = philo->eat_time;
	else if (philo->action == SLEEP_ACTION)
		time = philo->sleep_time;
	else if (philo->action == THINK_ACTION)
		time = philo->think_time;
	if (philo->uptime + time >= philo->die_time)
		philo->action = 5;
	else
		philo->uptime += time;
	print_status(philo);
	if (philo->action != 5)
		usleep(time * 1000);
	else
		return (-1);
	if (philo->action == EAT_ACTION)
	{
		g_forks += 2;
		philo->start = get_current_time();
		philo->uptime = 0;
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
		if (g_forks >= 2)
			g_forks -= 2;
		else
			status = 0;
		pthread_mutex_unlock(&g_lock); 
	}
	else
		status = ft_do_action(philo);
	return (status);
}

void	check_life(t_philo *philo)
{
	if (philo->start + philo->die_time <= get_current_time())
		philo->action = 5;
}

void	*ft_philo_life(void *arg)
{
	t_philo *me;
	int		life;

	me = (t_philo*)arg;
	life = 1;
	me->action = 1;
	me->start = get_current_time();
	while (life)
	{
		// printf("[---%d---]action: %d \n", me->name, me->action);
		life = ft_action(me);
		// printf("[-------]life: %d \n", life);
		if (life == 1)
			me->action = me->action == 4 ? 1 : me->action + 1;
		else if (life == -1)
		{
			finish_simulation(me->table);
			exit(0);
		}
		else
			life = 1;
		check_life(me);
	}
	return (NULL);
}

t_philo		*init_philo(int name, t_philo *prev)
{
	int 	die_time = 10000;
	int 	eat_time = 5000;
	int		sleep_time = 4000;
	int		eat_num = -1;
	t_philo	*philo;

	philo = malloc(sizeof(t_philo));
	philo->die_time = die_time;
	philo->think_time = die_time - eat_time - sleep_time;
	philo->eat_time = eat_time;
	philo->sleep_time = sleep_time;
	philo->eat_num = eat_num;
	philo->name = name;
	philo->action = 1;
	philo->head = 0;
	philo->tid = 0;
	philo->start = 0;
	philo->uptime = 0;
	philo->next = NULL;
	philo->prev = prev;
	if (prev)
		prev->next = philo;
	printf("Philo %d created\n", name);
	return (philo);
}

t_philo		*create_philos(int total, t_table *table)
{
	t_philo *head;
	t_philo *tmp;
	int		i;

	i = 0;
	head = NULL;
	tmp = NULL;
	while (i < total)
	{
		tmp = init_philo(i, tmp);
		tmp->table = table;
		if (!head)
			head = tmp;
		i++;
	}	
	head->prev = tmp;
	tmp->next = head;
	head->head = 1;
	printf("----------------- Starting simulation ---------------\n");
	return (head);
}

t_table		*init_table(char **args)
{
	t_table	*table;
	int	persons = 3;
	int forks = 3;
	int	i = 0;

	table = malloc(sizeof(t_table));
	// table->persons = atoi(persons);
	// table->forks = atoi(forks);
	table->persons = persons;
	table->forks = forks;
	g_forks = 3;
	table->tid = malloc(sizeof(int) * persons);
	pthread_mutex_init(&g_lock, NULL);
	table->philos = create_philos(persons, table);
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
		pthread_create(&(tmp->tid), NULL, &ft_philo_life, (void *)tmp);
		table->tid[i] = tmp->tid;
		tmp = tmp->next;
		i++;
		if (tmp->head)
			break ;
	}
	i = 0;
	while (i < table->persons)
		pthread_join(table->tid[i++], NULL);

}

void	finish_simulation(t_table *table)
{
	t_philo	*tmp;
	int		i;

	table->philos->prev->next = NULL;
	while (table->philos)
	{
		tmp = table->philos->next;
		ft_del(table->philos);
		table->philos = tmp;
	}
	i = -1;
	while (++i < table->persons)
		pthread_detach(table->tid[i]);
	// error caused here
	// ft_del(table->tid);
	// ft_del(table);
	pthread_mutex_destroy(&g_lock);
	exit(0);
}

int main(int argc, char **argv)
{
	t_table	*table;

	table = init_table(++argv);
	create_lifes(table);
	return (0);
}