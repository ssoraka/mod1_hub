/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tnode_rbtee_property.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ssoraka <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/01/12 18:12:06 by ssoraka           #+#    #+#             */
/*   Updated: 2020/01/12 18:12:06 by ssoraka          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "collections_header.h"
#include "collections_help_header.h"

int	ft_is_red_color(t_tnode *node)
{
	if (!node || node->color == BLACK)
		return (FALSE);
	return (TRUE);
}

int	ft_is_right_child(t_tnode *node)
{
	if (node == node->parent->right)
		return (TRUE);
	return (FALSE);
}

t_tnode	*ft_get_uncle(t_tnode *node)
{
	t_tnode	*grant;

	if (!node || !node->parent)
		return (NULL);
	grant = node->parent->parent;
	if (!grant)
		return (NULL);
	if (node->parent == grant->left)
		return (grant->right);
	else
		return (grant->left);
}

void	ft_tnode_left_rotate(t_tnode *node)
{
	t_tnode	*grant;
	t_tnode	*tmp;

	grant = node->parent->parent;
	if (!grant)
		return ;
	tmp = node->parent;
	if (grant->right == tmp)
		grant->right = node;
	else
		grant->left = node;
	node->parent = grant;
	tmp->right = node->left;
	if (node->left)
		node->left->parent = tmp;
	node->left = tmp;
	tmp->parent = node;
}

void	ft_tnode_right_rotate(t_tnode *node)
{
	t_tnode	*grant;
	t_tnode	*tmp;

	grant = node->parent->parent;
	if (!grant)
		return ;
	tmp = node->parent;
	if (grant->right == tmp)
		grant->right = node;
	else
		grant->left = node;
	node->parent = grant;
	tmp->left = node->right;
	if (node->right)
		node->right->parent = tmp;
	node->right = tmp;
	tmp->parent = node;
}
