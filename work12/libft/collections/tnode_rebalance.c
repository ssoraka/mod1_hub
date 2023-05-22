/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tnode_rebalance.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ssoraka <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/01/12 18:12:06 by ssoraka           #+#    #+#             */
/*   Updated: 2020/01/12 18:12:06 by ssoraka          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "collections_header.h"
#include "collections_help_header.h"

void	ft_case_red_dad_red_uncle(t_tnode *node)
{
	node->parent->color = BLACK;
	ft_get_uncle(node)->color = BLACK;
	node->parent->parent->color = RED;
	ft_tnode_rebalance(node->parent->parent);
}

void	ft_case_red_right_dad_and_black_left_uncle(t_tnode *node)
{
	if (node->parent->right == node)
	{
		node->parent->color = BLACK;
		node->parent->parent->color = RED;
		ft_tnode_left_rotate(node->parent);
	}
	else
	{
		ft_tnode_right_rotate(node);
		ft_case_red_right_dad_and_black_left_uncle(node->right);
	}
}

void	ft_case_red_left_dad_and_black_right_uncle(t_tnode *node)
{
	if (node->parent->left == node)
	{
		node->parent->color = BLACK;
		node->parent->parent->color = RED;
		ft_tnode_right_rotate(node->parent);
	}
	else
	{
		ft_tnode_left_rotate(node);
		ft_case_red_left_dad_and_black_right_uncle(node->left);
	}
}

void	ft_tnode_rebalance(t_tnode *node)
{
	if (!node || !node->parent)
		return ;
	else if (!node->parent->parent)
		node->color = BLACK;
	else if (node->color == RED && node->parent->color == RED)
	{
		if (ft_is_red_color(ft_get_uncle(node)))
			ft_case_red_dad_red_uncle(node);
		else if (ft_is_right_child(node->parent))
			ft_case_red_right_dad_and_black_left_uncle(node);
		else
			ft_case_red_left_dad_and_black_right_uncle(node);
	}
}
