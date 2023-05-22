/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   arr_iter.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ssoraka <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/01/12 18:12:06 by ssoraka           #+#    #+#             */
/*   Updated: 2020/01/12 18:12:06 by ssoraka          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "collections_header.h"

void	*get_next_arr_elem(t_iter *iter)
{
	t_arr	*arr;
	void	*elem;

	iter->value = NULL;
	elem = iter->elem;
	arr = (t_arr *)iter->collection;
	if (!elem)
		elem = arr->elems;
	else
		elem = (char *)elem + arr->elem_size;
	if ((char *)elem >= (char *)arr->elems + arr->elem_size * arr->elems_used)
		return (NULL);
	iter->value = arr->value(elem);
	iter->elem = elem;
	return (iter->value);
}

t_iter	get_arr_iter(t_arr *arr)
{
	t_iter	iter;

	ft_bzero(&iter, sizeof(t_iter));
	iter.collection = arr;
	iter.get_next_elem = get_next_arr_elem;
	return (iter);
}
