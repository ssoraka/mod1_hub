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

void	*get_next_llist_elem(t_iter *iter)
{
	t_llist	*list;
	t_lnode	*node;

	iter->value = NULL;
	node = (t_lnode *)iter->elem;
	list = (t_llist *)iter->collection;
	if (!node)
		node = list->start.next;
	else
		node = node->next;
	if (node == &list->end)
		return (NULL);
	iter->value = node->elem;
	iter->elem = node;
	return (iter->value);
}

t_iter	get_llist_iter(t_llist *list)
{
	t_iter	iter;

	ft_bzero(&iter, sizeof(t_iter));
	iter.collection = list;
	iter.get_next_elem = get_next_llist_elem;
	return (iter);
}
