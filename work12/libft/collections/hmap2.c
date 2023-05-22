/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   hmap.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ssoraka <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/01/12 18:12:06 by ssoraka           #+#    #+#             */
/*   Updated: 2020/01/12 18:12:06 by ssoraka          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "collections_header.h"
#include "collections_help_header.h"

t_arr	*ft_create_arr_of_elems(void *value, int elem_size,
		int elems_count, void (*func_del)(void *))
{
	t_arr	*arr;

	arr = ft_create_arr(elem_size, elems_count, func_del);
	if (!arr)
		return (NULL);
	ft_all_arr_init_by_value(arr, value);
	arr->elems_used = arr->elems_count;
	return (arr);
}

void	ft_del_arr_without_del_content(t_arr *arr, t_ilist *ilist)
{
	t_iter	iter;

	iter = get_arr_iter(arr);
	while (iter.get_next_elem(&iter))
		ilist->del_list_without_key_value(iter.value);
	arr->func_del = NULL;
	ft_del_arr(&arr);
}

t_bool	move_elements(t_hmap *map, t_ilist *from)
{
	t_iter	iter;

	iter = map->list.iterator(from->mem);
	while (iter.get_next_elem(&iter))
	{
		if (!ft_hashmap_put(map, iter.key, iter.value))
		{
			ft_del_arr_without_del_content(map->arr, &map->list);
			return (FALSE);
		}
	}
	return (TRUE);
}

int	ft_increase_hmap(t_hmap *hmap)
{
	t_hmap	t;
	t_iter	iter;

	ft_memcpy((void *)&t, hmap, sizeof(t_hmap));
	t.elems_used = 0;
	t.max_load = t.arr->elems_used * 2 * HASHMAP_LOAD;
	if (is_null(ft_create_arr_of_elems((void *)t.list.mem, hmap->list.size,
				hmap->arr->elems_used << 1, hmap->list.del), (void **) &t.arr))
		return (FALSE);
	iter = get_arr_iter(hmap->arr);
	while (iter.get_next_elem(&iter))
	{
		if (!move_elements(&t, (t_ilist *)iter.value))
			return (FALSE);
	}
	ft_del_arr_without_del_content(hmap->arr, &hmap->list);
	ft_memcpy((void *)hmap, (void *)&t, sizeof(t_hmap));
	return (TRUE);
}
