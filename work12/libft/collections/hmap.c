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

int	ft_hashmap_put(t_hmap *hmap, void *key, void *value)
{
	int		index;
	void	*list;

	if (hmap->elems_used >= hmap->max_load)
		if (!ft_increase_hmap(hmap))
			return (FALSE);
	index = hmap->func_hash(key) & (hmap->arr->elems_used - 1);
	list = ft_arr_get(hmap->arr, index);
	if (!(hmap->list.add(list, key, value)))
		return (FALSE);
	hmap->elems_used++;
	return (TRUE);
}

void	*ft_hashmap_get(t_hmap *hmap, void *key)
{
	int		index;
	void	*list;

	index = hmap->func_hash(key) & (hmap->arr->elems_used - 1);
	list = ft_arr_get(hmap->arr, index);
	return (hmap->list.find(list, key));
}

void	ft_del_hmap(t_hmap **hmap)
{
	ft_del_arr(&((*hmap)->arr));
	ft_memdel((void **)hmap);
}

t_hmap	*ft_create_hashmap(int (*func_hash)(void *), t_ilist *list)
{
	t_hmap	*hmap;

	if (!func_hash || !list)
		return (NULL);
	hmap = ft_memalloc(sizeof(t_hmap));
	if (!hmap)
		return (NULL);
	hmap->func_hash = func_hash;
	ft_memcpy((void *)&hmap->list, (void *)list, sizeof(t_ilist));
	hmap->max_load = HASHMAP_FIRST_COUNT * HASHMAP_LOAD;
	hmap->arr = ft_create_arr_of_elems((void *)hmap->list.mem, hmap->list.size,
			HASHMAP_FIRST_COUNT, hmap->list.del);
	if (!hmap->arr)
		ft_memdel((void **)&hmap);
	return (hmap);
}
