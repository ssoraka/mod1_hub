/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parts.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ssoraka <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/10/08 14:28:01 by ssoraka           #+#    #+#             */
/*   Updated: 2019/10/08 14:28:02 by ssoraka          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_mod1.h"

void	ft_create_new_water_in_cell(void *param, int j, int i, int k)
{
	t_point p;
	int xyz[3];
	int ijk[3];
	int parts_count;
	int n;

	parts_count = *((int *)param);
	n = parts_count * parts_count * parts_count;
	while (n > 0)
	{
		n--;
		ijk[0] = n / (parts_count * parts_count);
		ijk[2] = n % parts_count;
		ijk[1] = (n % (parts_count * parts_count)) / parts_count;

		xyz[0] = dx / parts_count * ijk[0] + dx / parts_count / 2 + dx * (i - 1);
		xyz[1] = dy / parts_count * ijk[1] + dy / parts_count / 2 + dy * (j - 1);
		xyz[2] = dz / parts_count * ijk[2] + dz / parts_count / 2 + dz * (k - 1);
		ft_fill_point(&p, xyz[1], xyz[0], xyz[2]);
		if (!ft_add_vektor2((void *)&(parts[j][i][k]), &p, WATER_COLOR))
			ft_del_all_print_error(MSG_ERROR1);
		//printf("%d_%d_%d_%d\n", n, ijk[1], ijk[0], ijk[2]);
	}
}


/*
REAL	ft_move_parts_x(t_vektr *p, int j, int i, int k)
{
	REAL x1;
	REAL u;

	x1 = i * dx;

	//printf("x1 = %d_%lf_%lf_%lf\n", j, y2, p1->y ,y1);
	u = (speed_u[j][i + 1][k] - speed_u[j][i][k]) / (dx)
	* (p->x - x1) + speed_u[j][i][k];
	return (u);
	//printf("%lf + %d\n",p1->otn_x, i, j);
}


REAL	ft_move_parts_y(t_vektr *p, int j, int i, int k)
{
	REAL y1;
	REAL v;

	y1 = j * dy;

	v = (speed_v[j + 1][i][k] - speed_v[j][i][k]) / (dy)
	* (p->y - y1) + speed_v[j][i][k];
	return (v);
}


REAL	ft_move_parts_z(t_vektr *p, int j, int i, int k)
{
	REAL z1;
	REAL w;

	z1 = k * dz;

	w = (speed_w[j][i][k + 1] - speed_w[j][i][k]) / (dz)
	* (p->z - z1) + speed_w[j][i][k];
	return (w);
}
*/

void	ft_move_part(t_dpoint *part, t_point *jik, REAL deltat)
{
	int i;
	int j;
	int k;

	i = jik->x;
	j = jik->y;
	k = jik->z;

	part->x = part->x + deltat * ((speed_u[j][i][k] - speed_u[j][i - 1][k])
	/ dx * (part->x - (i - 1) * dx) + speed_u[j][i - 1][k]);
	part->y = part->y + deltat * ((speed_v[j][i][k] - speed_v[j - 1][i][k])
	/ dy * (part->y - (j - 1) * dy) + speed_v[j - 1][i][k]);
	part->z = part->z + deltat * ((speed_w[j][i][k] - speed_w[j][i][k - 1])
	/ dz * (part->z - (k - 1) * dz) + speed_w[j][i][k - 1]);

	jik->x = (int)(part->x / dx) + 1;
	jik->y = (int)(part->y / dy) + 1;
	jik->z = (int)(part->z / dz) + 1;
}


void	ft_replace_part(t_vektr **begin, t_vektr *prev, t_vektr **p, t_point *jik)
{
	t_vektr *tmp;

	tmp = *p;
	if (*begin == *p)
	{
		*begin = (*p)->next;
		*p = *begin;
	}
	else
	{
		prev->next = (*p)->next;
		*p = prev;
	}
	tmp->next = parts[jik->y][jik->x][jik->z];
	parts[jik->y][jik->x][jik->z] = tmp;
}

void	ft_move_parts(void *param, int j, int i, int k)
{
	t_vektr **begin;
	t_vektr *prev;
	t_vektr *p;
	t_point p_jik;

/////////////////////////////////////////
//идея была в том, чтоб двигать воду только на поверхности
//но может образоваться воздушный карман
//	if (!ft_is_surface(flags[j][i][k]))
//		return ;
/////////////////////////////////////////
	begin = &(parts[j][i][k]);
	p = *(begin);

	while (p)
	{
		//printf("%lf %lf %lf\n", p->y, p->x, p->z);
		if (p->iteration == iteration)
		{
			prev = p;
			p = p->next;
		}
		else
		{
			p->iteration = iteration;
			ft_fill_point(&p_jik, j, i, k);
			ft_move_part(&(p->abs), &p_jik, deltat);
			if (p_jik.y != j || p_jik.x != i || p_jik.z != k)
				ft_replace_part(begin, prev, &p, &p_jik);
		}
	}
}


void	ft_mark_water_on_map(void *param, int j, int i, int k)
{
	if (map[j][i][k] == OBSTACLES || map[j][i][k] == BOUNDARY)
		return ;
	else if (parts[j][i][k])
		map[j][i][k] = WATER;
	else
		map[j][i][k] = EMPTY;
}


void	ft_recalk_parts(void)
{
	t_point start;
	t_point end;

	ft_fill_point(&start, 1, 1, 1);
	ft_fill_point(&end, jmax, imax, kmax);
	iteration++;
	ft_cycle_cube(NULL, &ft_move_parts, &start, &end);
	ft_cycle_cube(NULL, &ft_mark_water_on_map, &start, &end);
}
