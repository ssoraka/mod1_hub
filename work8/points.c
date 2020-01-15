/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   points.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ssoraka <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/10/27 17:51:06 by ssoraka           #+#    #+#             */
/*   Updated: 2019/10/27 17:51:07 by ssoraka          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_mod1.h"

void	ft_fill_point(t_point *p, int y, int x, int z)
{
	if (!p)
		return ;
	p->x = x;
	p->y = y;
	p->z = z;
}

void	ft_fill_dpoint(t_dpoint *p, REAL y, REAL x, REAL z)
{
	if (!p)
		return ;
	p->x = x;
	p->y = y;
	p->z = z;
}

void	ft_create_xyz(t_vis *vis)
{
	ft_fill_dpoint(&(vis->oxyz.ox), 0.0, 1.0, 0.0);
	ft_fill_dpoint(&(vis->oxyz.oy), 1.0, 0.0, 0.0);
	ft_fill_dpoint(&(vis->oxyz.oz), 0.0, 0.0, 1.0);
}
