/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   llist.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ssoraka <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/01/12 18:12:06 by ssoraka           #+#    #+#             */
/*   Updated: 2020/01/12 18:12:06 by ssoraka          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "collections_header.h"
#include "collections_help_header.h"

int	ft_llist_dpush(t_llist *list, void *elem)
{
	t_lnode	*node;

	if (!list)
		return (FALSE);
	node = ft_create_lnode(elem);
	if (!node)
		return (FALSE);
	ft_insert_lnode(node, list->end.prev);
	list->elems_count++;
	return (TRUE);
}

int	ft_llist_push(t_llist *list, void *elem)
{
	t_lnode	*node;

	if (!list)
		return (FALSE);
	node = ft_create_lnode(elem);
	if (!node)
		return (FALSE);
	ft_insert_lnode(node, &list->start);
	list->elems_count++;
	return (TRUE);
}

void	*ft_llist_pop(t_llist *list)
{
	if (!list || !list->elems_count)
		return (NULL);
	list->elems_count--;
	return (ft_cut_lnode(list->start.next));
}

void	*ft_llist_dpop(t_llist *list)
{
	if (!list || !list->elems_count)
		return (NULL);
	list->elems_count--;
	return (ft_cut_lnode(list->end.prev));
}
