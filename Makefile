NAME = execute 
TESTER_NAME = execute_tester

CC = cc
CFLAGS = -Wall -Wextra -Werror

SRC_DIR = ./src/
OBJ_DIR = ./objects/
HEADER_DIR = ./include/
LIBFT_DIR := ./libft/
TESTER_DIR := ./tester/

HEADER = execute.h
LIBFT_NAME = libft.a

SRC = main.c \
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
	$(CC) $(CFLAGS) $(OBJS) -L $(LIBFT_DIR) -lft -o $(NAME)


$(OBJ_DIR)%.o:$(SRC_DIR)%.c | $(OBJ_DIR)
	$(CC) $(CFLAGS) -I $(HEADER_DIR) -c  $< -o $@

$(OBJ_DIR):
	mkdir $(OBJ_DIR)

$(LIBFT):
	$(MAKE) -C $(LIBFT_DIR)

$(TESTER_NAME): $(TESTER_DIR)
		$(MAKE) -C $(TESTER_DIR)
	
$(TESTER_DIR):
	git clone git@github.com:Bank7656/bank_minishell_tester.git $(TESTER_DIR) 

run: $(NAME)
	@./$(NAME)

debug: $(NAME)
	@valgrind --leak-check=full --track-fds=yes ./$(NAME)

test: $(TESTER_NAME)
	@valgrind --leak-check=full --track-fds=yes $(TESTER_DIR)$(TESTER_NAME)

clean:
	rm -rf $(OBJ_DIR)
	rm -rf $(TESTER_DIR)
	$(MAKE) clean -C $(LIBFT_DIR)

fclean:
	rm -rf $(NAME)
	rm -rf $(OBJ_DIR)
	rm -rf $(TESTER_DIR)
	$(MAKE) fclean -C $(LIBFT_DIR)

diff:
	$(info The status of the repository, and the volume of per-file changes:)
	@git status
	@git --no-pager diff --stat

re: fclean all

.PHONY: re fclean clean run debug diff all
