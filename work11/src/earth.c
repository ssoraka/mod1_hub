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


void	ft_del_elems(void *elem)
{
	free(elem);
}

void	ft_del_earth(t_earth **earth)
{
	ft_del_arr(&(*earth)->points);
	ft_del_arr(&(*earth)->poligons);
	ft_memdel((void **)earth);
}

t_earth	*ft_create_earth(void)
{
	t_earth *earth;

	if (!(earth = (t_earth *)ft_memalloc(sizeof(t_earth))))
		return (NULL);
	earth->points = ft_create_arr_of_ptr(0, &ft_del_elems);
	earth->poligons = ft_create_arr_of_ptr(0, &ft_del_elems);
	if (!earth->points || !earth->poligons)
		ft_del_earth(&earth);
	return (earth);
}



int		ft_create_rectang_poligon3(t_arr *poligons, t_vektr **p, int color)
{
	t_plgn *tmp;

	if (!(tmp = ft_create_poligon(p[0], p[1], p[2], color)))
		return (FALSE);
	if (!(ft_arr_add(poligons, tmp)))
	{
		free(tmp);
		return (FALSE);
	}
	return (TRUE);
}



int		ft_create_line_of_poligons_of_relief2(t_earth *earth, int k)
{
	t_vektr *p[3];
	int i;

	i = 0;
	while (i < earth->columns - 1)
	{
		p[0] = ft_arr_get(earth->points, earth->columns * k + i);
		p[1] = ft_arr_get(earth->points, earth->columns * k + i + 1);
		p[2] = ft_arr_get(earth->points, earth->columns * (k + 1) + i);
		if (!ft_create_rectang_poligon3(earth->poligons, p, OBSTACLES_FRONT_COLOR))
			return (FALSE);
		p[0] = p[2];
		p[2] = ft_arr_get(earth->points, earth->columns * (k + 1) + i + 1);
		if (!ft_create_rectang_poligon3(earth->poligons, p, OBSTACLES_FRONT_COLOR))
			return (FALSE);
		i++;
	}
	return (TRUE);
}

int		ft_create_poligons_of_relief2(t_earth *earth)
{
	int k;

	k = 0;
	while (k < earth->rows - 1)
	{
		if (!ft_create_line_of_poligons_of_relief2(earth, k))
			return (FALSE);
		k++;
	}
	return (TRUE);
}


int		ft_create_points_of_relief2(t_earth *earth, int  **ground)
{
	t_vektr *tmp;
	int color;
	int k;
	int i;

	k = K0 * (1 + ADD_POINT);
	while (k < earth->rows + K0 * (1 + ADD_POINT))
	{
		i = I0 * (1 + ADD_POINT);
		while (i < earth->columns + I0 * (1 + ADD_POINT))
		{
			color = ft_grad_color(ground[k][i], MAP_HEIGTH2, COLOR_UP, COLOR_DOWN);
			if (!(tmp = ft_new_vektor2((REAL)i / (1 + ADD_POINT) + 0.5,
			ft_return_heigth(ground[k][i]), (REAL)k / (1 + ADD_POINT) + 0.5, color))
			|| !(ft_arr_add(earth->points, tmp)))
			{
				free (tmp);
				return (FALSE);
			}
			i++;
		}
		k++;
	}
	return (TRUE);
}


int		ft_create_relief2(t_earth *earth, int  **ground)
{
	earth->rows = (KMAX - K0) * (1 + ADD_POINT) + 1;
	earth->columns = (IMAX - I0) * (1 + ADD_POINT) + 1;
	if (!(ft_create_points_of_relief2(earth, ground)))
		return(FALSE);
	if (!(ft_create_poligons_of_relief2(earth)))
		return(FALSE);
	return(TRUE);
}

void	ft_print_smooth_relief(t_earth *earth, t_pict *pic, t_param *param)
{
	t_vektr *v;
	t_plgn *plgn;
	t_iter	iter;

	iter = get_arr_iter(earth->points);
	while ((v = (t_vektr *)iter.get_next_elem(&iter)))
		ft_change_points4(param, v);
	iter = get_arr_iter(earth->poligons);
	while ((plgn = (t_plgn *)iter.get_next_elem(&iter)))
	{
		ft_prepare_plgn_for_printing(plgn, param);
		//if (ft_vekt_cos(plgn->rot_n, (t_dpoint){0.0, 0.0, 1.0}) > 0.0)
			ft_print_plgn(plgn, pic, param->grad);
	}
}


void	ft_print_relief(t_earth *earth, t_arr *cells, t_pict *pic, t_param *param)
{
	if (!param->is_need_print_obstacles)
		return ;
	if (param->is_obstacles_change)
	{
		param->is_obstacles_change = FALSE;
		if (param->is_smooth_relief)
			ft_print_smooth_relief(earth, pic, param);
		ft_print_all_cell(cells, pic, param);
		ft_save_image(pic);
	}
	else
		ft_return_image(pic);
}
