/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_mem_arr_new.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ssoraka <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/04/26 10:25:25 by ssoraka           #+#    #+#             */
/*   Updated: 2019/04/26 10:25:25 by ssoraka          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	**ft_mem_arr_new(int row, int column, int size)
{
	int		i;
	void	**tab;

	tab = (void **)ft_memalloc(sizeof(void *) * (row + 1));
	if (tab == NULL)
		return (NULL);
	i = 0;
	while (i < row)
	{
		tab[i] = (void *)ft_memalloc(size * column);
		if (tab[i] == NULL)
		{
			ft_mem_arr_free((void ***)&tab);
			return (0);
		}
		i++;
	}
	return ((void **)tab);
}
