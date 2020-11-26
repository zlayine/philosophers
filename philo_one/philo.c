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
		philo->l_fork->state = 0;
		philo->r_fork->state = 0;
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
		if (!philo->l_fork->state && !philo->r_fork->state)
		{
			// printf("LF %d RF %d\n", philo->l_fork->state, philo->r_fork->state);
			philo->l_fork->state = 1;
			philo->r_fork->state = 1;
			print_status(philo);
		}
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

void	*ft_philo_life(void *arg)
{
	t_philo *me;
	int		life;

	me = (t_philo*)arg;
	life = 1;
	me->action = 1;
	me->start = get_current_time();
	// printf("Start %d LF %d RF %d\n", me->name, me->l_fork->state, me->r_fork->state);
	me->l_fork->state = 0;
	me->r_fork->state = 0;
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
		if (me->start + me->die_time <= get_current_time())
			me->action = 5;
	}
	return (NULL);
}

t_fork		*forge_fork()
{
	t_fork	*fork;

	fork = malloc(sizeof(t_fork));
	fork->state = 0;
	return (fork);
}

t_philo		*init_philo(int name, t_philo *prev)
{
	int 	die_time = 10000;
	int 	eat_time = 4000;
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
	philo->next = NULL;
	philo->prev = prev;
	philo->r_fork = forge_fork();
	philo->l_fork = NULL;
	if (prev)
	{
		philo->l_fork = prev->r_fork;
		prev->next = philo;
	}
	if (philo->l_fork)
		printf("%d RF: %d LF: %d\n", name, philo->r_fork->state, philo->l_fork->state);
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
		tmp = init_philo(i + 1, tmp);
		tmp->table = table;
		if (!head)
			head = tmp;
		i++;
	}
	head->l_fork = tmp->r_fork;
	printf("HEAD %d RF: %d LF: %d\n", head->name, head->r_fork->state, head->l_fork->state);
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