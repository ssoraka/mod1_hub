/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   llist_del.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ssoraka <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/01/12 18:12:06 by ssoraka           #+#    #+#             */
/*   Updated: 2020/01/12 18:12:06 by ssoraka          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "collections_header.h"
#include "collections_help_header.h"

void	ft_del_llist_elem(t_llist *list, int num)
{
	t_lnode	*node;
	void	*elem;

	if (!list)
		return ;
	node = ft_lnode_get(num, list->elems_count, list->start.next,
			list->end.prev);
	if (node)
	{
		elem = ft_cut_lnode(node);
		if (elem && list->func_del)
			list->func_del(elem);
		list->elems_count--;
	}
}

void	ft_del_llist(t_llist **list)
{
	void	*elem;
	int		i;
	t_llist	*tmp;

	if (!list || !(*list))
		return ;
	tmp = *list;
	i = tmp->elems_count;
	while (i > 0)
	{
		elem = ft_cut_lnode(tmp->start.next);
		if (tmp->func_del)
			tmp->func_del(elem);
		i--;
	}
	ft_memdel((void **)list);
}
