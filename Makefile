CC = cc
CFLAGS = -g
# CFLAGS = -Wall -Wextra -Werror

HEADER = execute.h
HEADER_DIR = ./

SRC = dummy.c \
	main.c \
	execute.c \
	execute_utils.c \

SRC_OBJS = $(SRC:.c=.o)

OBJ_DIR = ./objects/
OBJS = $(addprefix $(OBJ_DIR), $(SRC_OBJS))

NAME = execute 

all: $(NAME)

$(NAME): $(LIBFT) $(OBJ_DIR) $(OBJS) $(HEADER)
	$(CC) $(CFLAGS) $(OBJS) -o $(NAME)

$(OBJ_DIR)%.o:%.c
	$(CC) $(CFLAGS) -I $(HEADER_DIR) -c  $< -o $@

$(OBJ_DIR):
	mkdir $(OBJ_DIR)

run: $(NAME)
	@./$(NAME)

debug: $(NAME)
	valgrind --leak-check=full --track-fds=yes ./$(NAME)

clean:
	rm -rf $(OBJ_DIR)

fclean:
	rm -rf $(NAME)
	rm -rf $(OBJ_DIR)

re: fclean all

.PHONY: re fclean clean all