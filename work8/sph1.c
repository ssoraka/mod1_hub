/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sph2.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ssoraka <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/11/23 13:45:44 by ssoraka           #+#    #+#             */
/*   Updated: 2019/11/23 13:45:45 by ssoraka          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_mod1.h"

#define DENSITY_ALREADY_CALC 1
#define SPEED_ALREADY_CALC 2
#define POS_ALREADY_CALC 3


/*
**	я большой молодец, надо сделать функцию, которая будет применять функции
**	частицы к соседу по всем соседям
**
*/






REAL	ft_vekt_norm2(t_dpoint *v1, t_dpoint *v2)
{
	return (v1->x * v2->x + v1->y * v2->y + v1->z * v2->z);
}

REAL	ft_vekt_norm(t_dpoint *v)
{
	return (sqrt(v->x * v->x + v->y * v->y + v->z * v->z));
}

void	ft_vekt_vekt_multiplication(t_dpoint *v1, t_dpoint *v2, t_dpoint *answer)
{
	ft_fill_dpoint(answer, v1->y * v2->y, v1->x * v2->x, v1->z * v2->z);
}

void	ft_vekt_scalar_multiplication(t_dpoint *v1, REAL v2, t_dpoint *answer)
{
	ft_fill_dpoint(answer, v1->y * v2, v1->x * v2, v1->z * v2);
}

void	ft_vekt_difference(t_dpoint *v1, t_dpoint *v2, t_dpoint *answer)
{
	ft_fill_dpoint(answer, v1->y - v2->y, v1->x - v2->x, v1->z - v2->z);
}

void	ft_vekt_summ(t_dpoint *v1, t_dpoint *v2, t_dpoint *answer)
{
	ft_fill_dpoint(answer, v1->y + v2->y, v1->x + v2->x, v1->z + v2->z);
}

REAL	ft_kernel_function5(REAL h, REAL r)
{
	REAL r_h;
	REAL k;
	REAL answer;

	r_h = r / h;
	k = 1.0 / (PI * h * h * h);
	answer = 0.0;
	if (r_h < 1.0 && r_h > 0)
		answer = k * (1.0 - 1.5 * r_h * r_h + 0.75 * r_h * r_h * r_h);
	else if (r_h <= 2.0 && r_h >= 1.0)
		answer = k * 0.25 * pow((2.0 - r_h), 3);
	return (answer);
}

REAL	ft_derivative_kernel_function2(REAL h, REAL r)
{
	REAL r_h;
	REAL k;
	REAL answer;

	r_h = r / h;
	k = 1.0 / (PI * h * h * h * h);
	answer = 0.0;
	if (r_h < 1.0 && r_h > 0)
		answer = k * (-3.0 * r_h + 2.25 * r_h * r_h);
	else if (r_h <= 2.0 && r_h >= 1.0)
		answer = -k * 0.75 * pow((2.0 - r_h), 2);
	return (answer);
}

REAL	ft_derivative2_kernel_function2(REAL h, REAL r)
{
	REAL r_h;
	REAL k;
	REAL answer;

	r_h = r / h;
	k = 1.0 / (PI * h * h * h * h * h);
	answer = 0.0;
	if (r_h < 1.0 && r_h > 0)
		answer = k * (-3.0 + 4.5 * r_h);
	else if (r_h <= 2.0 && r_h >= 1.0)
		answer = k * 0.75 * (2.0 - r_h);
	return (answer);
}

int		ft_is_this_neighbor_was_not_add(t_part *p_i, t_part *p_j)
{
	int i;
	int n;

	i = 0;
	n = p_i->neighs.neigh_count;
	while(i < n)
	{
		if (p_i->neighs.neigh[i].part_j == p_j)
			return (FALSE);
		i++;
	}
	return (TRUE);
}


t_neigh	*ft_add_part_as_a_neighbor(t_part *p_i, t_neigh *n_j)
{
	t_neigh		*n;

	n = NULL;
	if (p_i->neighs.neigh_count + 1 < MAX_NEIGHBORS)
	{
		n = &(p_i->neighs.neigh[p_i->neighs.neigh_count]);
		(p_i->neighs.neigh_count)++;
		ft_memcpy((void *)n, (void *)n_j, sizeof(t_neigh));
	}
	return (n);
}

void	ft_check_and_add_parts_as_neighbors(void *param, t_part *p_i, t_part *p_j)
{
	t_neigh		tmp;
	t_dpoint	r;
	REAL		len;
	t_neigh		*n_ij;
	t_neigh		*n_ji;


	////////////
	p_i->density = p_i->mass;
	////////////
	if (p_i == p_j)
		return ;
	tmp.stage = 0;
	tmp.h_ij = (p_i->h + p_j->h) / 2.0;
	ft_vekt_difference(&(p_i->pos.abs), &(p_j->pos.abs), &r);
	len = ft_vekt_norm(&r);
	tmp.w_ij = ft_kernel_function5(tmp.h_ij, len);
	if (tmp.w_ij > 0.0 && ft_is_this_neighbor_was_not_add(p_i, p_j))
	{
		ft_vekt_scalar_multiplication(&r, (ft_derivative_kernel_function2(tmp.h_ij, len) / len), &(tmp.nabla_w_ij));
		tmp.part_j = p_j;
		n_ij = ft_add_part_as_a_neighbor(p_i, &tmp);
		if (p_i == p_j)
			return ;
		ft_vekt_scalar_multiplication(&(tmp.nabla_w_ij), -1.0, &(tmp.nabla_w_ij));
		tmp.part_j = p_i;
		n_ji = ft_add_part_as_a_neighbor(p_j, &tmp);
		if (n_ji && n_ij)
		{
			n_ij->neigh_clone = n_ji;
			n_ji->neigh_clone = n_ij;
		}
	}
}

/*
**	формула для плотности каждой частицы p = p + dp/dt
*/
void	ft_recalk_delta_density2(void *param, t_part *p_i, t_neigh *n_j)
{
	REAL		tmp;
	t_dpoint	d_speed;

	n_j->stage = DENSITY_ALREADY_CALC;
	ft_vekt_difference(&(p_i->speed), &(n_j->part_j->speed), &d_speed);
	tmp = ft_vekt_norm2(&d_speed, &(n_j->nabla_w_ij));

	p_i->delta_density += n_j->part_j->mass * tmp;

	if (!(n_j->neigh_clone))
		return ;
	n_j->neigh_clone->stage = DENSITY_ALREADY_CALC;
	n_j->part_j->delta_density += p_i->mass * tmp;
}

void	ft_recalk_delta_density_if_needs(void *param, t_part *p_i, t_neigh *n_j)
{

	if (n_j->stage == DENSITY_ALREADY_CALC)
		return ;

	ft_recalk_delta_density2(param, p_i, n_j);
}

void	ft_recalk_pressure(t_part *part, void *param)
{
	//part->press = part->c * part->c * part->density_0 / GAMMA
	//* (pow(part->density / part->density_0, GAMMA) - 1.0) + 100000.0;
	//part->press = DENSITY_0 * SPEED_OF_SOUND_C * SPEED_OF_SOUND_C / GAMMA * DELTA * 3
	//* (pow((part->density + part->delta_density) / part->density, GAMMA) - 1.0);
	//B https://ru.wikipedia.org/wiki/%D0%9E%D0%B1%D1%8A%D1%91%D0%BC%D0%BD%D1%8B%D0%B9_%D0%BC%D0%BE%D0%B4%D1%83%D0%BB%D1%8C_%D1%83%D0%BF%D1%80%D1%83%D0%B3%D0%BE%D1%81%D1%82%D0%B8
	//if (part->density < DENSITY_0)
	//	part->press = 0.0;
	//else
		part->press = 220000000.0
		//part->press = 1200000.0
		* (pow((part->density) / DENSITY_0, GAMMA) - 1.0);


}

//тут надо бы пересчитать скорость звука и радиус сглаживания
void	ft_change_density_and_pressure_and_color(t_part *part, void *param)
{
	//хз, надо уточнять.... SPEED_OF_SOUND_C
	/*REAL temp_press;
	temp_press = part->press;
	temp_press = ABS(temp_press - part->press);
	part->c = sqrt(temp_press / part->delta_density);*/

	part->density += part->delta_density * deltat;
	ft_recalk_pressure(part, param);
	part->delta_density = 0.0;
}


REAL	ft_return_fake_viscosity2(t_part *p_i, t_neigh *n_j)
{
	REAL viskosity;
	t_dpoint d_speed;
	t_dpoint d_pos;
	REAL mu_ij; //коэффициент динамической вязкости
	REAL press_ij;
	REAL c_ij;

	viskosity = 0.0;
	ft_vekt_difference(&(p_i->speed), &(n_j->part_j->speed), &d_speed);
	ft_vekt_difference(&(p_i->pos.abs), &(n_j->part_j->pos.abs), &d_pos);
	if (ft_vekt_norm2(&d_pos, &d_speed) < 0.0)
	{
		press_ij = (p_i->press + n_j->part_j->press) / 2.0; //полусумма давлений
		c_ij = (p_i->c + n_j->part_j->c) / 2.0; // полусумма скоростей звука частиц
		mu_ij = n_j->h_ij * ft_vekt_norm2(&d_pos, &d_speed)
		/ (ft_vekt_norm2(&d_pos, &d_pos) + CONST_E * CONST_E * n_j->h_ij * n_j->h_ij);
		//printf("mu__%lf__\n", mu_ij);
		viskosity = mu_ij * (CONST_B * mu_ij - CONST_A * c_ij) / press_ij;
	}
	return (viskosity);
}


void	ft_recalk_delta_speed2(void *param, t_part *p_i, t_neigh *n_j)
{
	REAL tmp;
	t_dpoint a_tmp;

	n_j->stage = SPEED_ALREADY_CALC;

	tmp = p_i->press / (p_i->density * p_i->density)
	+ n_j->part_j->press / (n_j->part_j->density * n_j->part_j->density)
	+ ft_return_fake_viscosity2(p_i, n_j);

	ft_vekt_scalar_multiplication(&(n_j->nabla_w_ij), -tmp * n_j->part_j->mass, &a_tmp);
	ft_vekt_summ(&(p_i->a), &a_tmp, &(p_i->a));

	if (!(n_j->neigh_clone))
		return ;
	n_j->neigh_clone->stage = SPEED_ALREADY_CALC;
	ft_vekt_scalar_multiplication(&(n_j->neigh_clone->nabla_w_ij), -tmp * p_i->mass, &a_tmp);
	ft_vekt_summ(&(n_j->part_j->a), &a_tmp, &(n_j->part_j->a));

}

void	ft_recalk_delta_speed_if_needs(void *param, t_part *p_i, t_neigh *n_j)
{
	if (p_i->type == OBSTACLES)
		return ;
	if (n_j->stage == SPEED_ALREADY_CALC)
		return ;
	if (p_i->press == 0.0 && n_j->part_j->press == 0.0)
		return ;
	ft_recalk_delta_speed2(param, p_i, n_j);
}



void	ft_recalk_smoothed_color_func_if_needs(void *param, t_part *p_i, t_neigh *n_j)
{
	if (p_i->type == OBSTACLES)
		return ;
	if (n_j->part_j->tension.is_surface)
		p_i->tension.smoothed_color += n_j->part_j->tension.color * n_j->w_ij;
	//ft_recalk_color_func(param, p_i, n_j);
}

/*
** остановился
*/

void	ft_recalk_normal_if_needs(void *param, t_part *p_i, t_neigh *n_j)
{
	if (p_i->type == OBSTACLES)
		return ;
	p_i->tension.normal.x -= (n_j->part_j->tension.smoothed_color - p_i->tension.smoothed_color) * n_j->nabla_w_ij.x;
	p_i->tension.normal.y -= (n_j->part_j->tension.smoothed_color - p_i->tension.smoothed_color) * n_j->nabla_w_ij.y;
	p_i->tension.normal.z -= (n_j->part_j->tension.smoothed_color - p_i->tension.smoothed_color) * n_j->nabla_w_ij.z;
	//ft_recalk_color_func(param, p_i, n_j);
}


void	ft_change_unit_normal(t_part *part, void *param)
{
	if (part->type == OBSTACLES)
		return ;
	ft_vekt_scalar_multiplication(&(part->tension.normal), 1.0 / ft_vekt_norm(&(part->tension.normal)), &(part->tension.unit_n));
}


void	ft_recalk_curvature_if_needs(void *param, t_part *p_i, t_neigh *n_j)
{
	if (p_i->type == OBSTACLES)
		return ;
	p_i->tension.k.x += (n_j->part_j->tension.normal.x - p_i->tension.normal.x) * n_j->nabla_w_ij.x;
	p_i->tension.k.y += (n_j->part_j->tension.normal.y - p_i->tension.normal.y) * n_j->nabla_w_ij.y;
	p_i->tension.k.z += (n_j->part_j->tension.normal.z - p_i->tension.normal.z) * n_j->nabla_w_ij.z;
	//ft_recalk_color_func(param, p_i, n_j);
}



void	ft_refresh_max_speed(REAL speed, REAL speed_c)
{
	if (speed > norm_speed)
		norm_speed = speed;
	if (speed_c > max_c)
		max_c = speed_c;
}

void	ft_change_speeds2(t_part *part, void *param)
{
	if (part->type != OBSTACLES)
	{
		//ft_vekt_summ(&(part->a), &g, &(part->a));
		//ft_vekt_scalar_multiplication(&(part->a), deltat, &(part->a));
		//ft_vekt_summ(&(part->speed), &(part->a), &(part->speed));

		part->speed.x += (part->a.x + g.x - SIGMA * part->tension.k.x * part->tension.normal.x) * deltat;
		part->speed.y += (part->a.y + g.y - SIGMA * part->tension.k.y * part->tension.normal.y) * deltat;
		part->speed.z += (part->a.z + g.z - SIGMA * part->tension.k.z * part->tension.normal.z) * deltat;

		if (ft_vekt_norm(&(part->speed)) > part->c * 0.1)
			ft_vekt_scalar_multiplication(&(part->speed), 0.1 * part->c / ft_vekt_norm(&(part->speed)), &(part->speed));
		ft_refresh_max_speed(ft_vekt_norm(&(part->speed)), part->c);
	}
	ft_fill_dpoint(&(part->a), 0.0, 0.0, 0.0);

}


void	ft_recalk_delta_coord2(void *param, t_part *p_i, t_neigh *n_j)
{
	REAL tmp;
	t_dpoint d_speed;

	n_j->stage = POS_ALREADY_CALC;

	tmp = n_j->part_j->mass * 2.0 / (p_i->press + n_j->part_j->press) * n_j->w_ij;
	ft_vekt_difference(&(p_i->speed), &(n_j->part_j->speed), &d_speed);
	ft_vekt_scalar_multiplication(&d_speed, tmp, &d_speed);
	ft_vekt_summ(&(p_i->delta_pos), &d_speed, &(p_i->delta_pos));
}


void	ft_recalk_delta_coord_if_needs(void *param, t_part *p_i, t_neigh *n_j)
{
	if (p_i->type == OBSTACLES)
		return ;
	if (n_j->stage == POS_ALREADY_CALC)
		return ;
	if ((p_i->press + n_j->part_j->press) == 0.0)
		return ;
	ft_recalk_delta_coord2(param, p_i, n_j);
}


#define COEFF_SPEED 0.5

void	ft_cheek_new_pos(t_part *part)
{
	if (part->pos.abs.x < DELTA)
	{
		part->pos.abs.x += DELTA - part->pos.abs.x;
		part->speed.x = -part->speed.x * COEFF_SPEED;
	}
	if (part->pos.abs.y < DELTA)
	{
		part->pos.abs.y += DELTA - part->pos.abs.y;
		part->speed.y = -part->speed.y * COEFF_SPEED;
	}
	if (part->pos.abs.z < DELTA)
	{
		part->pos.abs.z += DELTA - part->pos.abs.z;
		part->speed.z = -part->speed.z * COEFF_SPEED;
	}
	if (part->pos.abs.x >= (IMAX - 1) * DELTA)
	{
		part->pos.abs.x += (IMAX - 1) * DELTA - part->pos.abs.x;
		part->speed.x = -part->speed.x * COEFF_SPEED;
	}
	if (part->pos.abs.y >= (JMAX - 1) * DELTA)
	{
		part->pos.abs.y += (JMAX - 1) * DELTA - part->pos.abs.y;
		part->speed.y = -part->speed.y * COEFF_SPEED;
	}
	if (part->pos.abs.z >= (KMAX - 1) * DELTA)
	{
		part->pos.abs.z += (KMAX - 1) * DELTA - part->pos.abs.z;
		part->speed.z = -part->speed.z * COEFF_SPEED;
	}
}


void	ft_change_coordinates2(t_part *part, void *param)
{
	if (part->type == WATER)
	{
		//printf("%lf\n", part->pos.abs.y);

		part->pos.abs.x += (part->speed.x + CONST_EP * part->delta_pos.x) * deltat;
		part->pos.abs.y += (part->speed.y + CONST_EP * part->delta_pos.y) * deltat;
		part->pos.abs.z += (part->speed.z + CONST_EP * part->delta_pos.z) * deltat;
		//printf("_%lf_\n", part->pos.abs.y);
		ft_cheek_new_pos(part);
	}

	ft_fill_dpoint(&(part->delta_pos), 0.0, 0.0, 0.0);
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


void	ft_use_function_to_each_neighbors(t_part *begin, void *param, void (*f)(void *, t_part *, t_neigh *))
{
	int i;

	if (!begin || !f)
		return ;
	while (begin)
	{
		i = 0;
		while (i < begin->neighs.neigh_count)
		{
			f(param, begin, &(begin->neighs.neigh[i]));
			i++;
		}
		begin = begin->next;
	}
}

void	ft_clear_all_neighbors(t_part *part, void *param)
{
	part->neighs.neigh_count = 0;
	if (part->type == WATER)
	{
		ft_bzero((void *)(&part->tension), sizeof(t_tens));
		//надо это допилить и исправить...
		part->tension.is_surface = *((int *)param);
		part->tension.color = part->mass / part->density;
		//part->h = 1.20 * pow(part->mass / part->density, 0.3333);
		/*if (part->is_surface)
			//part->h = 1.20 * pow(part->mass / part->density, 0.3333);
			part->h = PART_H * 2.0;
		else
			part->h = PART_H;*/
	//	part->h = *((int *)param) * PART_H;
	//
	}
	//ft_bzero((void *)(&(part->neighs)), sizeof(t_neighs));
}

int		ft_is_part_of_surface(t_part ****parts, int j, int i, int k)
{
	int coeff;

	coeff = FALSE;
	if (j + 1 < JMAX - 1 && !parts[j + 1][i][k])
		coeff = TRUE;
	if (j - 1 > 1 && !parts[j - 1][i][k])
		coeff = TRUE;
	if (i + 1 < IMAX - 1 && !parts[j][i + 1][k])
		coeff = TRUE;
	if (i - 1 > 1 && !parts[j][i - 1][k])
		coeff = TRUE;
	if (k + 1 < KMAX - 1 && !parts[j][i][k + 1])
		coeff = TRUE;
	if (k - 1 > 1 && !parts[j][i][k - 1])
		coeff = TRUE;
	return (coeff);
}

void	ft_clear_neighbors(void *param, int j, int i, int k)
{
	t_part *parts;
	int is_surface;

	parts = ((t_part ****)param)[j][i][k];
	if (!parts)
		return ;

	is_surface = ft_is_part_of_surface((t_part ****)param, j, i, k);
	ft_use_function(parts, (void *)(&is_surface), &ft_clear_all_neighbors);
}


void	ft_new_delta_density(void *param, int j, int i, int k)
{
	t_part *parts;

	parts = ((t_part ****)param)[j][i][k];
	if (!parts)
		return ;
	ft_use_function_to_each_neighbors(parts, NULL, &ft_recalk_delta_density_if_needs);
}


void	ft_new_density_and_press_and_color(void *param, int j, int i, int k)
{
	t_part *parts;

	parts = ((t_part ****)param)[j][i][k];
	if (!parts)
		return ;
	ft_use_function(parts, NULL, &ft_change_density_and_pressure_and_color);
}



void	ft_new_delta_speeds(void *param, int j, int i, int k)
{
	t_part *parts;

	parts = ((t_part ****)param)[j][i][k];
	if (!parts)
		return ;
	ft_use_function_to_each_neighbors(parts, NULL, &ft_recalk_delta_speed_if_needs);
}


void	ft_new_smoothed_color_func(void *param, int j, int i, int k)
{
	t_part *parts;

	parts = ((t_part ****)param)[j][i][k];
	if (!parts)
		return ;
	ft_use_function_to_each_neighbors(parts, NULL, &ft_recalk_smoothed_color_func_if_needs);
}

void	ft_new_normal_to_surface(void *param, int j, int i, int k)
{
	t_part *parts;

	parts = ((t_part ****)param)[j][i][k];
	if (!parts)
		return ;
	ft_use_function_to_each_neighbors(parts, NULL, &ft_recalk_normal_if_needs);
}

void	ft_new_unit_normal_to_surface(void *param, int j, int i, int k)
{
	t_part *parts;

	parts = ((t_part ****)param)[j][i][k];
	if (!parts)
		return ;
	ft_use_function(parts, NULL, &ft_change_unit_normal);
}

void	ft_new_curvature_of_surface(void *param, int j, int i, int k)
{
	t_part *parts;

	parts = ((t_part ****)param)[j][i][k];
	if (!parts)
		return ;
	ft_use_function_to_each_neighbors(parts, NULL, &ft_recalk_curvature_if_needs);
}

void	ft_new_speeds2(void *param, int j, int i, int k)
{
	t_part *parts;

	parts = ((t_part ****)param)[j][i][k];
	if (!parts)
		return ;
	ft_use_function(parts, NULL, &ft_change_speeds2);
}


void	ft_new_delta_coordinates2(void *param, int j, int i, int k)
{
	t_part *parts;

	parts = ((t_part ****)param)[j][i][k];
	if (!parts)
		return ;
	ft_use_function_to_each_neighbors(parts, NULL, &ft_recalk_delta_coord_if_needs);
}





void	ft_use_function2(t_part **begin, void *param, void (*f)(t_part *, t_part **, void *))
{
	t_part prev_or;
	t_part *prev;
	t_part *next;

	if (!begin || !f)
		return ;
	prev = &prev_or;
	prev->next = *begin;
	while (*begin)
	{
		next = (*begin)->next;
		f(prev, begin, param);
		prev = *begin;
		*begin = next;
		//*begin = (*begin)->next;
	}
	*begin = prev_or.next;
}


void	ft_change_cells(t_part *prev, t_part **part, void *param)
{
	if (ft_part_change_cell(*part))
		ft_replace_part(prev, part, (t_part ****)param);
}


void	ft_new_coordinates2(void *param, int j, int i, int k)
{
	t_part *parts;

	parts = ((t_part ****)param)[j][i][k];
	if (!parts)
		return ;
	ft_use_function(parts, NULL, &ft_change_coordinates2);
	ft_use_function2(&(((t_part ****)param)[j][i][k]), param, &ft_change_cells);
}






void	ft_fill_surrounding_of_cell_by_j_i(t_cpart *cell, int j, int i, int k)
{
	t_part ****parts;

	parts = cell->begin;
	cell->surround[0] = &(parts[j + 1][i + 1][k]);
	cell->surround[1] = &(parts[j + 1][i - 1][k]);
	cell->surround[2] = &(parts[j - 1][i + 1][k]);
	cell->surround[3] = &(parts[j - 1][i - 1][k]);
	cell->surround[4] = &(parts[j + 1][i][k]);
	cell->surround[5] = &(parts[j - 1][i][k]);
	cell->surround[6] = &(parts[j][i + 1][k]);
	cell->surround[7] = &(parts[j][i - 1][k]);
	cell->surround[8] = &(parts[j][i][k]);
}

void	ft_comparison_part_with_list(t_part *part1, t_part *begin2, void *param, void (*f)(void *, t_part *, t_part *))
{
	if (!begin2 || !part1 || !f)
		return ;
	while (begin2)
	{
		//if (part1 != begin2)
			f(param, part1, begin2);
		begin2 = begin2->next;
	}
}

void	ft_comparison_part_with_lists(t_part *part, t_part ***surround, void *param, void (*f)(void *, t_part *, t_part *))
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
			ft_comparison_part_with_list(part, *(surround[n] + k), param, f);
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


void	ft_new_neighbors(void *param, int j, int i, int k)
{
	t_cpart cell;

	cell.begin = (t_part ****)param;
	cell.origin = cell.begin[j][i][k];
	if (!cell.origin)
		return ;
	ft_fill_surrounding_of_cell_by_j_i(&cell, j, i, k);
	ft_comparison_list_with_lists(&cell, NULL, &ft_check_and_add_parts_as_neighbors);
}




/*
**	типовой расчет частиц
*/

void	ft_solve_and_move_parts(void)
{
	t_point start;
	t_point end;

	deltat = ft_time_control(max_c, norm_speed, min_h);
	norm_speed = norm_speed * 0.1;
	max_c = max_c * 0.1;
	//printf("%lf\n", deltat);
	ft_fill_point(&start, 1, 1, 1);
	ft_fill_point(&end, JMAX, IMAX, KMAX);



	ft_cycle_cube((void *)parts, &ft_clear_neighbors, &start, &end);
	//long start_clock = clock();
	//надо допилить эту функцию
	ft_cycle_cube((void *)parts, &ft_new_neighbors, &start, &end);
	//long end_clock = clock();
	//printf("%ld\n", end_clock - start_clock);

	//считаем изменение плотности
	ft_cycle_cube((void *)parts, &ft_init_density, &start, &end);
	ft_cycle_cube((void *)parts, &ft_new_delta_density, &start, &end);

	//пересчитываем плотность и давление
	ft_cycle_cube((void *)parts, &ft_new_density_and_press_and_color, &start, &end);
	//считаем изменение скорости
	ft_cycle_cube((void *)parts, &ft_new_delta_speeds, &start, &end);
	//считаем силу поверхостного натяжения
	/*ft_cycle_cube((void *)parts, &ft_new_smoothed_color_func, &start, &end);
	ft_cycle_cube((void *)parts, &ft_new_normal_to_surface, &start, &end);
	ft_cycle_cube((void *)parts, &ft_new_unit_normal_to_surface, &start, &end);
	ft_cycle_cube((void *)parts, &ft_new_curvature_of_surface, &start, &end);*/
	//считаем скорости

	ft_cycle_cube((void *)parts, &ft_new_speeds2, &start, &end);
	//считаем изменение координат
	ft_cycle_cube((void *)parts, &ft_new_delta_coordinates2, &start, &end);

	ft_cycle_cube((void *)parts, &ft_new_coordinates2, &start, &end);
	//printf("%lf\n", deltat);


}


//сделать правильную инициализацию!!!

void	ft_fill_param_of_part(t_part *part, void *param)
{
	//PART_H
	//ft_putstr("1\n");
	part->h = PART_H * 1.00;//1.2 * pow(PART_MASS_0 / DENSITY_0, 1.0 / 3.0);
	part->mass = PART_MASS_0 * 1.0;
	//part->h = 1.3 * pow(part->mass / DENSITY_0, 1.0 / 3.0);
	part->c = SPEED_OF_SOUND_C;

	part->press_0 = 0.0;
	if (part->type == WATER)
	{
		;
		//part->speed.x = U_CONST;
		/*part->pos.abs.x += ((REAL)(rand() % 100)) / 500.0;
		part->pos.abs.y += ((REAL)(rand() % 100)) / 500.0;
		part->pos.abs.z += ((REAL)(rand() % 100)) / 500.0;*/
		/*part->speed.x += ((REAL)(rand() % 100)) / 500.0;
		part->speed.y += ((REAL)(rand() % 100)) / 500.0;
		part->speed.z += ((REAL)(rand() % 100)) / 500.0;*/
		//printf("%lf\n",  ((REAL)(rand() % 100)) / 500.0);
		//exit(0);
		part->density = part->mass;
	}
	else
//		part->h = PART_H * 1.0;
		part->density = DENSITY_0 * 1.00;// + ((REAL)(rand() % 100)) / 500.0;
}











/*
**	формула для начальной плотности для каждой частицы
*/
void	ft_first_density(void *param, t_part *p_i, t_neigh *n_j)
{
	if (p_i->type != WATER)
		return ;
	p_i->density += n_j->part_j->mass * n_j->w_ij;
	//p_i->delta_density = 0.0;
}

/*
**	высчитываем начальную плотность для каждой частицы
*/
void	ft_init_density(void *param, int j, int i, int k)
{
	t_part *parts;

	parts = ((t_part ****)param)[j][i][k];
	if (!parts)
		return ;
	ft_use_function_to_each_neighbors(parts, NULL, &ft_first_density);
}


void	ft_init_first_value_of_part_parameters(void)
{
	t_point start;
	t_point end;

	ft_fill_point(&start, 1, 1, 1);
	ft_fill_point(&end, JMAX, IMAX, KMAX);

	//ft_cycle_cube((void *)parts, &ft_clear_neighbors, &start, &end);
	//надо допилить эту функцию
	ft_cycle_cube((void *)parts, &ft_new_neighbors, &start, &end);

	ft_cycle_cube((void *)parts, &ft_init_density, &start, &end);
}


//
