/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tnode_init_del.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ssoraka <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/01/12 18:12:06 by ssoraka           #+#    #+#             */
/*   Updated: 2020/01/12 18:12:06 by ssoraka          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "collections_header.h"
#include "collections_help_header.h"

t_tnode	*ft_create_tnode(void *key, void *value)
{
	t_tnode	*node;

	node = ft_memalloc(sizeof(t_tnode));
	if (node)
	{
		node->elem = value;
		node->key = key;
		node->color = RED;
	}
	return (node);
}

void	ft_tnode_del(t_tnode *node, void (*func)(void *, void *))
{
	if (func && node->elem)
		func(node->key, node->elem);
	free(node);
}

void	ft_tnode_del_all(t_tnode *node, void (*func)(void *, void *))
{
	if (node->left)
		ft_tnode_del_all(node->left, func);
	if (node->right)
		ft_tnode_del_all(node->right, func);
	ft_tnode_del(node, func);
}
