# **************************************************************************** #
#                          WHITE CHOCOLATE MOCHA SHELL                         #
# **************************************************************************** #

NAME		= minishell
CC			= cc
CFLAGS		= -Wall -Werror -Wextra -g

# Include paths
INC_DIR		= ./includes
INC_FLAGS	= -I$(INC_DIR) -I$(INC_DIR)/libft -I/usr/include

# Libraries
LIBFT_DIR	= $(INC_DIR)/libft
LIBFT_A		= $(LIBFT_DIR)/libft.a
LIBS		= -lreadline -lncurses

# Source files
SRC_DIR		= ./src
SRCS		= $(SRC_DIR)/main.c \
		 	  $(SRC_DIR)/lexer/lexer.c \
		 	  $(SRC_DIR)/lexer/lexer_utils.c \
			  $(SRC_DIR)/lexer/lexer_token.c \
		 	  $(SRC_DIR)/lexer/lexer_operators.c\
		 	  $(SRC_DIR)/garbage_collector/gc_utils.c \
		 	  $(SRC_DIR)/parser/parser.c \
		 	  $(SRC_DIR)/parser/parser_command.c \
		 	  $(SRC_DIR)/parser/parser_redir.c \
		 	  $(SRC_DIR)/parser/parser_utils.c \
		 	  $(SRC_DIR)/parser/parser_validate.c \
		 	  $(SRC_DIR)/enviroment/enviroment.c \
		 	  $(SRC_DIR)/enviroment/enviroment_utils.c \
			  $(SRC_DIR)/expander/expander.c\
			  $(SRC_DIR)/errors/errors.c\
			  $(SRC_DIR)/builtins/builtin_env.c \
			  $(SRC_DIR)/builtins/builtin_echo.c \
			  $(SRC_DIR)/builtins/builtin_exit.c \
			  $(SRC_DIR)/builtins/builtin_unset.c \
			  $(SRC_DIR)/builtins/builtin_export.c \
			  $(SRC_DIR)/builtins/builtin_cd.c \
			  $(SRC_DIR)/builtins/builtin_pwd.c \
			  $(SRC_DIR)/builtins/builtin_dispatch.c \
			  $(SRC_DIR)/builtins/variable.c


# Object files
OBJ_DIR		= ./obj
OBJS		= $(SRCS:%.c=$(OBJ_DIR)/%.o)

# Colors
GREEN		= \033[0;32m
YELLOW		= \033[0;33m
RESET		= \033[0m

# Rules
all: $(NAME)

$(NAME): $(LIBFT_A) $(OBJS)
	@echo "$(GREEN)🔗 Linking $(NAME)...$(RESET)"
	@$(CC) $(CFLAGS) $(OBJS) $(LIBFT_A) $(LIBS) -o $(NAME)
	@echo "$(GREEN)✅ $(NAME) is ready!$(RESET)"

$(OBJ_DIR)/%.o: %.c
	@mkdir -p $(dir $@)
	@echo "$(YELLOW)📦 Compiling $<...$(RESET)"
	@$(CC) $(CFLAGS) $(INC_FLAGS) -c $< -o $@

$(LIBFT_A):
	@echo "$(YELLOW)📚 Building libft...$(RESET)"
	@make -C $(LIBFT_DIR)

clean:
	@rm -rf $(OBJ_DIR)
	@make clean -C $(LIBFT_DIR)
	@echo "$(YELLOW)🧹 Object files cleaned$(RESET)"

fclean: clean
	@rm -rf $(NAME)
	@make fclean -C $(LIBFT_DIR)
	@echo "$(YELLOW)🗑️  $(NAME) removed$(RESET)"

re: fclean all


# Test
TEST_SRCS   = $(filter-out $(SRC_DIR)/main.c, $(SRCS)) \
              ./test.c
TEST_OBJS   = $(TEST_SRCS:%.c=$(OBJ_DIR)/%.o)
TEST_NAME   = minishell_test

test: $(LIBFT_A) $(TEST_OBJS)
	@echo "$(GREEN)🔗 Linking $(TEST_NAME)...$(RESET)"
	@$(CC) $(CFLAGS) $(TEST_OBJS) $(LIBFT_A) $(LIBS) -o $(TEST_NAME)
	@echo "$(GREEN)✅ $(TEST_NAME) is ready!$(RESET)"

tclean:
	@rm -f $(TEST_NAME)
	@echo "$(YELLOW)🧹 Test binary cleaned$(RESET)"

.PHONY: all clean fclean re