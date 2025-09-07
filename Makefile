NAME = execute 

CC = cc
CFLAGS = -g
# CFLAGS = -Wall -Wextra -Werror

SRC_DIR = ./src/
OBJ_DIR = ./objects/
HEADER_DIR = ./include/
LIBFT_DIR := ./libft/

HEADER = execute.h
LIBFT_NAME = libft.a

SRC = dummy.c \
	main.c \
	execute.c \
	execute_pipeline.c \
	execute_utils.c \
	command_utils.c
OBJECTS = $(SRC:.c=.o)

HEADERS = $(addprefix $(HEADER_DIR), $(HEADER))
SRCS = $(addprefix $(SRC_DIR), $(SRC))
OBJS = $(addprefix $(OBJ_DIR), $(OBJECTS))
LIBFT = $(addprefix $(LIBFT_DIR), $(LIBFT_NAME))

all: $(NAME)

$(NAME): $(LIBFT) $(OBJ_DIR) $(OBJS) $(HEADERS)
	$(CC) $(CFLAGS) $(OBJS) -o $(NAME)

$(OBJ_DIR)%.o:$(SRC_DIR)%.c | $(OBJ_DIR)
	$(CC) $(CFLAGS) -I $(HEADER_DIR) -c  $< -o $@

$(OBJ_DIR):
	mkdir $(OBJ_DIR)

$(LIBFT):
	$(MAKE) -C $(LIBFT_DIR)

run: $(NAME)
	@./$(NAME)

debug: $(NAME)
	@valgrind --leak-check=full --track-fds=yes ./$(NAME)

clean:
	rm -rf $(OBJ_DIR)
	$(MAKE) clean -C $(LIBFT_DIR)

fclean:
	rm -rf $(NAME)
	rm -rf $(OBJ_DIR)
	$(MAKE) fclean -C $(LIBFT_DIR)

diff:
	$(info The status of the repository, and the volume of per-file changes:)
	@git status
	@git diff --stat

re: fclean all

.PHONY: re fclean clean all
