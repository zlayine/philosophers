NAME = philo_two

SRC = utils/ft_atoi.c \
		utils/ft_del.c \
		utils/ft_putchar.c \
		utils/ft_putstr.c \
		utils/ft_itoa.c

OBJ = $(SRC:.c=.o)

MAIN = philo.c

all: $(NAME)

$(NAME): $(OBJ) $(MAIN)  philo.h
	@gcc -lpthread -lrt $(MAIN) $(OBJ) -o $(NAME)
	@echo "SUCCESS! NO WWW ADDED"

%.o: %.c
	@gcc -c $< -o $@ -I philo.h

clean:
	@rm -rf $(OBJ)

fclean: clean
	@rm -rf $(NAME)

re: fclean all