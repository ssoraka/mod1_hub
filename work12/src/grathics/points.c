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

#include "../../includes/ft_mod1.h"

void	ft_fill_point(t_point *p, int y, int x, int z)
{
	if (!p)
		return ;
	p->x = x;
	p->y = y;
	p->z = z;
}

void	ft_fill_dpoint(t_dpoint *p, t_real y, t_real x, t_real z)
{
	if (!p)
		return ;
	p->x = x;
	p->y = y;
	p->z = z;
}

t_real	ft_dot_dpoints(t_dpoint *a, t_dpoint *b)
{
	return (a->x * b->x + a->y * b->y + a->z * b->z);
}

t_real	ft_vektr_len(t_dpoint *a)
{
	return (sqrt(ft_dot_dpoints(a, a)));
}

t_real	ft_vekt_cos(t_dpoint a, t_dpoint b)
{
	t_real	cos;

	cos = ft_dot_dpoints(&a, &b) / sqrt(
			ft_dot_dpoints(&a, &a) * ft_dot_dpoints(&b, &b));
	return (cos);
}
