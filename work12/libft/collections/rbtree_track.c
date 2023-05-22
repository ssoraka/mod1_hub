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

/*
** todo надо исправить эту функцию
*/

void	ft_rbtree_prefix(t_rbtr *tree,
			void (*func)(void *, void *), void *param)
{
	if (tree->root.left)
		ft_tnode_prefix(tree->root.left, func, param);
}

void	ft_rbtree_postfix(t_rbtr *tree,
			void (*func)(void *, void *), void *param)
{
	if (tree->root.left)
		ft_tnode_postfix(tree->root.left, func, param);
}

void	ft_rbtree_infix(t_rbtr *tree,
			void (*func)(void *, void *), void *param)
{
	if (tree->root.left)
		ft_tnode_infix(tree->root.left, func, param);
}
