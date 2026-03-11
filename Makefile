NAME            = minishell
CC              = cc
CFLAGS          = -Wall -Wextra -Werror -g

# Directories
SRC_DIR         = src
OBJ_DIR         = obj
INC_DIR         = include

# Libft
LIBFT_PATH      = libft/src
LIBFT_LIB       = $(LIBFT_PATH)/libft.a

# Valgrind suppression file
SUPP_FILE       = readline.supp

# Source files
CFILES = main.c \
         initialize/operational_loop_init.c \
		 initialize/operational_loop_utils.c \
		 initialize/data_init.c \
         signals/signals.c \
		 execution/builtins_execution.c \
		 execution/env_utils.c \
		 execution/error_utils.c \
         execution/execution.c \
		 execution/heredoc_handling.c \
		 execution/heredoc_utils.c \
		 execution/heredoc_utils_two.c \
		 execution/path_utils.c \
		 execution/path_utils_two.c \
         execution/pipeline_execution.c \
         execution/pipeline_utils.c \
         execution/pipeline_utils_two.c \
         builtins/ft_cd.c \
         builtins/ft_echo.c \
         builtins/ft_env.c \
         builtins/ft_exit.c \
         builtins/ft_export.c \
         builtins/ft_export_utils.c \
         builtins/ft_unset.c \
         builtins/ft_pwd.c \
         lexer/tokenize.c \
         lexer/tokenize_utils.c \
         lexer/parser_syntax.c \
         parser/struct_init.c \
         parser/create_struct.c \
         parser/struct_handlers.c \
         expansion/struct_expansion.c \
		 expansion/struct_expansion_two.c \
         expansion/struct_expansion_utils.c \
         expansion/struct_hdoc.c \
         utils/free_data.c \
		 utils/free_data_two.c \
         utils/helper.c \
         utils/ft_split_upgrated.c

SRC             = $(addprefix $(SRC_DIR)/, $(CFILES))
OBJECTS         = $(addprefix $(OBJ_DIR)/, $(CFILES:.c=.o))

RM              = rm -rf

#   RULES
all: $(NAME)

$(NAME): $(LIBFT_LIB) $(OBJECTS)
	@echo "Linking $(NAME)..."
	@$(CC) $(CFLAGS) $(OBJECTS) -lreadline $(LIBFT_LIB) -o $(NAME)
	@for i in 1 2 3 4 5 6 7; do \
		clear; \
		printf "\033[32m\n%*s    .----.   @   @\n" $$(($$i*2)) ""; \
		printf "%*s   / .-\"-.'.  \\v/\n" $$(($$i*2)) ""; \
		printf "%*s   | | '\\ \\ \\_/ )\n" $$(($$i*2)) ""; \
		printf "%*s ,-\\ \`-.' /.'  /\n" $$(($$i*2)) ""; \
		printf "%*s'---'----'----'\n\033[0m" $$(($$i*2)) ""; \
		sleep 0.1; \
	done
	@echo "\033[33m\n\t  \"Welcome to my \$$HOME!\" \n\033[0m"

$(LIBFT_LIB):
	@make -C $(LIBFT_PATH)

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c
	@mkdir -p $(dir $@)
	@$(CC) $(CFLAGS) -I$(INC_DIR) -I$(LIBFT_PATH) -c $< -o $@

clean:
	@echo "Cleaning object files..."
	@$(RM) $(OBJ_DIR)
	@make -C $(LIBFT_PATH) clean
	@echo "✓ clean done"

fclean: clean
	@for i in 10 9 8 7 6 5 4 3; do \
		clear; \
		printf "\033[32m\n%*s  @   @   .----.\n" $$(($$i*2)) ""; \
		printf "%*s   \\v/  .\`.- \" -. \n" $$(($$i*2)) ""; \
		printf "%*s    ( \\_/ / / / | |\n" $$(($$i*2)) ""; \
		printf "%*s     \\  .\`\\ /.\` /-,\n" $$(($$i*2)) ""; \
		printf "%*s      '----'----'---'\n\033[0m" $$(($$i*2)) ""; \
		sleep 0.1; \
	done
	@echo "\033[33m\n\t  \"Cleaning everything... bye bye!\" \n\033[0m"
	@$(RM) $(NAME)
	@$(RM) $(OBJ_DIR)
	@make -C $(LIBFT_PATH) fclean

re: fclean all

valgrind: $(NAME)
	valgrind --leak-check=full \
             --show-leak-kinds=all \
             --suppressions=$(SUPP_FILE) \
             --track-fds=yes \
             --trace-children=yes \
             ./$(NAME)

valgrind_full: $(NAME)
	valgrind --leak-check=full \
             --show-leak-kinds=all \
             --verbose \
             --suppressions=$(SUPP_FILE) \
             ./$(NAME)

.PHONY: all clean fclean re valgrind valgrind_full
