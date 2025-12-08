/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_printf.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mariacos <mariacos@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/25 18:40:48 by mariacos          #+#    #+#             */
/*   Updated: 2025/11/09 16:20:09 by mariacos         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef FT_PRINTF_H
# define FT_PRINTF_H

# include <stdarg.h>
# include <unistd.h>

// int	ft_strlen(char *str);
int	ft_putnbr(int nb);
int	ftprint_string(va_list *args);
int	ftprint_unsigned_base(unsigned long n, char *base, unsigned int base_len);
int	ftprint_unsigned(va_list *args, char format);
int	ft_putchar(char c);
int	check_format(const char format, va_list *args);
int	ft_printf(const char *str, ...);

#endif
