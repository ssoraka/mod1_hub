/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   arr_add.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ssoraka <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/01/12 18:12:06 by ssoraka           #+#    #+#             */
/*   Updated: 2020/01/12 18:12:06 by ssoraka          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "collections_header.h"
#include "collections_help_header.h"

t_bool	ft_realloc_arr(t_arr *arr, size_t new_count)
{
	void	*tmp;

	if (!arr)
		return (FALSE);
	if (new_count <= arr->elems_count)
		return (TRUE);
	if (is_null(ft_memalloc(new_count * arr->elem_size), &tmp))
		return (FALSE);
	if (arr->elems)
		ft_memcpy8(tmp, arr->elems, arr->elems_count * arr->elem_size);
	free(arr->elems);
	arr->elems = tmp;
	arr->elems_count = new_count;
	return (TRUE);
}

void	*ft_arr_add(t_arr *arr, void *elem)
{
	char	*tmp;

	if (!arr || !elem)
		return (NULL);
	if (arr->elems_used == arr->elems_count)
		if (!ft_realloc_arr(arr, arr->elems_count * ARR_REALLOC_COEF + 1))
			return (NULL);
	tmp = (char *)arr->elems + arr->elems_used * arr->elem_size;
	if (arr->value == ft_return_ptr)
		ft_memcpy8((void *)tmp, &elem, arr->elem_size);
	else
		ft_memcpy8((void *)tmp, elem, arr->elem_size);
	(arr->elems_used)++;
	return (tmp);
}
