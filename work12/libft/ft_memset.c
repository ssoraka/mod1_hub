/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memset.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ssoraka <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/04/14 09:52:04 by ssoraka           #+#    #+#             */
/*   Updated: 2019/04/15 20:25:00 by ssoraka          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	*ft_memset(void *dest, int c, size_t n)
{
	size_t			i;
	unsigned char	chr;
	char			*str;

	i = 0;
	chr = (unsigned char)c;
	str = dest;
	while (i < n)
		str[i++] = chr;
	return (dest);
}

void	*ft_memset8(void *dest, int c, size_t n)
{
	size_t			i;
	size_t			n2;
	unsigned long	chr;
	unsigned long	*str;

	i = 0;
	n2 = n / 8;
	chr = (unsigned int)c;
	chr = (chr << 32) | chr;
	str = dest;
	while (i < n2)
		str[i++] = chr;
	if (n > n2 * 8)
		ft_memset((void *)(str + n2), c, n % 8);
	return (dest);
}
