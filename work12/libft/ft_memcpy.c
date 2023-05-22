/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memcpy.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ssoraka <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/04/14 11:10:14 by ssoraka           #+#    #+#             */
/*   Updated: 2019/04/14 11:29:37 by ssoraka          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	*ft_memcpy(void *dst, const void *src, size_t n)
{
	size_t		i;
	char		*dst2;
	const char	*src2;

	i = 0;
	dst2 = dst;
	src2 = src;
	if (dst == src)
		return (dst);
	while (i < n)
	{
		dst2[i] = src2[i];
		i++;
	}
	return (dst);
}

void	*ft_memcpy8(void *dst, const void *src, size_t n)
{
	size_t		i;
	size_t		n2;
	long		*dst2;
	const long	*src2;

	if (dst == src)
		return (dst);
	n2 = n / 8;
	dst2 = dst;
	src2 = src;
	i = 0;
	while (i < n2)
	{
		dst2[i] = src2[i];
		i++;
	}
	if (n > n2 * 8)
		ft_memcpy((void *)(dst2 + n2), (void *)(src2 + n2), n % 8);
	return (dst);
}
