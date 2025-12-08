/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_printf_utils.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mariacos <mariacos@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/25 18:40:05 by mariacos          #+#    #+#             */
/*   Updated: 2025/11/09 16:13:01 by mariacos         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_printf.h"
#include "libft.h"

/* static int	ft_strlen(char *str)
{
	int	i;

	i = 0;
	while (str[i])
		i++;
	return (i);
} */

int	ft_putnbr(int nb)
{
	int	count;

	count = 0;
	if (nb == -2147483648)
		return (write(1, "-2147483648", 11));
	if (nb < 0)
	{
		count += ft_putchar('-'); 
		nb = -nb;
	}
	if (nb >= 10)
		count += ft_putnbr(nb / 10);
	count += ft_putchar((nb % 10) + '0');
	return (count);
}

int	ftprint_string(va_list *args)
{
	char	*string;
	int		len;

	string = va_arg(*args, char *);
	if (!string)
		string = "(null)";
	len = ft_strlen((const char *)string);
	write(1, string, len);
	return (len);
}

int	ftprint_unsigned_base(unsigned long n, char *base, unsigned int base_len)
{
	int					count;
	unsigned long		div;
	char				c;

	div = 1;
	count = 0;
	while (n / div >= base_len)
		div *= base_len;
	while (div > 0)
	{
		c = base[(n / div) % base_len];
		count += ft_putchar(c);
		div /= base_len;
	}
	return (count);
}

int	ftprint_unsigned(va_list *args, char format)
{
	int					count;
	unsigned long		n;
	char				*base;

	count = 0;
	if (format == 'p')
	{
		n = (unsigned long)va_arg(*args, void *);
		if (n == 0)
			return (write(1, "(nil)", 5));
		count += write(1, "0x", 2);
		base = "0123456789abcdef";
	}
	else
	{
		n = (unsigned int)va_arg(*args, unsigned int);
		if (format == 'u')
			base = "0123456789";
		else if (format == 'x')
			base = "0123456789abcdef";
		else
			base = "0123456789ABCDEF";
	}
	count += ftprint_unsigned_base(n, base, (unsigned int)ft_strlen((const char *)base));
	return (count);
}
