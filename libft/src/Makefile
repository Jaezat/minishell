# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: mariacos <mariacos@student.42lisboa.com    +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2025/04/18 09:47:33 by mariacos          #+#    #+#              #
#    Updated: 2026/01/30 18:53:20 by mariacos         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME		=	libft.a
SRC_FILES	=	ft_atoi ft_bzero ft_calloc ft_isalnum ft_isalpha ft_isascii ft_isdigit ft_isprint\
			ft_itoa ft_memchr ft_memcmp ft_memcpy ft_memmove ft_memset ft_putchar_fd ft_putendl_fd\
			ft_putnbr_fd ft_putstr_fd ft_split ft_strchr ft_strdup ft_striteri ft_strjoin ft_strlcat\
			ft_strlcpy ft_strlen ft_strmapi ft_strcmp ft_strncmp ft_strnstr ft_strrchr ft_strtrim ft_substr\
			ft_tolower ft_toupper 
EXTRA_FILES	=	get_next_line free_2d_array ft_printf ft_printf_utils ft_print_2d_array ft_size_2d_array\

OBJ_DIR		=	obj/
INC_DIR		=	../include/

SRC			=	$(addsuffix .c, $(SRC_FILES))
EXTRA_SRC	=	$(addsuffix .c, $(EXTRA_FILES))

OBJ			=	$(addprefix $(OBJ_DIR), $(addsuffix .o, $(SRC_FILES)))
EXTRA_OBJ	=	$(addprefix $(OBJ_DIR), $(addsuffix .o, $(EXTRA_FILES)))

CC			=	cc
CFLAGS		=	-Wall -Wextra -Werror -I$(INC_DIR)
LIB			=	ar rcs
RM			=	rm -rf

# ===== RULES =====
all: $(NAME)

# Build libft.a including both SRC and EXTRA_SRC
$(NAME): $(OBJ) $(EXTRA_OBJ)
	@$(LIB) $(NAME) $(OBJ) $(EXTRA_OBJ)
	@echo "libft built including extras."

# Compile object files
$(OBJ_DIR)%.o: %.c
	@mkdir -p $(OBJ_DIR)
	@$(CC) $(CFLAGS) -c $< -o $@

clean:
	@echo "Cleaning object files..."
	@$(RM) $(OBJ_DIR)

fclean: clean
	@echo "Cleaning library..."
	@$(RM) $(NAME)

re: fclean all

.PHONY: all clean fclean re
