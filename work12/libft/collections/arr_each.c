/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   arr_each.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ssoraka <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/01/12 18:12:06 by ssoraka           #+#    #+#             */
/*   Updated: 2020/01/12 18:12:06 by ssoraka          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "collections_header.h"
#include "collections_help_header.h"

void	ft_for_each_elem(t_arr *arr, void (*func)(void *, void *), void *param)
{
	int		i;
	void	*elem;

	if (!arr || !arr->elems || !func)
		return ;
	elem = arr->elems;
	i = arr->elems_used;
	while (i > 0)
	{
		if ((arr->value(elem)))
			func(arr->value(elem), param);
		elem += arr->elem_size;
		i--;
	}
}

int	ft_arr_init_by_value(t_arr *arr, int count, void *value)
{
	void	*ptr;
	int		i;

	if (!ft_realloc_arr(arr, count))
		return (FALSE);
	ptr = arr->elems;
	i = 0;
	while (i < count)
	{
		ft_memcpy(ptr, value, arr->elem_size);
		ptr += arr->elem_size;
		i++;
	}
	return (TRUE);
}

void	ft_all_arr_init_by_value(t_arr *arr, void *value)
{
	if (arr->elems_count)
		ft_arr_init_by_value(arr, arr->elems_count, value);
}
