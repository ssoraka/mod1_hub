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
	return (value);
}



void	ft_bicube_interpolate_x2(void *param, int j, int i, int k)
{
	t_ground *ground;
	int n;
	int tmp[4];

	ground = (t_ground *)param;
	ground->complex[k * (1 + ADD_POINT)][i * (1 + ADD_POINT)] = ground->simple[k][i];
	n = 1;
	while (n < ADD_POINT + 1)
	{
		if (i < IMAX)
			ground->complex[k * (1 + ADD_POINT)][i * (1 + ADD_POINT) + n] = ft_cube_interpolate(ground->simple[k] + i - 1, ((REAL)n / (1 + ADD_POINT)));
		else
		{
			tmp[0] = ground->simple[k][i - 1];
			tmp[1] = ground->simple[k][i];
			tmp[2] = ground->simple[k][i + 1];
			tmp[3] = ground->simple[k][i + 1];
			ground->complex[k * (1 + ADD_POINT)][i * (1 + ADD_POINT) + n] = ft_cube_interpolate(tmp, ((REAL)n / (1 + ADD_POINT)));
		}
		n++;
	}
}


void	ft_bicube_interpolate_x(void *param, int j, int i, int k)
{
	t_ground *ground;
	int n;
	int tmp[4];
	int value;

	ground = (t_ground *)param;
	ground->complex[(k) * (1 + ADD_POINT)][(i) * (1 + ADD_POINT)] = ground->simple[k][i];
	if (i > IMAX)
		return ;
	n = 1;
	while (n < ADD_POINT + 1)
	{
		value = ground->simple[K0][I0];
		tmp[0] = (i == 0) ? ground->simple[k][i] : ground->simple[k][i - 1];
		tmp[1] = ground->simple[k][i];
		tmp[2] = ground->simple[k][i + 1];
		tmp[3] = (i == IMAX) ? ground->simple[k][i + 1] : ground->simple[k][i + 2];
		//if (i > I0 && k > K0 && i < IMAX && k < KMAX)
		if (i > 0 &&  i < IMAX)
			value = ft_cube_interpolate(tmp, ((REAL)n / (1 + ADD_POINT)));
		ground->complex[k * (1 + ADD_POINT)][i * (1 + ADD_POINT) + n] = value;
		n++;
	}
}


void	ft_bicube_interpolate_y(void *param, int j, int i, int k)
{
	t_ground *ground;
	int n;
	int tmp[4];
	int value;

	ground = (t_ground *)param;
	tmp[1] = ground->complex[k * (1 + ADD_POINT)][i];
	tmp[2] = ground->complex[(k + 1) * (1 + ADD_POINT)][i];
	tmp[0] = (k == 0) ? tmp[1] : ground->complex[(k - 1) * (1 + ADD_POINT)][i];
	tmp[3] = (k == KMAX) ? tmp[2] : ground->complex[(k + 2) * (1 + ADD_POINT)][i];
	n = 1;
	while (n < ADD_POINT + 1)
	{
		value = ground->simple[K0][I0];
		if (k > 0 && k < KMAX)
			value = ft_cube_interpolate(tmp, ((REAL)n / (1 + ADD_POINT)));
		ground->complex[k * (1 + ADD_POINT) + n][i] = value;
		n++;
	}
}


int		**ft_create_complex_ground_from_simple(int  **simple_ground)
{
	t_point start;
	t_point end;
	t_ground ground;

	ground.simple = simple_ground;
	if (!(ground.complex = (int **)ft_mem_arr_new((KMAX + 2) * (1 + ADD_POINT), (IMAX + 2) * (1 + ADD_POINT), sizeof(int))))
		return (NULL);
	ft_fill_point(&start, 0, 0, 0);
	ft_fill_point(&end, 0, IMAX + 1, KMAX + 1);
	ft_cycle_cube((void *)(&ground), &ft_bicube_interpolate_x, &start, &end);
	ft_fill_point(&start, 0, I0, 0);
	ft_fill_point(&end, 0, (I0 + ADD_POINT) * (IMAX + 1), KMAX);
	ft_cycle_cube((void *)(&ground), &ft_bicube_interpolate_y, &start, &end);
	return (ground.complex);
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
	t_vektr *v;
	t_dpoint p;

	i = I0 * (1 + ADD_POINT);
	// printf("%d__", ground[i + 1]);
	while (i <= IMAX * (1 + ADD_POINT))
	{
		color = ft_grad_color(ground[i], MAP_HEIGTH2, COLOR_UP, COLOR_DOWN);
		//MAP_HEIGTH2 перевести на даблы и избавиться
		ft_fill_dpoint(&p, ft_return_heigth(ground[i]) + 0.00,
		(REAL)i / (1 + ADD_POINT) + 0.5,
		(REAL)k / (1 + ADD_POINT) + 0.5);
		if (!(v = ft_new_vektor2(p.x, p.y, p.z, color)))
		{
			ft_del_vektor(begin);
			return (FAIL);
		}
		v->next = *begin;
		*begin = v;
		i++;
	}
	return (SUCCESS);
}


t_vektr	*ft_create_points_of_relief(int  **ground, t_vektr **p)
{
	t_vektr *tmp;
	int k;
	int n;

	tmp = NULL;
	k = K0 * (1 + ADD_POINT);
	n = 0;
	while (n < (KMAX - K0) * (1 + ADD_POINT) + 1)
	{
		p[n] = NULL;
		if (ft_create_points_from_string(&(p[n]), ground[n + k], n + k) == FAIL)
			return (NULL);
		ft_points_push_back(p[n], tmp);
		tmp = p[n];
		n++;
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
	int i;

	i = 0;
	while (i < (IMAX - I0) * (1 + ADD_POINT)) //p1->next && p2->next)
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
		i++;
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
	while (k < (KMAX - K0) * (1 + ADD_POINT))
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
	t_vektr *p_arr[(KMAX - K0) * (1 + ADD_POINT) + 1];
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
