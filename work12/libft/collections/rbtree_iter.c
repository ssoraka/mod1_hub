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
#include "collections_help_header.h"

t_tnode	*ft_rbtree_get_next_node(t_rbtr *tree, t_tnode *node)
{
	if (node->right)
	{
		node = node->right;
		node = ft_find_left_value(node);
	}
	else if (node->parent != &tree->root && node == node->parent->left)
		node = node->parent;
	else
	{
		while (node != node->parent->left)
			node = node->parent;
		if (node->parent == &tree->root)
			node = NULL;
		else
			node = node->parent;
	}
	return (node);
}

void	*get_next_rbtree_elem(t_iter *iter)
{
	t_rbtr	*tree;
	t_tnode	*node;

	iter->value = NULL;
	iter->key = NULL;
	node = (t_tnode *)iter->elem;
	tree = (t_rbtr *)iter->collection;
	if (!node && tree->root.left)
		node = ft_find_left_value(tree->root.left);
	else if (node)
		node = ft_rbtree_get_next_node(tree, node);
	if (!node)
		return (NULL);
	iter->value = node->elem;
	iter->key = node->key;
	iter->elem = node;
	return (iter->value);
}

t_iter	get_rbtree_iter(t_rbtr *tree)
{
	t_iter	iter;

	ft_bzero(&iter, sizeof(t_iter));
	iter.collection = tree;
	iter.get_next_elem = get_next_rbtree_elem;
	return (iter);
}
