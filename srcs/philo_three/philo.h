/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zlayine <zlayine@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/12/15 14:32:06 by zlayine           #+#    #+#             */
/*   Updated: 2020/12/18 14:40:12 by zlayine          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILO_H
# define PHILO_H

# include <stdio.h>
# include <unistd.h>
# include <sys/time.h>
# include <pthread.h>
# include <string.h>
# include <stdlib.h>
# include <semaphore.h>
# include <dispatch/dispatch.h>
# include <signal.h>

typedef struct	s_table
{
	int				persons;
	int				forks;
	int				end;
	sem_t			*mtdie;
	pthread_t		checker;
	struct s_philo	*philos;
}				t_table;

typedef struct	s_philo
{
	int				die_time;
	int				eat_time;
	int				sleep_time;
	int				eat_num;
	int				name;
	int				head;
	long			start;
	int				die;
	sem_t			*done;
	sem_t			*print;
	sem_t			*sem;
	sem_t			*mtphilo;
	pthread_t		checker;
	struct timeval	start_time;
	struct s_philo	*next;
	struct s_philo	*prev;
	struct s_table	*table;
}				t_philo;

# define FORK_ACTION 1
# define EAT_ACTION 2
# define SLEEP_ACTION 3
# define THINK_ACTION 4
# define DIE_ACTION 5
# define SIM_OVER 6

int				ft_atoi(const char *str);
void			ft_putstr(char *s);
char			*ft_itoa(long n);
void			ft_putchar(char c);
void			ft_del(void *data);
int				ft_is_strdig(char *str);
char			*ft_strjoin(char const *s1, char const *s2);
size_t			ft_strlen(const char *str);
void			ft_putnbr(long long d);
void			print_status(t_philo *philo, int action);
long			get_current_time(int micro, struct timeval start_time);
int				ft_do_action(t_philo *philo);
int				ft_action(t_philo *philo);
void			check_life(t_philo *philo);
int				ft_philo_life(t_philo *philo);
t_philo			*init_philo(int name, t_philo *prev, char **args);
t_philo			*create_philos(int total, t_table *table, char **args);
t_table			*init_table(char **args);
void			create_lifes(t_table *table);
void			finish_simulation(t_table *table, int death);
void			ft_sleep(t_philo *philo);
void			ft_eat(t_philo *philo);
void			ft_drop_fork(t_philo *philo);
void			ft_get_fork(t_philo *philo);
sem_t			*init_semaphore(int total, char *name);
void			*game_checker(void *arg);
void			free_simulation(t_philo *curr);
void			life_creation_end(t_table *table, pid_t *pids);
t_table			*init_table(char **args);
void			create_lifes(t_table *table);
int				valid_args(int total, char **args);

#endif
