PHILO_ONE = philo_one

PHILO_TWO = philo_two

PHILO_THREE = philo_three

SRCS_ONE = srcs/philo_one/inits.c \
	srcs/philo_one/finish.c \
	srcs/philo_one/philo_actions.c \
	srcs/philo_one/philo_checker.c

SRCS_TWO = srcs/philo_two/inits.c \
	srcs/philo_two/finish.c \
	srcs/philo_two/philo_actions.c \
	srcs/philo_two/philo_checker.c

SRCS_THREE = srcs/philo_three/inits.c \
	srcs/philo_three/philo_actions.c \
	srcs/philo_three/finish.c \
	srcs/philo_three/philo_checker.c

UTILS = utils/ft_atoi.c \
		utils/ft_del.c \
		utils/ft_putchar.c \
		utils/ft_putstr.c \
		utils/ft_isdigit_str.c \
		utils/ft_strlen.c \
		utils/ft_strjoin.c \
		utils/tools.c \
		utils/ft_putnbr.c \
		utils/ft_itoa.c

OBJ = $(UTILS:.c=.o)

PHILO_ONE_MAIN = srcs/philo_one/philo.c
PHILO_ONE_HEAD = srcs/philo_one/philo.h

PHILO_TWO_MAIN = srcs/philo_two/philo.c
PHILO_TWO_HEAD = srcs/philo_two/philo.h

PHILO_THREE_MAIN = srcs/philo_three/full.c
PHILO_THREE_HEAD = srcs/philo_three/philo.h

all: $(PHILO_ONE) $(PHILO_TWO) $(PHILO_THREE)

$(PHILO_ONE): $(OBJ) $(SRCS_ONE) $(PHILO_ONE_MAIN) $(PHILO_ONE_HEAD)
	@gcc $(PHILO_ONE_MAIN) $(SRCS_ONE) $(OBJ) -o $(PHILO_ONE)
	@echo "PHILO ONE SUCCESS!"

$(PHILO_TWO): $(OBJ) $(SRCS_TWO) $(PHILO_TWO_MAIN) $(PHILO_TWO_HEAD)
	@gcc $(PHILO_TWO_MAIN) $(SRCS_TWO) $(OBJ) -o $(PHILO_TWO)
	@echo "PHILO TWO SUCCESS!"


$(PHILO_THREE): $(OBJ) $(SRCS_THREE) $(PHILO_THREE_MAIN) $(PHILO_THREE_HEAD)
	# @gcc $(PHILO_THREE_MAIN) $(SRCS_THREE) $(OBJ) -o $(PHILO_THREE)
	@gcc $(PHILO_THREE_MAIN) $(OBJ) -o $(PHILO_THREE)
	@echo "PHILO THREE SUCCESS!"

%.o: %.c
	@gcc -c -Wall -Wextra -Werror $< -o $@ -I philo.h

clean:
	@rm -rf $(OBJ)

fclean: clean
	@rm -rf $(PHILO_ONE) $(PHILO_TWO) $(PHILO_THREE)

re: fclean all

