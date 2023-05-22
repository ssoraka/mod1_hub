/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tnode_track.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ssoraka <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/01/12 18:12:06 by ssoraka           #+#    #+#             */
/*   Updated: 2020/01/12 18:12:06 by ssoraka          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "collections_header.h"
#include "collections_help_header.h"

t_tnode	*ft_find_left_value(t_tnode *node)
{
	while (node->left)
		node = node->left;
	return (node);
}

void	ft_tnode_infix(t_tnode *node,
		void (*func)(void *, void *), void *param)
{
	if (node->left)
		ft_tnode_infix(node->left, func, param);
	func(node->elem, param);
	if (node->right)
		ft_tnode_infix(node->right, func, param);
}

void	ft_tnode_postfix(t_tnode *node,
		void (*func)(void *, void *), void *param)
{
	if (node->right)
		ft_tnode_postfix(node->right, func, param);
	func(node->elem, param);
	if (node->left)
		ft_tnode_postfix(node->left, func, param);
}

void	ft_tnode_prefix(t_tnode *node,
		void (*func)(void *, void *), void *param)
{
	func(node->elem, param);
	if (node->left)
		ft_tnode_prefix(node->left, func, param);
	if (node->right)
		ft_tnode_prefix(node->right, func, param);
}
