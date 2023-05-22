/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   llist_get.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ssoraka <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/01/12 18:12:06 by ssoraka           #+#    #+#             */
/*   Updated: 2020/01/12 18:12:06 by ssoraka          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "collections_header.h"
#include "collections_help_header.h"

void	ft_for_each_llist(t_llist *list,
		void (*func)(void *, void *), void *param)
{
	t_lnode	*node;
	t_lnode	*end;

	if (!func || !list)
		return ;
	node = list->start.next;
	end = &list->end;
	while (node != end)
	{
		func(node->elem, param);
		node = node->next;
	}
}

void	*ft_llist_get(t_llist *list, int num)
{
	t_lnode	*node;

	if (!list)
		return (NULL);
	node = ft_lnode_get(num, list->elems_count,
			list->start.next, list->end.prev);
	if (node)
		return (node->elem);
	return (NULL);
}
