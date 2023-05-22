/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   arr.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ssoraka <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/01/12 18:12:06 by ssoraka           #+#    #+#             */
/*   Updated: 2020/01/12 18:12:06 by ssoraka          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "collections_header.h"
#include "collections_help_header.h"

void	ft_del_arr(t_arr **arr)
{
	int		i;
	void	*elem;

	if (arr && *arr)
	{
		if ((*arr)->func_del && (*arr)->elems_used > 0)
		{
			i = (*arr)->elems_used - 1;
			while (i >= 0)
			{
				elem = ft_arr_get_addr(*arr, i);
				(*arr)->func_del((*arr)->value(elem));
				i--;
			}
		}
		free((*arr)->elems);
		ft_memdel((void **)arr);
	}
}

void	ft_del_elem(t_arr *arr, size_t num)
{
	void	*dst;
	void	*src;

	if (!arr || !arr->elems_used || num < 0 || num >= arr->elems_used)
		return ;
	(arr->elems_used)--;
	dst = arr->elems + arr->elem_size * num;
	src = arr->elems + arr->elem_size * arr->elems_used;
	if (arr->func_del)
		arr->func_del(arr->value(dst));
	ft_memcpy8(dst, src, arr->elem_size);
}

void	ft_del_elems_if(t_arr *arr, t_bool (*need_del)(void *, void *),
		void *param)
{
	int		i;
	void	*elem;

	if (!arr || !arr->elems || !arr->elems_used || !need_del)
		return ;
	i = arr->elems_used - 1;
	elem = ft_arr_get_addr(arr, i);
	while (i >= 0)
	{
		if (need_del(arr->value(elem), param))
			ft_del_elem(arr, i);
		elem -= arr->elem_size;
		i--;
	}
}
