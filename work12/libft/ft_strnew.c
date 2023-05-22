/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strnew.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ssoraka <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/04/16 20:42:08 by ssoraka           #+#    #+#             */
/*   Updated: 2019/04/17 17:21:49 by ssoraka          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strnew(size_t size)
{
	char	*str;

	str = NULL;
	if (size + 1 == 0)
		return (NULL);
	str = (char *)ft_memalloc(sizeof(char) * (size + 1));
	if (!str)
		return (NULL);
	return (str);
}
