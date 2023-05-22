/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_atoi.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ssoraka <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/03/04 16:42:49 by ssoraka           #+#    #+#             */
/*   Updated: 2019/04/15 20:25:23 by ssoraka          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

int	ft_atoi(const char *str)
{
	size_t	i;
	size_t	nb;
	int		znak;

	nb = 00;
	i = 0;
	znak = 1;
	while (str[i] != '\0' && (str[i] == ' ' || (str[i] >= 9 && str[i] <= 13)))
		i++;
	if (str[i] == '-' || str[i] == '+')
	{
		if (str[i] == '-')
			znak = -1;
		i++;
	}
	while (str[i] != '\0' && str[i] >= '0' && str[i] <= '9')
	{
		if (nb > 922337203685477580)
			return ((1 + znak) / (-2));
		if (nb == 922337203685477580 && str[i] - 48 > 8 + (1 + znak) / (-2))
			return ((1 + znak) / (-2));
		nb = 10 * nb + (str[i] - 48);
		i++;
	}
	return (znak * nb);
}

int	get_digit(char c, int base)
{
	int	num;

	if (c >= '0' && c <= '9')
		num = c - '0';
	else if (c >= 'a' && c <= 'f')
		num = c - 'a' + 10;
	else if (c >= 'A' && c <= 'F')
		num = c - 'A' + 10;
	else
		num = base;
	if (num < base)
		return (num);
	else
		return (-1);
}

int	ft_atoi_base(const char *str, int base)
{
	int	result;
	int	sign;
	int	digit;

	result = 0;
	if (!str || base <= 0)
		return (result);
	while (*str && ft_isspace(*str))
		str++;
	sign = 1;
	if (*str == '-' && base == 10)
	{
		sign = -1;
		++str;
	}
	while (*str)
	{
		digit = get_digit(*str, base);
		if (digit == -1)
			break ;
		result = result * base;
		result = result + (digit * sign);
		str++;
	}
	return (result);
}
