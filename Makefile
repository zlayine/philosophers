PHILO_ONE = philo_one

PHILO_TWO = philo_two

PHILO_THREE = philo_three

UTILS = utils/ft_atoi.c \
		utils/ft_del.c \
		utils/ft_putchar.c \
		utils/ft_putstr.c \
		utils/ft_itoa.c

OBJ = $(UTILS:.c=.o)

PHILO_ONE_MAIN = philo_one/philo.c
PHILO_ONE_HEAD = philo_one/philo.h

PHILO_TWO_MAIN = philo_two/philo.c
PHILO_TWO_HEAD = philo_two/philo.h

PHILO_THREE_MAIN = philo_three/philo.c
PHILO_THREE_HEAD = philo_three/philo.h

all: $(PHILO_ONE) $(PHILO_TWO)

$(PHILO_ONE): $(OBJ) $(PHILO_ONE_MAIN) $(PHILO_ONE_HEAD)
	@gcc $(PHILO_ONE_MAIN) $(OBJ) -o $(PHILO_ONE)
	@echo "SUCCESS! NO WWW ADDED"

$(PHILO_TWO): $(OBJ) $(PHILO_TWO_MAIN) $(PHILO_TWO_HEAD)
	@gcc $(PHILO_TWO_MAIN) $(OBJ) -o $(PHILO_TWO)
	@echo "SUCCESS! NO WWW ADDED"



%.o: %.c
	@gcc -c $< -o $@ -I philo.h

clean:
	@rm -rf $(OBJ)

fclean: clean
	# @rm -rf $(PHILO_ONE) $(PHILO_TWO)

re: fclean all
