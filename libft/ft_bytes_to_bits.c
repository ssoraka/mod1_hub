/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_byte_to_bits.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ssoraka <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/04/28 17:24:20 by ssoraka           #+#    #+#             */
/*   Updated: 2019/04/28 17:24:50 by ssoraka          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

/*
**	функция возвращает содержимое переменной var
**	в виде строки битов для вывода
**	ft_putendl(ft_bytes_to_bits((void *)(&var), sizeof(var), "__"));
**	первый аргумент - указатель на 1-ый байт
**	второй - количество байт
**	третий - строка для отображения разделителя между байтами
*/

static int	ft_byte_to_bits(char *str, char c)
{
	unsigned char	chr;
	int				i;

	i = 0;
	if (str == NULL)
		return (FAIL);
	chr = (unsigned char)c;
	while (i < 8)
	{
		str[7 - i] = chr % 2 + '0';
		chr = chr >> 1;
		i++;
	}
	return (SUCCESS);
}

char		*ft_bytes_to_bits(void *ptr, int n, char *separator)
{
	char	*str;
	char	*c;
	int		byte;
	int		i;

	i = 0;
	c = (char *)ptr;
	if (c == NULL || separator == NULL || n == 0)
		return (NULL);
	byte = 8 + ft_strlen(separator);
	str = ft_strnew(n * 8 + (n - 1) * ft_strlen(separator));
	while (n > 0 && str)
	{
		if (ft_byte_to_bits(str + i, *(c + n - 1)) == FAIL)
		{
			free(str);
			return (NULL);
		}
		if (n > 1)
			str = ft_strcat(str, separator);
		i = i + byte;
		n--;
	}
	return (str);
}
