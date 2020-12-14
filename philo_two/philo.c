#include "philo.h"
#include <unistd.h> 

void	print_status(t_philo *philo, int action)
{
	sem_wait(philo->print);
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
	sem_post(philo->print);
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
		if (philo->die)
			break ;
		if (philo->eat_num == 0)
		{
			sem_wait(philo->print);
			puts("Simulation stops here");
			finish_simulation(philo->table);
			break;
		}
		if (get_current_time(1) > philo->start + (philo->die_time * 1000))
		{
			philo->die = 1;
			print_status(philo, DIE_ACTION);
			finish_simulation(philo->table);
			break ;
		}
	}
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
	me->checker = checker;
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
	if (prev)
		prev->next = philo;
	// printf("Philo %d created\n", name);
	return (philo);
}

t_philo		*create_philos(int total, t_table *table, char **args)
{
	t_philo 		*head;
	t_philo			*tmp;
	sem_t			*sem;
	sem_t			*print;
	int		i;

	// printf("----------------- Creating simulation ---------------\n");
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
		usleep(150);
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

	i = -1;
	curr = table->philos;
	curr->prev->next = NULL;
	while (curr)
	{
		curr->die = 1;
		pthread_detach(curr->thrd);
		pthread_detach(curr->checker);
		tmp = curr->next;
		// ft_del(curr);
		curr = tmp;
	}
	while (table->forks < table->persons)
	{
    	sem_post(table->philos->sem);
		table->forks++;
	}
	sem_close(table->philos->sem);
    sem_close(table->philos->print); 
	sem_unlink("table_sem");
	sem_unlink("print_sem");

	// ft_del(curr->print);
	// ft_del(curr->mutex);
	
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

sem_t	*init_semaphore(int total, char *name)
{
	sem_t	*sem;

	sem_unlink(name);
	sem = malloc(sizeof(sem_t));
	sem = sem_open(name, O_CREAT, 0666, total);
	return (sem);
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