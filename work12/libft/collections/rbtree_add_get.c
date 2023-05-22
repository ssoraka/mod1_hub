/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   rbtree_add_get.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ssoraka <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/01/12 18:12:06 by ssoraka           #+#    #+#             */
/*   Updated: 2020/01/12 18:12:06 by ssoraka          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "collections_header.h"
#include "collections_help_header.h"

void	ft_rbtree_insert(t_rbtr *tree, t_tnode *node)
{
	t_tnode	*tmp;
	t_tnode	**current;

	if (!tree || !node)
		return ;
	tmp = &tree->root;
	current = &tmp->left;
	while (*current)
	{
		tmp = *current;
		if (tree->func_cmp(tmp->key, node->key) >= 0)
			current = &tmp->right;
		else
			current = &tmp->left;
	}
	*current = node;
	node->parent = tmp;
	(tree->elems_count)++;
}

t_bool	ft_rbtree_add(t_rbtr *tree, void *key, void *value)
{
	t_tnode	*node;

	if (!tree || !value)
		return (FALSE);
	node = ft_create_tnode(key, value);
	if (!node)
		return (FALSE);
	ft_rbtree_insert(tree, node);
	ft_tnode_rebalance(node);
	return (TRUE);
}

void	*ft_rbtree_get_elem(t_rbtr *tree, void *key)
{
	t_tnode	*node;
	int		cmp;

	if (!tree || !key)
		return (NULL);
	node = tree->root.left;
	while (node)
	{
		cmp = tree->func_cmp(node->key, key);
		if (!cmp)
			return (node->elem);
		else if (cmp > 0)
			node = node->right;
		else
			node = node->left;
	}
	return (NULL);
}
