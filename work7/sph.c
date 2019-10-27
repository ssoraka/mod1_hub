/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sph.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ssoraka <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/10/22 17:01:16 by ssoraka           #+#    #+#             */
/*   Updated: 2019/10/22 17:01:17 by ssoraka          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_mod1.h"

/*
нужно разобраться с граничными условиями
скорее всего придется стоить статичные частицы, которые буду обсчитывать,
но не буду двигать

1) создать структуру точки с 3-мя даблами

2) создать структуру частицы, которая будет иметь у себя
-давление
-плотность
-скорость
-ускорение
-изменение плотности
-координаты

3)структуры частиц должны при движении переставлять себя в список
можно добавлять себя в конец списка частиц, тогда не будет их повторного пробегания

-сначала вычисляем первоначальную плотность
-из плотности вычисляем давление
-из плотностей и давлений вычисляем ускорение
-из ускорений определяем новые скорости и по кругу

*/


/*
gamma = 7;
p0 = 1; //начальная плотность жидкости
g = 10; //— ускорение свободного падения
//H — начальная высота столба жидкости
//y — вертикальная координата частицы
P0 = 200 * p0 * g * (H - y) / gamma;
*/




REAL	kernel_function_xyz(REAL ri, REAL rj, REAL h, int projection)
{
	REAL r;
	REAL r_h;
	REAL k;
	REAL answer;

	r = rj - ri;
	if (r < 0.0)
		r = -r;
	r_h = r / h;
	if (r_h >= 2.0)
		return (0);
	k = 1.0 / (3.14 * h);
	if (r_h < 1.0)
		answer = k * (1.0 - 1.5 * r_h * r_h + 0.75 * r_h * r_h * r_h);
	else
		answer = k * 0.25 * pow((2.0 - r_h), 3);
	return (answer);
}


REAL	ft_norma_vektora(REAL x, REAL y, REAL z)
{
	REAL norma;

	norma = sqrt(x * x + y * y + z * z);
	return (norma);
}

REAL	ft_distanse(t_dpoint *pi, t_dpoint *pj, REAL radius, int projection)
{
	REAL r;

	r = ft_norma_vektora(pi->x - pj->x, pi->y - pj->y, pi->z - pj->z);
	if (r > radius || projection == SCALAR)
		return (r);
	else if (projection == DIR_X)
		r = pi->x - pj->x;
	else if (projection == DIR_Y)
		r = pi->y - pj->y;
	else if (projection == DIR_Z)
		r = pi->z - pj->z;
	//хз, надо ли в модуль определять...
	if (r < 0.0)
		r = -r;
	return (r);
}


REAL	ft_kernel_function(t_dpoint *pi, t_dpoint *pj, REAL h, int projection)
{
	REAL r;
	REAL r_h;
	REAL k;
	REAL answer;

	r = ft_distanse(pi, pj, 2.0 * h, projection);
	r_h = r / h;
	k = 1.0 / (PI * h);
	answer = 0.0;
	if (r_h < 1.0 && r_h > 0)
		answer = k * (1.0 - 1.5 * r_h * r_h + 0.75 * r_h * r_h * r_h);
	else if (r_h <= 2.0 && r_h >= 1.0)
		answer = k * 0.25 * pow((2.0 - r_h), 3);
	return (answer);


}


void	ft_use_function(t_part *begin, void *param, void (*f)(t_part *, void *))
{
	if (!begin || !f)
		return ;
	while (begin)
	{
		f(begin, param);
		begin = begin->next;
	}
}

void	ft_use_function2(t_part **begin, void *param, void (*f)(t_part **, t_part *, void *))
{
	t_part **prev;
	t_part *part;

	if (!begin || !f)
		return ;
	prev = begin;
	part = *begin;
	while (part)
	{
		f(prev, part, param);
		prev = &part;
		part = part->next;
	}
}

void	ft_comparison_part_with_list(t_part *part1, t_part *begin2, void *param, void (*f)(void *, t_part *, t_part *))
{
	if (!begin2 || !part1 || !f)
		return ;
	while (begin2)
	{
		if (part1 != begin2)
			f(param, part1, begin2);
		begin2 = begin2->next;
	}
}





void	ft_comparison_part_with_lists(t_part *part, t_part **surround, void *param, void (*f)(void *, t_part *, t_part *))
{
	int n;
	int k;

	if (!part || !surround)
		return ;
	n = 0;
	while (n < COUNT_NEAR_CELL)
	{
		//для 2-мерного случая 0, для 3-ехмерного случая 1
		k = - COUNT_NEAR_CELL_IN_ONE_DIMENSION / 3;
		while (k < COUNT_NEAR_CELL_IN_ONE_DIMENSION - 1)
		{
			ft_comparison_part_with_list(part, surround[n] + k, param, f);
			k++;
		}
		n++;
	}

}

void	ft_comparison_list_with_lists(t_cpart *cell, void *param, void (*f)(void *, t_part *, t_part *))
{
	while (cell->origin)
	{
		ft_comparison_part_with_lists(cell->origin, cell->surround, param, f);
		cell->origin = cell->origin->next;
	}
}




void	ft_fill_surrounding_of_cell_by_j_i(t_cpart *cell, int j, int i, int k)
{
	t_part ****parts;

	parts = cell->begin;
	cell->surround[0] = parts[j + 1][i + 1][k];
	cell->surround[1] = parts[j + 1][i - 1][k];
	cell->surround[2] = parts[j - 1][i + 1][k];
	cell->surround[3] = parts[j - 1][i - 1][k];
	cell->surround[4] = parts[j + 1][i][k];
	cell->surround[5] = parts[j - 1][i][k];
	cell->surround[6] = parts[j][i + 1][k];
	cell->surround[7] = parts[j][i - 1][k];
	cell->surround[8] = parts[j][i][k];
}


/*
**	формула для начальной плотности для каждой частицы
*/
void	ft_first_density(void *param, t_part *part_i, t_part *part_j)
{
	part_i->density += part_j->mass * ft_kernel_function(&(part_i->pos.abs), &(part_j->pos.abs), PART_H, SCALAR);
}


void	ft_fill_param_of_part(t_part *part, void *param)
{
	part->h = PART_H;
	part->c = SPEED_OF_SOUND_C;
	part->mass = PART_MASS_0;
	part->press_0 = PRESS_0;
	if (part->type == WATER)
		part->speed.x = U_CONST;
	else
		part->density = DENSITY_0;
}


/*
**	высчитываем начальную плотность для каждой частицы
*/
void	ft_init_density(void *param, int j, int i, int k)
{
	t_cpart cell;

	cell.begin = (t_part ****)param;
	cell.origin = cell.begin[j][i][k];
	ft_fill_surrounding_of_cell_by_j_i(&cell, j, i, k);
	ft_comparison_list_with_lists(&cell, NULL, &ft_first_density);
}

void	ft_init_parts(void *param, int j, int i, int k)
{
	t_part *begin;

	begin = ((t_part ****)param)[j][i][k];
	ft_use_function(begin, NULL, &ft_fill_param_of_part);
}
/*
**	формула для плотности каждой частицы p = p + dp/dt
*/
void	ft_recalk_delta_density(void *param, t_part *part_i, t_part *part_j)
{
	part_i->delta_density += part_j->mass
	* ((part_i->speed.x - part_j->speed.x)
	* ft_kernel_function(&(part_i->pos.abs), &(part_j->pos.abs), part_j->h, DIR_X)
	+ (part_i->speed.y - part_j->speed.y)
	* ft_kernel_function(&(part_i->pos.abs), &(part_j->pos.abs), part_j->h, DIR_Y)
	+ (part_i->speed.z - part_j->speed.z)
	* ft_kernel_function(&(part_i->pos.abs), &(part_j->pos.abs), part_j->h, DIR_Z));
}


void	ft_change_density(t_part *part, void *param)
{
	part->density += part->delta_density * deltat;
	part->delta_density = 0.0;
}

/*
**	пересчет плотности для ячейки
*/
void	ft_new_density(void *param, int j, int i, int k)
{
	t_cpart cell;

	cell.begin = (t_part ****)param;
	cell.origin = cell.begin[j][i][k];
	ft_fill_surrounding_of_cell_by_j_i(&cell, j, i, k);
	ft_comparison_list_with_lists(&cell, NULL, &ft_recalk_delta_density);
	ft_use_function(cell.begin[j][i][k], NULL, &ft_change_density);
}



/*
**	формула для давления каждой частицы P = B [(p/p0)^7 - 1]
**	B = 200*p0*g(H-y)/7
**	p0 = 1 начальная плотность жидкости
**	g = 10 — ускорение свободного падения
**	H — начальная высота столба жидкости
**	y — вертикальная координата частицы
**	формула для давления каждой частицы P = p0*c*c/7 [(p/p0)^7 - 1]
*/

void	ft_recalk_pressure(t_part *part, void *param)
{
	part->press = part->c * part->c * DENSITY_0 / GAMMA
	* (pow(part->density / DENSITY_0, GAMMA) - 1.0) + 0.0;
}


/*
**	пересчет давления для ячейки
*/
void	ft_new_pressure(void *param, int j, int i, int k)
{
	t_part *begin;

	begin = ((t_part ****)param)[j][i][k];
	ft_use_function(begin, NULL, &ft_recalk_pressure);
}




REAL	ft_calc_fake_viscosity(t_part *p_i, t_part *p_j, t_dpoint *d_speed, t_dpoint *d_pos)
{
	REAL viskosity;
	REAL mu_ij; //коэффициент динамической вязкости
	REAL press_ij;
	REAL c_ij;
	REAL h_ij;

	h_ij = (p_j->h + p_j->h) / 2; // полусумма длин сглаживания частиц
	c_ij = (p_j->c + p_j->c) / 2; // полусумма скоростей звука частиц
	press_ij = (p_i->press + p_j->press) / 2; //полусумма давлений
	c_ij = (p_j->c + p_j->c) / 2; // полусумма скоростей звука частиц
	mu_ij = h_ij
	* ((d_speed->x * d_pos->x) / (d_pos->x + CONST_E * CONST_E * h_ij * h_ij)
	+ (d_speed->y * d_pos->y) / (d_pos->y + CONST_E * CONST_E * h_ij * h_ij)
	+ (d_speed->z * d_pos->z) / (d_pos->z + CONST_E * CONST_E * h_ij * h_ij));
	viskosity = mu_ij * (CONST_B * mu_ij - CONST_A * c_ij) / press_ij;
	return (viskosity);
}


REAL	ft_return_fake_viscosity(t_part *p_i, t_part *p_j)
{
	REAL viskosity;
	REAL div_speed;
	t_dpoint d_speed;
	t_dpoint d_pos;

	viskosity = 0.0;
	d_speed.y = p_i->speed.y - p_j->speed.y;
	d_speed.x = p_i->speed.x - p_j->speed.x;
	d_speed.z = p_i->speed.z - p_j->speed.z;
	d_pos.y = p_i->pos.abs.y - p_j->pos.abs.y;
	d_pos.x = p_i->pos.abs.x - p_j->pos.abs.x;
	d_pos.z = p_i->pos.abs.z - p_j->pos.abs.z;
	div_speed = d_speed.x * d_pos.x + d_speed.y * d_pos.y + d_speed.z * d_pos.z;
	if (div_speed < 0)
		ft_calc_fake_viscosity(p_i, p_j, &d_speed, &d_pos);
	return (viskosity);
}



void	ft_recalk_delta_speed(void *param, t_part *part_i, t_part *part_j)
{
	REAL tmp;

	tmp = (part_i->press / (part_i->density * part_i->density)
	+ part_j->press / (part_j->density * part_j->density)
	+ ft_return_fake_viscosity(part_i, part_j)) * part_j->mass;
	part_i->a.x += tmp
	* ft_kernel_function(&(part_i->pos.abs), &(part_j->pos.abs), part_i->h, DIR_X);
	part_i->a.y += tmp
	* ft_kernel_function(&(part_i->pos.abs), &(part_j->pos.abs), part_i->h, DIR_Y);
	part_i->a.z += tmp
	* ft_kernel_function(&(part_i->pos.abs), &(part_j->pos.abs), part_i->h, DIR_Z);
}

void	ft_change_speeds(t_part *part, void *param)
{
	REAL tmp;
	part->speed.x += (part->a.x + g.x) * deltat;
	part->speed.y += (part->a.y + g.y) * deltat;
	part->speed.z += (part->a.z + g.z) * deltat;
	ft_fill_dpoint(&(part->a), 0.0, 0.0, 0.0);
	tmp = ft_norma_vektora(part->speed.x, part->speed.y, part->speed.z);
	if (tmp > norm_speed)
		norm_speed = tmp;
}


/*
**	пересчет скоростей для частицы
*/
void	ft_new_speeds(void *param, int j, int i, int k)
{
	t_cpart cell;

	cell.begin = (t_part ****)param;
	cell.origin = cell.begin[j][i][k];
	ft_fill_surrounding_of_cell_by_j_i(&cell, j, i, k);
	ft_comparison_list_with_lists(&cell, NULL, &ft_recalk_delta_speed);
	ft_use_function(cell.begin[j][i][k], NULL, &ft_change_speeds);
}


/*
**	пересчет координат для частицы
*/


void	ft_change_coordinates(t_part *part, void *param)
{
	if (part->type == WATER)
	{
		part->pos.abs.x += (part->speed.x + CONST_EP * part->delta_pos.x) * deltat;
		part->pos.abs.y += (part->speed.y + CONST_EP * part->delta_pos.y) * deltat;
		part->pos.abs.z += (part->speed.z + CONST_EP * part->delta_pos.z) * deltat;
	}
	ft_fill_dpoint(&(part->delta_pos), 0.0, 0.0, 0.0);
}

void	ft_change_cells(t_part **prev, t_part *part, void *param)
{
	if (ft_part_change_cell(part))
		ft_replace_part(prev, part, (t_part ****)param);
}



/*
//простой вариант
void	ft_new_coordinates(void *param, int j, int i, int k)
{
	t_part *begin;

	begin = ((t_part ****)param)[j][i][k];
	ft_use_function(begin, NULL, &ft_change_coordinates);
}
*/


//сложный вариант
void	ft_recalk_delta_coord(void *param, t_part *part_i, t_part *part_j)
{
	REAL tmp;

	tmp = part_j->mass * 2.0 / (part_i->press + part_j->press);
	part_i->delta_pos.x += tmp * (part_i->speed.x - part_j->speed.x)
	* ft_kernel_function(&(part_i->pos.abs), &(part_j->pos.abs), part_i->h, DIR_X);
	part_i->delta_pos.y += tmp * (part_i->speed.y - part_j->speed.y)
	* ft_kernel_function(&(part_i->pos.abs), &(part_j->pos.abs), part_i->h, DIR_Y);
	part_i->delta_pos.z += tmp * (part_i->speed.z - part_j->speed.z)
	* ft_kernel_function(&(part_i->pos.abs), &(part_j->pos.abs), part_i->h, DIR_Z);
}


void	ft_new_coordinates(void *param, int j, int i, int k)
{
	t_cpart cell;

	cell.begin = (t_part ****)param;
	cell.origin = cell.begin[j][i][k];
	ft_fill_surrounding_of_cell_by_j_i(&cell, j, i, k);
	ft_comparison_list_with_lists(&cell, NULL, &ft_recalk_delta_coord);
	ft_use_function(cell.begin[j][i][k], NULL, &ft_change_coordinates);
	ft_use_function2(&(cell.begin[j][i][k]), param, &ft_change_cells);
}



/*
**	типовой расчет частиц
*/

void	ft_solve_and_move_parts(void)
{
	t_point start;
	t_point end;

	deltat = ft_time_control(SPEED_OF_SOUND_C, norm_speed, min_h);
	ft_fill_point(&start, 1, 1, 1);
	ft_fill_point(&end, JMAX, IMAX, KMAX);
	ft_cycle_cube(NULL, &ft_new_density, &start, &end);
	ft_cycle_cube(NULL, &ft_new_speeds, &start, &end);
	ft_cycle_cube((void *)parts, &ft_new_coordinates, &start, &end);
}


void	ft_init_first_value_of_part_parameters(void)
{
	t_point start;
	t_point end;

	ft_fill_point(&start, 0, 0, 0);
	ft_fill_point(&end, JMAX + 1, IMAX + 1, KMAX + 1);
	ft_cycle_cube(NULL, &ft_init_parts, &start, &end);
	ft_fill_point(&start, 1, 1, 1);
	ft_fill_point(&end, JMAX, IMAX, KMAX);
	ft_cycle_cube(NULL, &ft_init_density, &start, &end);
}




//
