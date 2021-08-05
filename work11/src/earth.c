/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   earth.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ssoraka <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/02/29 23:24:26 by ssoraka           #+#    #+#             */
/*   Updated: 2020/02/29 23:24:26 by ssoraka          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/ft_mod1.h"

void	ft_del_earth(t_earth **earth)
{
	ft_del_arr(&(*earth)->points);
	ft_del_arr(&(*earth)->poligons);
	ft_memdel((void **)earth);
}

t_earth	*ft_create_earth(void)
{
	t_earth	*earth;

	earth = (t_earth *)ft_memalloc(sizeof(t_earth));
	if (!earth)
		return (NULL);
	earth->points = ft_create_arr(sizeof(t_vektr), 16, NULL);
	earth->poligons = ft_create_arr(sizeof(t_plgn), 16, NULL);
	if (!earth->points || !earth->poligons)
		ft_del_earth(&earth);
	return (earth);
}

int	ft_create_rectang_poligon3(t_arr *poligons, t_vektr **p)
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

int	ft_create_line_of_poligons_of_relief2(t_earth *earth, int k)
{
	t_vektr	*p[3];
	int		i;

	i = 0;
	while (i < earth->columns - 1)
	{
		p[0] = ft_arr_get(earth->points, earth->columns * k + i);
		p[1] = ft_arr_get(earth->points, earth->columns * k + i + 1);
		p[2] = ft_arr_get(earth->points, earth->columns * (k + 1) + i);
		if (!ft_create_rectang_poligon3(earth->poligons, p))
			return (FALSE);
		p[0] = p[2];
		p[2] = ft_arr_get(earth->points, earth->columns * (k + 1) + i + 1);
		if (!ft_create_rectang_poligon3(earth->poligons, p))
			return (FALSE);
		i++;
	}
	return (TRUE);
}

int	ft_create_poligons_of_relief2(t_earth *earth)
{
	int	k;

	k = 0;
	while (k < earth->rows - 1)
	{
		if (!ft_create_line_of_poligons_of_relief2(earth, k))
			return (FALSE);
		k++;
	}
	return (TRUE);
}

int	ft_create_points_of_relief2(t_earth *earth, int **ground)
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
					MAP_HEIGTH2, COLOR_UP, COLOR_DOWN);
			ft_fill_dpoint(&tmp.abs,
				ft_return_heigth(ground[k][i]),
				(REAL)i / (1 + ADD_POINT) + 0.5,
				(REAL)k / (1 + ADD_POINT) + 0.5);
			if (!(ft_arr_add(earth->points, &tmp)))
				return (FALSE);
			i++;
		}
		k++;
	}
	return (TRUE);
}

int	ft_create_relief2(t_earth *earth, int **ground)
{
	earth->rows = (KMAX - K0) * (1 + ADD_POINT) + 1;
	earth->columns = (IMAX - I0) * (1 + ADD_POINT) + 1;
	if (!(ft_create_points_of_relief2(earth, ground)))
		return (FALSE);
	if (!(ft_create_poligons_of_relief2(earth)))
		return (FALSE);
	return (TRUE);
}

void	ft_print_smooth_relief(t_earth *earth, t_pict *pic, t_param *param)
{
	t_vektr	*v;
	t_plgn	*plgn;
	t_iter	iter;
	t_prop	prop;

	iter = get_arr_iter(earth->points);
	while (iter.get_next_elem(&iter))
	{
		v = (t_vektr *)iter.value;
		ft_rotate_point_around_point(param, v);
	}
	prop = set_param(DEFAULT_POINT | (GRADIENT * param->grad), 0,
			OBSTACLES_FRONT_COLOR);
	iter = get_arr_iter(earth->poligons);
	while (iter.get_next_elem(&iter))
	{
		plgn = (t_plgn *)iter.value;
		ft_prepare_plgn_for_printing(plgn, param);
		ft_print_plgn(plgn, pic, prop);
	}
}

void	ft_print_relief(t_earth *earth, t_arr *cells, t_pict *pic,
						t_param *param)
{
	if (!param->is_need_print_obstacles)
		return ;
	if (param->need_refresh)
	{
		param->need_refresh = FALSE;
		if (param->is_smooth_relief == SMOOTH_EARTH)
			ft_print_smooth_relief(earth, pic, param);
		ft_print_all_cell(cells, pic, param);
		if (param->is_smooth_relief == COLORED_EARTH)
			ft_memcpy((void *)pic->addr, (void *)pic->index, pic->count_byte);
		ft_save_image(pic);
	}
	else
		ft_return_image(pic);
}
