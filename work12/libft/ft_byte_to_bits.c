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

char	*ft_byte_to_bits(char c)
{
	unsigned char	chr;
	char			*str;
	int				i;

	i = 0;
	str = ft_strnew(0);
	chr = (unsigned char)c;
	while (i < 8)
	{
		str[7 - i] = chr % 2 + '0';
		chr = chr >> 1;
		i++;
	}
	return (str);
}

char	*ft_bytes_to_bits(char *c, int n, char *separator)
{
	char	*str;
	char	*temp1;
	char	*temp2;
	int		i;

	i = 0;
	str = ft_strnew(0);
	while (i < n)
	{
		n--;
		temp2 = str;
		temp1 = ft_byte_to_bits(*(c + n));
		str = ft_strjoin(temp2, temp1);
		free(temp2);
		free(temp1);
		temp1 = str;
		if (i < n)
			str = ft_strjoin(str, separator);
		else
			str = ft_strjoin(str, "\n");
		free(temp1);
	}
	return (str);
}
