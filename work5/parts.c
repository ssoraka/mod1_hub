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
	int jik[3];
	int parts_count;
	int n;

	parts_count = *((int *)param);
	n = parts_count * parts_count * parts_count;
	while (n > 0)
	{
		n--;
		jik[0] = n / (parts_count * parts_count);
		jik[2] = n % parts_count;
		jik[1] = n % (parts_count * parts_count) - jik[2];

		xyz[0] = dx / parts_count * jik[0] + dx / parts_count / 2 + dx * (i - 1);
		xyz[1] = dy / parts_count * jik[1] + dy / parts_count / 2 + dy * (j - 1);
		xyz[2] = dz / parts_count * jik[2] + dz / parts_count / 2 + dz * (k - 1);
		ft_fill_point(&p, xyz[1], xyz[0], xyz[2]);
		if (!ft_add_vektor2((void *)&(parts[j][i][k]), &p, WATER_COLOR))
			ft_del_all_print_error(MSG_ERROR1);
	}
}



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


void	ft_move_part(t_vektr *part, t_point *jik, int iteration, REAL deltat)
{
	part->iteration = iteration;
	part->x = part->x + deltat * ft_move_parts_x(part, jik->y, jik->x, jik->z);
	part->y = part->y + deltat * ft_move_parts_y(part, jik->y, jik->x, jik->z);
	part->z = part->z + deltat * ft_move_parts_y(part, jik->y, jik->x, jik->z);
	part->otn.x = (int)part->x;
	part->otn.y = (int)part->y;
	part->otn.z = (int)part->z;
	jik->x = (int)(part->otn.x / dx) + 1;
	jik->y = (int)(part->otn.y / dy) + 1;
	jik->z = (int)(part->otn.z / dz) + 1;
}


void	ft_mark_water_on_map(int j, int i, int k)
{
	if (i >= 1 && j >= 1 && k >= 1 && i <= imax && j <= jmax && k <= kmax)
		if (map[j][i][k] == EMPTY)
			map[j][i][k] = WATER;
}


void	ft_replace_part(t_vektr **prev, t_vektr *part, t_point *jik)
{

	if (*prev == part)
		*prev = part->next;
	else
		(*prev)->next = part->next;
	part->next = parts[jik->y][jik->x][jik->z];
	parts[jik->y][jik->x][jik->z] = part;

}

void	ft_move_parts(void *param, int j, int i, int k)
{
	t_vektr **parts;
	t_vektr *p;
	t_point p_jik;

	if (!ft_is_surface(flags[j][i][k]))
		return ;
	parts = (t_vektr **)param;
	p = *parts;
	while (p)
	{
		if (p->iteration < iteration)
		{
			ft_fill_point(&p_jik, j, i, k);
			ft_move_part(p, &p_jik, iteration, deltat);
			if (p_jik.y != j || p_jik.x != i || p_jik.z != k)
				ft_replace_part(parts, p, &p_jik);
			ft_mark_water_on_map(p_jik.y, p_jik.x, p_jik.z);
		}
		*parts = p;
		p = p->next;
	}
}




void	ft_recalk_parts(void)
{
	t_point start;
	t_point end;

	ft_fill_point(&start, 1, 1, 1);
	ft_fill_point(&end, jmax, imax, kmax);
	ft_cycle_cube(NULL, &ft_move_parts, &start, &end);
}
