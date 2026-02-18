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
CFILES          = main.c operational_loop_init.c \
                  lexer/tokenize.c lexer/tokenize_utils.c lexer/parser_syntax.c \
                  parser/struct_init.c parser/struct_init_two.c parser/struct_handlers.c \
                  expansion/struct_expansion.c expansion/struct_expansion_utils.c expansion/struct_hdoc.c \
                  utils/data_init.c utils/free_data.c utils/helper.c utils/ft_split_upgrated.c \
                  ft_cd.c ft_pwd.c ft_exit.c ft_echo.c 

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
	@echo "\033[33m\n\t\t  \"Welcome to my \$$HOME!\" \n\033[0m"

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
	@echo "\033[33m\n\t\t  \"Cleaning everything... bye bye!\" \n\033[0m"
	@$(RM) $(NAME)
	@$(RM) $(OBJ_DIR)
	@make -C $(LIBFT_PATH) fclean

re: fclean all

valgrind: $(NAME)
	valgrind --leak-check=full \
             --show-leak-kinds=all \
             --suppressions=$(SUPP_FILE) \
             ./$(NAME)

valgrind_full: $(NAME)
	valgrind --leak-check=full \
             --show-leak-kinds=all \
             --track-origins=yes \
             --verbose \
             --suppressions=$(SUPP_FILE) \
             ./$(NAME)

.PHONY: all clean fclean re valgrind valgrind_full