/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   full.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zlayine <zlayine@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/12/15 14:24:59 by zlayine           #+#    #+#             */
/*   Updated: 2020/12/18 13:49:15 by zlayine          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

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

	pthread_mutex_lock(philo->print);
	ft_putnbr(get_current_time(0, philo->start_time));
	ft_putchar('\t');
	if (action != 6)
	{
		ft_putnbr(philo->name);
		ft_putchar(' ');
	}
	ft_putstr(get_action(action));
	action != 5 ? pthread_mutex_unlock(philo->print) : 0;
}

// long	get_current_time(int micro, struct timeval start_time)
// {
// 	struct timeval	current_time;
// 	int				time;

// 	gettimeofday(&current_time, NULL);
// 	time = ((current_time.tv_sec - start_time.tv_sec)
// 		* 1000000L + current_time.tv_usec) - start_time.tv_usec;
// 	if (!micro)
// 		return (time / 1000);
// 	else
// 		return (time);
// }

// int		valid_args(int total, char **args)
// {
// 	if (total < 5 || total > 6)
// 	{
// 		ft_putstr("Error: please specify the required arguments\n");
// 		return (0);
// 	}
// 	if (!ft_is_strdig(args[0]) || !ft_is_strdig(args[1]) ||
// 		!ft_is_strdig(args[2]) || !ft_is_strdig(args[3]) ||
// 		(total == 6 && !ft_is_strdig(args[4])))
// 	{
// 		ft_putstr("Error: value of an arguments must be numbers only\n");
// 		return (0);
// 	}
// 	if (ft_atoi(args[0]) <= 0 || ft_atoi(args[1]) <= 0 ||
// 		ft_atoi(args[2]) <= 0 || ft_atoi(args[3]) <= 0 || (total == 6 &&
// 			ft_atoi(args[4]) <= 0))
// 	{
// 		ft_putstr("Error: value of an argument is out of range\n");
// 		return (0);
// 	}
// 	return (1);
// }

void	free_simulation(t_philo *curr, int total)
{
	t_philo			*tmp;
	pthread_mutex_t	*mutex;
	pthread_mutex_t	*print;
	int				i;

	i = -1;
	mutex = curr->mutex;
	print = curr->print;
	curr->prev->next = NULL;
	pthread_mutex_destroy(print);
	while (++i < total)
		pthread_mutex_destroy(&mutex[i]);
	while (curr)
	{
		pthread_mutex_unlock(curr->mtphilo);
		pthread_mutex_lock(curr->mtphilo);
		curr->die = 1;
		pthread_mutex_unlock(curr->mtphilo);
		pthread_mutex_destroy(curr->mtphilo);
		pthread_detach(curr->thrd);
		pthread_detach(curr->checker);
		tmp = curr->next;
		ft_del(curr);
		curr = tmp;
	}
}

void	finish_simulation(t_table *table, int death)
{
	t_philo	*curr;
	t_philo	*tmp;
	int		i;

	curr = table->philos;
	free_simulation(curr, table->persons);
	ft_del(table);
	exit(0);
}



void	ft_get_fork(t_philo *philo)
{
	pthread_mutex_lock(&philo->mutex[philo->l_fork]);
	pthread_mutex_lock(&philo->mutex[philo->r_fork]);
	print_status(philo, FORK_ACTION);
	print_status(philo, FORK_ACTION);
}

void	ft_drop_fork(t_philo *philo)
{
	pthread_mutex_unlock(&philo->mutex[philo->r_fork]);
	pthread_mutex_unlock(&philo->mutex[philo->l_fork]);
}

void	ft_eat(t_philo *philo)
{
	philo->start = get_current_time(1, philo->start_time) + (philo->die_time * 1000);
	pthread_mutex_lock(philo->mtphilo);
	print_status(philo, EAT_ACTION);
	usleep(philo->eat_time * 1000);
	pthread_mutex_unlock(philo->mtphilo);
	if (philo->eat_num != -1)
		philo->eat_num--;
}

void	ft_sleep(t_philo *philo)
{
	print_status(philo, SLEEP_ACTION);
	usleep(philo->sleep_time * 1000);
	print_status(philo, THINK_ACTION);
}

t_philo		*init_philo(int name, t_philo *prev, char **args)
{
	t_philo			*philo;

	philo = malloc(sizeof(t_philo));
	philo->die_time = ft_atoi(args[1]);
	philo->eat_time = ft_atoi(args[2]);
	philo->sleep_time = ft_atoi(args[3]);
	philo->eat_num = args[4] ? ft_atoi(args[4]) : -1;
	philo->name = name;
	philo->mtphilo = malloc(sizeof(pthread_mutex_t));
	pthread_mutex_init(philo->mtphilo, NULL);
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

void		init_mutex(pthread_mutex_t *mutex, int total)
{
	int		i;

	i = -1;
	while (++i < total)
		pthread_mutex_init(&mutex[i], NULL);
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
	table->mtdie = malloc(sizeof(pthread_mutex_t));
	pthread_mutex_init(table->mtdie, NULL);
	return (table);
}

t_philo		*create_philos(int total, t_table *table, char **args)
{
	t_philo			*head;
	t_philo			*tmp;
	pthread_mutex_t	*mutex;
	pthread_mutex_t	*print;
	int				i;

	i = -1;
	head = NULL;
	mutex = malloc(sizeof(pthread_mutex_t) * total);
	print = malloc(sizeof(pthread_mutex_t));
	init_mutex(mutex, total);
	pthread_mutex_init(print, NULL);
	while (++i < total)
	{
		tmp = init_philo(i + 1, tmp, args);
		tmp->table = table;
		tmp->mutex = mutex;
		tmp->print = print;
		head = head ? head : tmp;
	}
	head->l_fork = tmp->r_fork;
	head->prev = tmp;
	tmp->next = head;
	head->head = 1;
	return (head);
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
	pthread_mutex_unlock(table->mtdie);
	return (NULL);
}

void	*ft_philo_checker(void *arg)
{
	t_philo *philo;

	philo = (t_philo*)arg;
	while (1)
	{
		pthread_mutex_lock(philo->mtphilo);
		if (philo->die == 0 && (philo->eat_num || philo->eat_num == -1) &&
			get_current_time(1, philo->start_time) > philo->start)
		{
			philo->die = 1;
			print_status(philo, DIE_ACTION);
			pthread_mutex_unlock(philo->mtphilo);
			pthread_mutex_unlock(philo->table->mtdie);
			break ;
		}
		pthread_mutex_unlock(philo->mtphilo);
		usleep(5);
	}
	return (NULL);
}

void	*ft_philo_life(void *arg)
{
	t_philo			*me;
	pthread_t		checker;

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
		}
	}
	return (NULL);
}

void	create_lifes(t_table *table)
{
	t_philo			*tmp;
	pthread_t		checker;
	struct timeval	current_time;

	tmp = table->philos;
	pthread_create(&checker, NULL, &game_checker, (void *)table);
	pthread_detach(checker);
	pthread_mutex_lock(table->mtdie);
	gettimeofday(&current_time, NULL);
	while (tmp)
	{
		tmp->start_time = current_time;
		pthread_create(&tmp->thrd, NULL, &ft_philo_life, (void *)tmp);
		usleep(45);
		tmp = tmp->next;
		if (tmp->head)
			break ;
	}
	pthread_mutex_lock(table->mtdie);
	pthread_mutex_unlock(table->mtdie);
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
