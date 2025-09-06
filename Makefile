CC = cc
CFLAGS = -g
# CFLAGS = -Wall -Wextra -Werror

HEADER = execute.h
HEADER_DIR = ./include/
HEADERS = $(addprefix $(HEADER_DIR), $(HEADER))

SRC = dummy.c \
	main.c \
	execute.c \
	execute_pipeline.c \
	execute_utils.c \
	command_utils.c

SRC_DIR = ./src/
SRCS = $(addprefix $(SRC_DIR), $(SRC))

SRC_OBJS = $(SRC:.c=.o)

OBJ_DIR = ./objects/
OBJS = $(addprefix $(OBJ_DIR), $(SRC_OBJS))

LIBFT_NAME := libft.a
LIBFT_DIR := ./libft/
LIBFT = $(addprefix $(LIBFT_DIR), $(LIBFT_NAME))

NAME = execute 

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
	valgrind --leak-check=full --track-fds=yes ./$(NAME)

clean:
	rm -rf $(OBJ_DIR)
	$(MAKE) clean -C $(LIBFT_DIR)

fclean:
	rm -rf $(NAME)
	rm -rf $(OBJ_DIR)
	$(MAKE) fclean -C $(LIBFT_DIR)

re: fclean all

.PHONY: re fclean clean all
