/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_printf.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mariacos <mariacos@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/24 14:42:57 by mariacos          #+#    #+#             */
/*   Updated: 2025/11/09 16:20:05 by mariacos         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_printf.h"

int	ft_putchar(char c)
{
	return (write(1, &c, 1));
}

int	check_format(const char format, va_list *args)
{
	if (format == 'c')
		return (ft_putchar(va_arg(*args, int)));
	else if (format == 's')
		return (ftprint_string(args));
	else if ((format == 'i') || (format == 'd'))
		return (ft_putnbr(va_arg(*args, int)));
	else if (format == 'u' || format == 'x' || format == 'X' || format == 'p')
		return (ftprint_unsigned(args, format));
	else if (format == '%')
		return (ft_putchar('%'));
	return (0);
}

int	ft_printf(const char *str, ...)
{
	va_list	args;
	int		i;
	int		printed_chars;

	i = 0;
	printed_chars = 0;
	va_start(args, str);
	if (!str)
		return (-1);
	while (str[i])
	{
		if (str[i] == '%' && str[i + 1])
		{
			i++;
			printed_chars += check_format(str[i], &args);
		}
		else if (str[i] == '%' && !str[i + 1])
			return (-1);
		else
			printed_chars += ft_putchar(str[i]);
		i++;
	}
	va_end(args);
	return (printed_chars);
}
