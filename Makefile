# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: mariacos <mariacos@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2025/10/27 16:31:27 by mariacos          #+#    #+#              #
#    Updated: 2025/12/06 17:22:56 by mariacos         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

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

# Source files (you currently have only main.c)
CFILES          = main.c
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

.PHONY: all clean fclean re
