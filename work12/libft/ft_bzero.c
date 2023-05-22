/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_bzero.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ssoraka <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/04/14 10:50:40 by ssoraka           #+#    #+#             */
/*   Updated: 2019/04/14 11:08:34 by ssoraka          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	ft_bzero(void *s, size_t n)
{
	size_t	i;
	char	*str;

	i = 0;
	str = s;
	while (i < n)
		str[i++] = '\0';
}

void	ft_bzero8(void *s, size_t n)
{
	size_t	i;
	size_t	n2;
	long	*str;

	i = 0;
	n2 = n / 8;
	str = s;
	while (i < n2)
		str[i++] = '\0';
	if (n > n2 * 8)
		ft_bzero((void *)(str + n2), n % 8);
}
