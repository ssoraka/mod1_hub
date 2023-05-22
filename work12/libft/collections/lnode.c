/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lnode.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ssoraka <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/01/12 18:12:06 by ssoraka           #+#    #+#             */
/*   Updated: 2020/01/12 18:12:06 by ssoraka          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "collections_header.h"
#include "collections_help_header.h"

t_lnode	*ft_create_lnode(void *value)
{
	t_lnode	*lnode;

	lnode = ft_memalloc(sizeof(t_lnode));
	if (lnode)
		lnode->elem = value;
	return (lnode);
}

void	ft_insert_lnode(t_lnode *node, t_lnode *before)
{
	t_lnode	*after;

	if (!node || !before)
		return ;
	after = before->next;
	before->next = node;
	node->prev = before;
	node->next = after;
	after->prev = node;
}

void	*ft_cut_lnode(t_lnode *node)
{
	t_lnode	*before;
	t_lnode	*after;
	void	*elem;

	before = node->prev;
	after = node->next;
	if (before)
		before->next = after;
	if (after)
		after->prev = before;
	elem = node->elem;
	if (before != after)
		free(node);
	return (elem);
}

void	ft_del_lnode(t_lnode **node, void (*func_del)(void *))
{
	func_del(ft_cut_lnode(*node));
}

t_lnode	*ft_lnode_get(int num, int count, t_lnode *start, t_lnode *end)
{
	t_lnode	*node;

	if (num < 0 || num >= count)
		return (NULL);
	if (num <= count >> 1)
	{
		node = start;
		while (num-- > 0)
			node = node->next;
	}
	else
	{
		node = end;
		while (++num < count)
			node = node->prev;
	}
	return (node);
}
