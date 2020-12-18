/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zlayine <zlayine@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/12/15 14:39:14 by zlayine           #+#    #+#             */
/*   Updated: 2020/12/18 10:56:59 by zlayine          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void	print_status(t_philo *philo, int action)
{
	char	*tmp;

	action != DIE_ACTION ? sem_wait(philo->print) : 0;
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
	action != DIE_ACTION ? sem_post(philo->print) : 0;
}

// long	get_current_time(int micro, struct timeval start_time)
// {
// 	struct timeval	current_time;
// 	int				time;

// 	gettimeofday(&current_time, NULL);
// 	time = ((current_time.tv_sec - start_time.tv_sec) * 1000000L
// 		+ current_time.tv_usec) - start_time.tv_usec;
// 	if (!micro)
// 		return (time / 1000);
// 	else
// 		return (time);
// }

void	life_creation_end(t_table *table, pid_t *pids)
{
	int			i;
	pthread_t	checker;

	pthread_create(&checker, NULL, &game_checker, (void *)table);
	table->checker = checker;
	sem_wait(table->game);
	table->end = !table->end ? 1 : table->end;
	i = -1;
	while (++i < table->persons)
		kill(pids[i], 0);
	finish_simulation(table, 2);
	if (table->end == 1)
		finish_simulation(table, 1);
	else
		pthread_join(checker, NULL);
}

void	create_lifes(t_table *table)
{
	int				i;
	int				pid;
	t_philo			*tmp;
	struct timeval	current_time;
	pid_t			pids[table->persons];

	i = 0;
	tmp = table->philos;
	sem_wait(table->game);
	gettimeofday(&current_time, NULL);
	while (tmp)
	{
		tmp->start_time = current_time;
		pid = ft_philo_life(tmp);
		if (pid < 0)
			exit(1);
		tmp = tmp->next;
		pids[i++] = pid;
		if (tmp->head)
			break ;
	}
	life_creation_end(table, pids);
}

int		main(int argc, char **argv)
{
	t_table	*table;

	if (!valid_args(argc, ++argv))
		return (1);
	table = init_table(argv);
	create_lifes(table);
	return (0);
}
