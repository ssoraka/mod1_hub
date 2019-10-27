/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ground.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ssoraka <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/10/18 20:37:56 by ssoraka           #+#    #+#             */
/*   Updated: 2019/10/18 20:37:57 by ssoraka          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_mod1.h"


typedef struct		s_ground
{
	int				**complex;
	int				**simple;
}					t_ground;



int		ft_cube_interpolate (int *arr, REAL x)
{
	REAL value;

	value = arr[1] + (-arr[0] + arr[2]) * (x / 2)
	+ (2 * arr[0] - 5 * arr[1] + 4 * arr[2] - arr[3]) * x * (x / 2)
	+ (-arr[0] + 3 * arr[1] - 3 * arr[2] + arr[3]) * x * x * (x / 2);

	return ((int)value);
}



void	ft_bicube_interpolate_x(void *param, int j, int i, int k)
{
	t_ground *ground;
	int n;
	int tmp[4];

	ground = (t_ground *)param;
	ground->complex[k * (1 + ADD_POINT)][i * (1 + ADD_POINT)] = ground->simple[k][i];
	n = 1;
	while (n < ADD_POINT + 1)
	{
		if (i < IMAX - 2)
			ground->complex[k * (1 + ADD_POINT)][i * (1 + ADD_POINT) + n] = ft_cube_interpolate(ground->simple[k] + i - 1, ((REAL)n / (1 + ADD_POINT)));
		else
		{
			tmp[0] = ground->simple[k][i - 1];
			tmp[1] = ground->simple[k][i];
			tmp[2] = ground->simple[k][i + 1];
			tmp[3] = 0;
			ground->complex[k * (1 + ADD_POINT)][i * (1 + ADD_POINT) + n] = ft_cube_interpolate(tmp, ((REAL)n / (1 + ADD_POINT)));
		}
		n++;
	}
}


void	ft_bicube_interpolate_y(void *param, int j, int i, int k)
{
	t_ground *ground;
	int n;
	int tmp[4];

	ground = (t_ground *)param;
	ft_bzero((void *)tmp, 4 * sizeof(int));
	tmp[0] = ground->complex[(k - 1) * (1 + ADD_POINT)][i];
	tmp[1] = ground->complex[k * (1 + ADD_POINT)][i];
	tmp[2] = ground->complex[(k + 1) * (1 + ADD_POINT)][i];
	if (k < KMAX - 2)
		tmp[3] = ground->complex[(k + 2) * (1 + ADD_POINT)][i];
	n = 1;
	while (n < ADD_POINT + 1)
	{
		ground->complex[k * (1 + ADD_POINT) + n][i] = ft_cube_interpolate(tmp, ((REAL)n / (1 + ADD_POINT)));
		n++;
	}
}


int		**ft_create_complex_ground_from_simple(int  **simple_ground)
{
	t_point start;
	t_point end;
	t_ground ground;

	ground.simple = simple_ground;
	if (!(ground.complex = (int **)ft_mem_arr_new(KMAX * (1 + ADD_POINT), IMAX * (1 + ADD_POINT), sizeof(int))))
		return (NULL);
	ft_fill_point(&start, 1, 1, 1);
	ft_fill_point(&end, 0, IMAX - 2, KMAX - 1);
	ft_cycle_cube((void *)(&ground), &ft_bicube_interpolate_x, &start, &end);
	ft_fill_point(&start, 1, 1 + ADD_POINT, 1);
	ft_fill_point(&end, 0, (1 + ADD_POINT) * (IMAX - 1), KMAX - 2);
	ft_cycle_cube((void *)(&ground), &ft_bicube_interpolate_y, &start, &end);
	return (ground.complex);
}



int		ft_color_from_z(int value)
{
	int color;

	color = ft_grad_color(value, MAP_HEIGTH2, COLOR_UP, COLOR_DOWN);

	return (color);
}


void	ft_points_push_back(t_vektr *begin, t_vektr *last)
{
	if (!begin || !last)
		return ;
	while (begin->next)
		begin = begin->next;
	begin->next = last;
}

int		ft_create_points_from_string(t_vektr **begin, int *ground, int k)
{
	int i;
	int color;
	t_point p;

	i = 0;
	while (i < IMAX * (1 + ADD_POINT))
	{
		color = ft_color_from_z(ground[i]);
		ground[i] = (ground[i] * KMAX * MAP_KOEF * dy) / MAP_HEIGTH2;
		ft_fill_point(&p, ground[i] , (i * dx) / (1 + ADD_POINT), (k * dz) / (1 + ADD_POINT));
		if (!ft_add_vektor2((void *)begin, &p, color))
		{

			ft_del_vektor(begin);
			return (FAIL);
		}
		i++;
	}
	return (SUCCESS);
}


t_vektr	*ft_create_points_of_relief(int  **ground, t_vektr **p)
{
	t_vektr *tmp;
	int k;

	tmp = NULL;
	k = 0;
	while (k < KMAX * (1 + ADD_POINT))
	{
		p[k] = NULL;
		if (ft_create_points_from_string(&(p[k]), ground[k], k) == FAIL)
			return (NULL);
		ft_points_push_back(p[k], tmp);
		tmp = p[k];
		k++;
	}
	return (tmp);
}


int		ft_create_rectang_poligon2(t_plgn **plgn, t_vektr **p, int color)
{
	t_plgn *tmp;


	if (!(tmp = ft_create_poligon(p[0], p[1], p[2], color)))
	{
		ft_del_poligines(plgn);
		return (FAIL);
	}
	tmp->next = *plgn;
	*plgn = tmp;
	if (!(tmp = ft_create_poligon(p[0], p[3], p[2], color)))
	{
		ft_del_poligines(plgn);
		return (FAIL);
	}
	tmp->next = *plgn;
	*plgn = tmp;
	return (SUCCESS);
}



int		ft_create_line_of_poligons_of_relief(t_plgn **plgn, t_vektr *p1, t_vektr *p2)
{
	t_vektr *p[4];

	while (p1->next && p2->next)
	{
		p[0] = p1;
		p[1] = p1->next;
		p[2] = p2->next;
		p[3] = p2;

		if (ft_create_rectang_poligon2(plgn, p, OBSTACLES_FRONT_COLOR) == FAIL)
		{
			ft_del_poligines(plgn);
			return (FALSE);
		}
		p1 = p1->next;
		p2 = p2->next;
	}
	return (TRUE);
}


t_plgn	*ft_create_poligons_of_relief(t_vektr **p_arr)
{
	t_plgn *plgn;
	t_vektr *begin1;
	t_vektr *begin2;
	int k;

	k = 0;
	plgn = NULL;
	while (k < KMAX * (1 + ADD_POINT) - 1)
	{
		begin1 = p_arr[k];
		begin2 = p_arr[k + 1];
		if (!ft_create_line_of_poligons_of_relief(&plgn, begin1, begin2))
			break ;
		k++;
	}
	return (plgn);
}


int		ft_create_relief(t_vis *vis, int  **ground)
{
	t_vektr *p;
	t_vektr *p_arr[KMAX * (1 + ADD_POINT)];
	t_plgn *plgn;

	//ft_bzero((void *)p_arr, KMAX * (1 + ADD_POINT) * sizeof(t_vektr *));
	if (!(p = ft_create_points_of_relief(ground, (t_vektr **)p_arr)))
		return(FALSE);
	if (!(plgn = ft_create_poligons_of_relief((t_vektr **)p_arr)))
	{
		ft_del_vektor(&p);
		return(FALSE);
	}
	vis->points = p;
	vis->plgn = plgn;
	return(TRUE);
}
