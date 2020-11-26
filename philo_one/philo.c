#include "../includes/philo.h"

void	ft_eat()
{

}

// void	ft_sleep(int time, int name)
// {
// 	print_status(3, name);
// 	usleep(time * 1000);
// }

void	ft_die()
{

}

void	ft_do_action(t_philo *philo, int action)
{
	int time;
	if (action == EAT_ACTION)
		time = philo->eat_time;
	else if (action == SLEEP_ACTION)
		time = philo->sleep_time;
	else if (action == THINK_ACTION)
		time = philo->think_time;
}

int		ft_action(t_philo *philo, int action)
{
	int status;

	status = 0;
	if (action == FORK_ACTION)
	{
		pthread_mutex_lock(&g_lock); 
		if (g_forks > 0)
			g_forks -= 2;
		else
			status = -1;
		pthread_mutex_unlock(&g_lock); 
	}
	else if (action != DIE_ACTION)
	{
		print_status(action, philo->name);
		usleep(10 * 1000);
		status = 1;
	}
	if (action == EAT_ACTION)
	{
		g_forks += 2;
		philo->start = reset_timer(philo);
	}
	if (action == DIE_ACTION)
		print_status(action, philo->name);
	return (status);
}

void	print_status(int status, int name)
{
	struct timeval current_time;

	gettimeofday(&current_time, NULL);
	printf("%d ", current_time.tv_sec * 1000);
	printf("%d ", name);
	if (status == FORK_ACTION)
		printf("has taken a fork");
	else if (status == EAT_ACTION)
		printf("is eating");
	else if (status == SLEEP_ACTION)
		printf("is sleeping");
	else if (status == THINK_ACTION)
		printf("is thinking");
	else if (status == DIE_ACTION)
		printf("died");
}

int		check_life(int start, int end, int time)
{

}

void	*ft_philo_life(void *arg)
{
	t_philo *me;
	int		life;
	int		status;

	int		start;
	int		end;
	struct timeval current_time;

	me = (t_philo*)arg;
	life = 1;
	status = 1;
	while (life)
	{
		life = ft_action(me, status);
		if (life == 1)
			status = status == 4 ? 1 : status + 1;
		else if (life == -1)
			status = 5;
		else
			life = 1;
		// if (!check_life(start, end, me->die_time))
		// 	ft_die();
		// gettimeofday(&current_time, NULL);
		// start = current_time.tv_sec;
		
		// gettimeofday(&current_time, NULL);
	}
	// pthread_mutex_lock(&g_lock); 
	printf("%d is alive\n", me->name);
	// sleep(3);
  	// pthread_mutex_unlock(&g_lock); 
	return (NULL);
}

t_philo		*init_philo(int name, t_philo *prev)
{
	int 	die_time = 0;
	int 	eat_time = 0;
	int		sleep_time = 0;
	int		eat_num = -1;
	t_philo	*philo;

	philo = malloc(sizeof(t_philo));
	philo->die_time = die_time;
	philo->die_time = die_time - eat_time - sleep_time;
	philo->eat_time = eat_time;
	philo->sleep_time = sleep_time;
	philo->eat_num = eat_num;
	philo->name = name;
	philo->status = 0;
	philo->head = 0;
	philo->tid = 0;
	philo->start = 0;
	philo->next = NULL;
	philo->prev = prev;
	if (prev)
		prev->next = philo;
	printf("Philo %d created\n", name);
	return (philo);
}

t_philo		*create_philos(int total)
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
		if (!head)
			head = tmp;
		i++;
	}	
	head->prev = tmp;
	tmp->next = head;
	head->head = 1;
	printf("Philo head %d created\n", head->name);
	printf("----------------- Philos created ---------------\n");
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
	table->tid = malloc(sizeof(int) * persons);
	if (pthread_mutex_init(&g_lock, NULL) != 0)
	{ 
        printf("\n mutex init has failed\n"); 
        // return 1; 
    }
	table->philos = create_philos(persons);
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

int main(int argc, char **argv)
{
	t_table	*table;

	table = init_table(NULL);

	create_lifes(table);
	pthread_mutex_destroy(&g_lock); 

	return (0);
}