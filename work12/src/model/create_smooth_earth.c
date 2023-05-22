/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   create_smooth_earth.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ssoraka <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/02/29 23:24:26 by ssoraka           #+#    #+#             */
/*   Updated: 2020/02/29 23:24:26 by ssoraka          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/ft_mod1.h"

int	ft_create_rectang_poligon(t_arr *poligons, t_vektr **p)
{
	t_plgn	tmp;

	ft_bzero((void *)&tmp, sizeof(t_plgn));
	tmp.p[0] = p[0];
	tmp.p[1] = p[1];
	tmp.p[2] = p[2];
	tmp.n = ft_ret_norm(&p[0]->abs, &p[1]->abs, &p[2]->abs);
	if (!(ft_arr_add(poligons, &tmp)))
		return (FALSE);
	return (TRUE);
}

int	ft_create_line_of_poligons_of_relief(t_earth *earth, int k)
{
	t_vektr	*p[3];
	int		i;

	i = 0;
	while (i < earth->columns - 1)
	{
		p[0] = ft_arr_get(earth->points, earth->columns * k + i);
		p[1] = ft_arr_get(earth->points, earth->columns * k + i + 1);
		p[2] = ft_arr_get(earth->points, earth->columns * (k + 1) + i);
		if (!ft_create_rectang_poligon(earth->poligons, p))
			return (FALSE);
		p[0] = p[2];
		p[2] = ft_arr_get(earth->points, earth->columns * (k + 1) + i + 1);
		if (!ft_create_rectang_poligon(earth->poligons, p))
			return (FALSE);
		i++;
	}
	return (TRUE);
}

int	ft_create_poligons_of_relief(t_earth *earth)
{
	int	k;

	k = 0;
	while (k < earth->rows - 1)
	{
		if (!ft_create_line_of_poligons_of_relief(earth, k))
			return (FALSE);
		k++;
	}
	return (TRUE);
}

int	ft_create_points_of_relief(t_earth *earth, int **ground)
{
	t_vektr	tmp;
	int		k;
	int		i;

	ft_bzero((void *)&tmp, sizeof(t_vektr));
	k = K0 * (1 + ADD_POINT);
	while (k < earth->rows + K0 * (1 + ADD_POINT))
	{
		i = I0 * (1 + ADD_POINT);
		while (i < earth->columns + I0 * (1 + ADD_POINT))
		{
			tmp.color = ft_grad_color(ground[k][i],
					(JMAX + J0) * 100, COLOR_UP, COLOR_DOWN);
			ft_fill_dpoint(&tmp.abs,
				ft_return_heigth(ground[k][i]),
				(t_real)i / (1 + ADD_POINT) + 0.5,
				(t_real)k / (1 + ADD_POINT) + 0.5);
			if (!(ft_arr_add(earth->points, &tmp)))
				return (FALSE);
			i++;
		}
		k++;
	}
	return (TRUE);
}

int	ft_create_relief(t_earth *earth, int **ground)
{
	earth->rows = (KMAX - K0) * (1 + ADD_POINT) + 1;
	earth->columns = (IMAX - I0) * (1 + ADD_POINT) + 1;
	if (!(ft_create_points_of_relief(earth, ground)))
		return (FALSE);
	if (!(ft_create_poligons_of_relief(earth)))
		return (FALSE);
	return (TRUE);
}
