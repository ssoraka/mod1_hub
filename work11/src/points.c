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

#include "../includes/ft_mod1.h"

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

void	ft_create_xyz(t_oxyz *oxyz)
{
	ft_fill_dpoint(&(oxyz->ox), 0.0, 1.0, 0.0);
	ft_fill_dpoint(&(oxyz->oy), 1.0, 0.0, 0.0);
	ft_fill_dpoint(&(oxyz->oz), 0.0, 0.0, 1.0);
}

REAL	ft_dot_dpoints(t_dpoint *a, t_dpoint *b)
{
	return (a->x * b->x + a->y * b->y + a->z * b->z);
}

REAL	ft_vektr_len(t_dpoint *a)
{
	return (sqrt(ft_dot_dpoints(a, a)));
}

t_dpoint	ft_ret_norm(t_dpoint *a, t_dpoint *b, t_dpoint *c)
{
	t_dpoint n;
	t_dpoint ba;
	t_dpoint bc;

	ft_fill_dpoint(&ba, a->y - b->y, a->x - b->x, a->z - b->z);
	ft_fill_dpoint(&bc, c->y - b->y, c->x - b->x, c->z - b->z);
	ft_fill_dpoint(&n,
		ba.z * bc.x - ba.x * bc.z,
		ba.y * bc.z - ba.z * bc.y,
		ba.x * bc.y - ba.y * bc.x);
	return (n);
}

REAL	ft_vekt_cos(t_dpoint a, t_dpoint b)
{
	REAL cos;

	cos = ft_dot_dpoints(&a, &b) /
	sqrt(ft_dot_dpoints(&a, &a) * ft_dot_dpoints(&b, &b));
	return (cos);
}
