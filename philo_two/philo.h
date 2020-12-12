#ifndef PHILO_H
# define PHILO_H

#include <stdio.h>
#include <unistd.h>
#include <sys/time.h>
#include <pthread.h> 
#include <string.h> 
#include <stdlib.h> 
#include <semaphore.h> 


typedef struct	s_table
{
	int				persons;
	int				forks;
	struct s_philo	*philos;
	pthread_t		*tid;
}				t_table;

typedef struct	s_fork
{
	int			state;
}				t_fork;

typedef struct	s_philo
{
	int				action;
	int				die_time;
	int				eat_time;
	int				sleep_time;
	int				eat_num;
	int				name;
	int				head;
	int				start;
	int				die;
	pthread_t		thrd;
	struct s_table	*table;
	struct s_philo	*next;
	struct s_philo	*prev;
	struct s_table	*table;
	int				l_fork;
	int				r_fork;
	sem_t			*print;
	sem_t			*sem;
}				t_philo;

# define FORK_ACTION 1
# define EAT_ACTION 2
# define SLEEP_ACTION 3
# define THINK_ACTION 4
# define DIE_ACTION 5


int			ft_atoi(const char *str);
void		ft_putstr(char *s);
void		ft_putchar(char c);
void		ft_del(void *data);
void		print_status(t_philo *philo, int action);
long		get_current_time(int micro);
int			ft_do_action(t_philo *philo);
int			ft_action(t_philo *philo);
void		check_life(t_philo *philo);
void		*ft_philo_life(void *arg);
t_philo		*init_philo(int name, t_philo *prev, char **args);
t_philo		*create_philos(int total, t_table *table, char **args);
t_table		*init_table(char **args);
void		create_lifes(t_table *table);
void		finish_simulation(t_table *table);

#endif