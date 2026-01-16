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
CFILES          = main.c lexer_init.c data_init.c operational_loop_init.c parser_syntax.c ft_cd.c ft_pwd.c ft_exit.c
SRC             = $(addprefix $(SRC_DIR)/, $(CFILES))
OBJECTS         = $(addprefix $(OBJ_DIR)/, $(CFILES:.c=.o))

RM              = rm -rf

#   RULES
all: $(NAME)

$(NAME): $(LIBFT_LIB) $(OBJECTS)
	@echo "Linking $(NAME)..."
	$(CC) $(CFLAGS) $(OBJECTS) -lreadline $(LIBFT_LIB) -o $(NAME)
	@echo "✓ minishell built!"

# Compile libft
$(LIBFT_LIB):
	@echo "Compiling libft..."
	make -C $(LIBFT_PATH)

# Create obj/ and compile .o
$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c | $(OBJ_DIR)
	$(CC) $(CFLAGS) -I$(INC_DIR) -I$(LIBFT_PATH) -c $< -o $@

$(OBJ_DIR):
	mkdir -p $(OBJ_DIR)

clean:
	@echo "Cleaning object files..."
	$(RM) $(OBJ_DIR)
	make -C $(LIBFT_PATH) clean
	@echo "✓ clean done"

fclean: clean
	@echo "Removing executable and libraries..."
	$(RM) $(NAME)
	make -C $(LIBFT_PATH) fclean
	@echo "✓ fclean done"

re: fclean all

# Valgrind targets
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
