#include <stdio.h>
#include <unistd.h>
#include <sys/time.h>
#include <pthread.h> 
#include <string.h> 
#include <stdlib.h> 

typedef struct	s_table
{
	int				persons;
	int				forks;
	struct s_philo	*philos;
	pthread_t		*tid;
}				t_table;

typedef struct	s_philo
{
	int				status;
	int				die_time;
	int				eat_time;
	int				sleep_time;
	int				eat_num;
	int				name;
	int				head;
	pthread_t		tid;
	struct s_philo	*next;
	struct s_philo	*prev;
}				t_philo;

# define FORK_ACTION 1
# define EAT_ACTION 2
# define SLEEP_ACTION 3
# define THINK_ACTION 4
# define DIE_ACTION 5

int					g_forks;
pthread_mutex_t		g_lock;