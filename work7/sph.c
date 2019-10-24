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
typedef struct		s_part
{
	REAL			density;
	REAL			press;
	REAL			mass;
	REAL			h;
	struct s_dpoint	a;
	struct s_dpoint	speed;
	struct s_vektr	p;
}					t_part;
*/

/*
gamma = 7;
p0 = 1; //начальная плотность жидкости
g = 10; //— ускорение свободного падения
//H — начальная высота столба жидкости
//y — вертикальная координата частицы
P0 = 200 * p0 * g * (H - y) / gamma;
*/


#define DIR_X 1
#define DIR_Y 2
#define DIR_Z 4
#define SCALAR (DIR_X + DIR_Y + DIR_Z)

//начальная плотность жидкости
#define DENSITY_0 1.0
//ускорение свободного падения
#define FG 10.0
//характеристика адиабаты
#define GAMMA 7
//радиус сферы у одной частицы
#define PART_H 0.5
//масса сферы одной частицы
#define PI 3.141592
#define PART_MASS_0 (PI * 0.75 * PART_H * PART_H * PART_H)
//высоту столба воды можно как-то иначе определить, тут ее на шару поставил...
#define PRESS_0 (200 * DENSITY_0 * FG * JMAX * DELTA / GAMMA)

REAL kernel_function_xyz(REAL ri, REAL rj, REAL h, int projection)
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


REAL ft_distanse(t_dpoint *pi, t_dpoint *pj, int projection)
{
	REAL r;

	r = 0.0;
	if (projection == SCALAR)
		r = sqrt(pow(pi->x - pj->x, 2.0) + pow(pi->y - pj->y, 2.0) + pow(pi->z - pj->z, 2.0));
	else if (projection == DIR_X)
		r = pi->x - pj->x;
	else if (projection == DIR_Y)
		r = pi->y - pj->y;
	else if (projection == DIR_Z)
		r = pi->z - pj->z;
	if (r < 0.0)
		r = -r;
	return (r);
}


REAL ft_kernel_function(t_dpoint *pi, t_dpoint *pj, REAL h, int projection)
{
	REAL r;
	REAL r_h;
	REAL k;
	REAL answer;

	r = ft_distanse(pi, pj, projection);
	r_h = r / h;
	if (r_h >= 2.0)
		return (0);
	k = 1.0 / (PI * h);
	if (r_h < 1.0)
		answer = k * (1.0 - 1.5 * r_h * r_h + 0.75 * r_h * r_h * r_h);
	else
		answer = k * 0.25 * pow((2.0 - r_h), 3);
	return (answer);


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



#define COUNT_NEAR_CELL_IN_ONE_DIMENSION 3
#define COUNT_NEAR_CELL (COUNT_NEAR_CELL_IN_ONE_DIMENSION * 3)

void	ft_comparison_part_with_lists(t_part *part, t_part **surround, void *param, void (*f)(void *, t_part *, t_part *))
{
	int n;
	int k;

	n = 0;
	if (!part || !surround)
		return ;
	while (n < COUNT_NEAR_CELL)
	{
		k = 0;
		while (k < COUNT_NEAR_CELL_IN_ONE_DIMENSION)
		{
			ft_comparison_part_with_list(part, surround[n] + k, param, f);
			k++;
		}
		n++;
	}

}

void	ft_comparison_list_with_lists(t_part *part, t_part **surround, void *param, void (*f)(void *, t_part *, t_part *))
{
	while (part)
	{
		ft_comparison_part_with_lists(part, surround, param, f);
		part = part->next;
	}
}




void	ft_fill_surrounding_of_cell_by_j_i(t_cpart *cell, int j, int i, int k)
{
	t_part ***parts;

	parts = cell->begin;
	cell->surround[0] = &parts[j + 1][i + 1][k - 1];
	cell->surround[1] = &parts[j + 1][i - 1][k - 1];
	cell->surround[2] = &parts[j - 1][i + 1][k - 1];
	cell->surround[3] = &parts[j - 1][i - 1][k - 1];
	cell->surround[4] = &parts[j + 1][i][k - 1];
	cell->surround[5] = &parts[j - 1][i][k - 1];
	cell->surround[6] = &parts[j][i + 1][k - 1];
	cell->surround[7] = &parts[j][i - 1][k - 1];
	cell->surround[8] = &parts[j][i][k - 1];
}



void	ft_first_density(void *param, t_part *part_i, t_part *part_j)
{
	part_i->density += part_j->mass * ft_kernel_function(&(part_i->pos.abs), &(part_j->pos.abs), PART_H, SCALAR);
}



/*
**	высчитываем начальную плотность для каждой частицы
*/
void	ft_init_density(void *param, int j, int i, int k)
{
	t_cpart cell;

	cell.begin = (t_part ***)param;
	cell.origin = &(cell.begin[j][i][k]);
	ft_fill_surrounding_of_cell_by_j_i(&cell, j, i, k);
	ft_comparison_list_with_lists(cell.origin, cell.surround, NULL, &ft_first_density);
}




//вычисляем давление для каждой частицы из плотности
gamma = 7;
p0 = 1; //начальная плотность жидкости
g = 10; //— ускорение свободного падения
//H — начальная высота столба жидкости
//y — вертикальная координата частицы
P0 = 200 * p0 * g * (H - y) / gamma;
while (i <= parts_count)
{
	parts[i].P = P0 * (power(parts[i].p / p0, gamma) + 0.0)
	i++;
}

//высчитываем плотность для каждой частицы
while (i <= parts_count)
{
	parts[i].a.x = 0.0;
	parts[i].a.y = 0.0;
	parts[i].a.z = 0.0;
	j = 0;
	while (j <= parts_count)
	{
		Пij = 0.0;
		//вроде это дивергенция скорости
		div_v = (parts[i].v.x - parts[j].v.x) * (parts[i].x - parts[j].x)
		+ (parts[i].v.y - parts[j].v.y) * (parts[i].y - parts[j].y)
		+ (parts[i].v.z - parts[j].v.z) * (parts[i].z - parts[j].z);
		if (div_v < 0.0)
		{
			e = 0.1;
			B = 1; //где-то 0
			a = 1; //(0.02 - 0.03)//коэфф искуственной вязкости
			// какая-то херота
			h = (parts[i].h + parts[j].h) / 2;
			muij = h * (parts[i].v.x - parts[j].v.x) * (parts[i].x - parts[j].x)
			/ ((parts[i].x - parts[j].x) + e * e * h * h)
			+ h * (parts[i].v.y - parts[j].v.y) * (parts[i].y - parts[j].y)
			/ ((parts[i].y - parts[j].y) + e * e * h * h)
			+ h * (parts[i].v.z - parts[j].v.z) * (parts[i].z - parts[j].z)
			/ ((parts[i].z - parts[j].z) + e * e * h * h)
			Pij = (parts[i].P + parts[j].P) / 2; //полусумма давлений
			cij = (ci + cj) / 2; // полусумма скоростей звука частиц
			Пij = muij * (B * muij - a * cij) / Pji;
		}

		//определяем ускорение
		parts[i].a.x += -parts[j].m * ft_kernel_xyz(parts[i].x, parts[j].x, H)
		* (parts[i].P / (parts[i].p * parts[i].p)
		+ parts[j].P / (parts[j].p * parts[j].p) + Пij)
		+ gx;
		parts[i].a.y +=...
		parts[i].a.z +=...
		j++;
	}
	//новая скорость
	i++;
}


while (i <= parts_count)
{
	parts[i].v.x += parts[i].a.x * delta_t;
	parts[i].v.y += parts[i].a.y * delta_t;
	parts[i].v.z += parts[i].a.z * delta_t;
	i++;
}


while (i <= parts_count)
{
	u.x = parts[i].v.x;
	u.y = parts[i].v.y;
	u.z = parts[i].v.z;
	j = 0;
	while (j <= parts_count)
	{
		Pij = (parts[i].P + parts[j].P) / 2;
		e = ?;
		u.x += e * parts[j].m * ft_kernel_xyz(parts[i].x, parts[j].x, H)
		* (parts[i].v.x - parts[j].v.x) / Pij;
		u.y += ...;
		u.z += ...;
		j++;
	}
	parts[i].x += u.x * delta_t;
	parts[i].y += u.y * delta_t;
	parts[i].z += u.z * delta_t;
	i++;
}



1
