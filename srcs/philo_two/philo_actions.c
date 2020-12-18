/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_actions.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zlayine <zlayine@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/12/15 16:51:11 by zlayine           #+#    #+#             */
/*   Updated: 2020/12/18 14:29:08 by zlayine          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void	ft_get_fork(t_philo *philo)
{
	sem_wait(philo->sem);
	print_status(philo, FORK_ACTION);
	sem_wait(philo->sem);
	print_status(philo, FORK_ACTION);
}

void	ft_eat(t_philo *philo)
{
	sem_wait(philo->mtphilo);
	philo->die = -2;
	philo->start = get_time();
	philo->death_time = philo->start + philo->die_time;
	print_status(philo, EAT_ACTION);
	usleep(philo->eat_time * 1000);
	if (philo->eat_num != -1)
		philo->eat_num--;
	philo->die = 0;
	sem_post(philo->mtphilo);
}

void	ft_finish_eat(t_philo *philo)
{
	print_status(philo, SLEEP_ACTION);
	sem_post(philo->sem);
	sem_post(philo->sem);
	usleep(philo->sleep_time * 1000);
	print_status(philo, THINK_ACTION);
}
