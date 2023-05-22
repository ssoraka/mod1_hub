/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   llist_add.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ssoraka <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/01/12 18:12:06 by ssoraka           #+#    #+#             */
/*   Updated: 2020/01/12 18:12:06 by ssoraka          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "collections_header.h"
#include "collections_help_header.h"

int	ft_llist_add(t_llist *list, void *elem)
{
	return (ft_llist_dpush(list, elem));
}

int	ft_insert_llist(t_llist *list, void *elem, int num)
{
	t_lnode	*node;
	t_lnode	*next;

	if (!list)
		return (FALSE);
	next = ft_lnode_get(num, list->elems_count, list->start.next,
			list->end.prev);
	if (!next)
		return (FALSE);
	node = ft_create_lnode(elem);
	if (!node)
		return (FALSE);
	ft_insert_lnode(node, next->prev);
	return (TRUE);
}
