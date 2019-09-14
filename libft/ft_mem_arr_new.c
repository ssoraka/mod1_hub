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
	char	**tab;

	i = 0;
	if ((tab = (char**)ft_memalloc(sizeof(char *) * (row + 1))) == NULL)
		return (NULL);
	while (i < row)
	{
		if ((tab[i] = (char*)ft_memalloc(size * column)) == NULL)
		{
			ft_str_arr_free(&tab);
			return (0);
		}
		i++;
	}
	return ((void	**)tab);
}
