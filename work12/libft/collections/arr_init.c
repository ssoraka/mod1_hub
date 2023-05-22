/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   arr_init.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ssoraka <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/01/12 18:12:06 by ssoraka           #+#    #+#             */
/*   Updated: 2020/01/12 18:12:06 by ssoraka          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "collections_header.h"
#include "collections_help_header.h"

void	*ft_return_elem(void *elem)
{
	return (elem);
}

void	*ft_return_ptr(void *elem)
{
	return (*((void **)elem));
}

int	ft_init_arr(t_arr *arr, size_t elem_size, size_t elems_count,
			void (*func_del)(void *))
{
	if (!arr)
		return (FALSE);
	if (elems_count != 0 && elems_count < ARR_FIRST_COUNT)
		elems_count = ARR_FIRST_COUNT;
	arr->elems_count = elems_count;
	arr->elems_used = 0;
	arr->elem_size = elem_size;
	arr->func_del = func_del;
	arr->elems = ft_memalloc(elem_size * elems_count);
	if (!arr->elems)
		return (FALSE);
	return (TRUE);
}

t_arr	*ft_create_arr(size_t elem_size,
			size_t elems_count, void (*func_del)(void *))
{
	t_arr	*arr;

	if (elem_size == 0)
		return (NULL);
	arr = ft_memalloc(sizeof(t_arr));
	if (arr)
	{
		arr->value = &ft_return_elem;
		if (!ft_init_arr(arr, elem_size, elems_count, func_del))
			ft_memdel((void **)&arr);
	}
	return (arr);
}

t_arr	*ft_create_arr_of_ptr(size_t elems_count, void (*func_del)(void *))
{
	t_arr	*arr;

	arr = ft_create_arr(SIZE_OF_PTR, elems_count, func_del);
	if (!arr)
		return (NULL);
	arr->value = &ft_return_ptr;
	return (arr);
}
