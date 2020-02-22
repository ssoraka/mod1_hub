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


void	ft_try_add_part_as_neighbor(void *part_j, void *part_i)
{
	t_part		*p_i;
	t_part		*p_j;
	t_neigh		tmp;
	REAL		len;

	p_i = part_i;
	p_j = part_j;
	if (p_j->type == EMPTY)
		return ;
	tmp.h_ij = (g_param[p_i->type][F_H] + g_param[p_j->type][F_H]) / 2.0;
	ft_vekt_difference(&(p_i->pos), &(p_j->pos), &tmp.r_ij);
	len = ft_vekt_norm(&tmp.r_ij);
	tmp.w_ij = ft_kernel_function5(tmp.h_ij, len);
	if (tmp.w_ij > 0.0)
	{
		#ifdef CHANGE_FLUIDS
		if (p_i->type == MAGMA && p_j->type == WATER)
			p_i->type = MAGMA2;
	//	else if (p_i->type == WATER && p_j->type == MAGMA2)
	//		p_i->type = WATER2;
		#endif
		ft_vekt_scalar_multiplication(&tmp.r_ij, (ft_derivative_kernel_function2(tmp.h_ij, len) / len), &(tmp.nabla_w_ij));
		tmp.part_j = p_j;
		if (!ft_arr_add(&(p_i->neigh), (void *)&tmp))
			ft_del_all_print_error("need more memory");
	}
}



/*
**	формула для плотности каждой частицы p = p + dp/dt
*/

void	ft_calk_delta_density(void *neigh_j, void *part_i)
{
	REAL		tmp;
	t_dpoint	d_speed;
	t_part		*p_i;
	t_neigh		*n_j;

	p_i = (t_part *)part_i;
	n_j = (t_neigh *)neigh_j;
	ft_vekt_difference(&(p_i->speed), &(n_j->part_j->speed), &d_speed);
	tmp = ft_vekt_norm2(&d_speed, &(n_j->nabla_w_ij));

	p_i->delta_density += g_param[n_j->part_j->type][F_MASS] * tmp;
}

void	ft_recalk_pressure(t_part *part, void *param)
{
	//B https://ru.wikipedia.org/wiki/%D0%9E%D0%B1%D1%8A%D1%91%D0%BC%D0%BD%D1%8B%D0%B9_%D0%BC%D0%BE%D0%B4%D1%83%D0%BB%D1%8C_%D1%83%D0%BF%D1%80%D1%83%D0%B3%D0%BE%D1%81%D1%82%D0%B8
	part->press = g_param[part->type][F_PRESS] * (pow((part->density) / g_param[part->type][F_DENS], GAMMA) - 1.0);
	if (part->press < 0.0)
		part->press = 0.0;
	//if (part->type == MAGMA)
	//	part->press *= 20.0;
}

//тут надо бы пересчитать скорость звука и радиус сглаживания
void	ft_new_density_and_press_and_color(void *part, void *param)
{
	//хз, надо уточнять.... SPEED_OF_SOUND_C
	/*REAL temp_press;
	temp_press = part->press;
	temp_press = ABS(temp_press - part->press);
	part->c = sqrt(temp_press / part->delta_density);*/
	if (((t_part *)part)->type == EMPTY)
		return ;
	((t_part *)part)->density += ((t_part *)part)->delta_density * deltat;
	ft_recalk_pressure(((t_part *)part), param);
	((t_part *)part)->delta_density = 0.0;
}


REAL	ft_return_fake_viscosity2(t_part *p_i, t_neigh *n_j)
{
	REAL viskosity;
	REAL press_ij;
	REAL c_ij;

	viskosity = 0.0;
	if (ft_vekt_norm2(&(n_j->r_ij), &(n_j->u_ij)) < 0.0)
	{
		press_ij = (p_i->press + n_j->part_j->press) / 2.0; //полусумма давлений
		c_ij = (g_param[p_i->type][F_C] + g_param[n_j->part_j->type][F_C]) / 2.0; // полусумма скоростей звука частиц
		//printf("mu__%lf__\n", mu_ij);
		viskosity = n_j->h_ij * n_j->mu_ij * (n_j->h_ij * n_j->mu_ij * CONST_B - CONST_A * c_ij) / press_ij;
	}
	return (viskosity);
}


void	ft_refresh_max_stress(t_neigh *n_j)
{
	REAL tmp;

	tmp = ft_vekt_norm2(&(n_j->r_ij), &(n_j->u_ij)) / ft_vekt_norm2(&(n_j->r_ij), &(n_j->r_ij)) * n_j->h_ij;
	if (tmp > norm_speed)
	{
		norm_speed = tmp;
	}
}

void	ft_calk_delta_speed_if_needs(void *neigh_j, void *part_i)
{
	REAL tmp;
	t_dpoint a_tmp;
	t_part *p_i;
	t_neigh *n_j;

	p_i = part_i;
	n_j = neigh_j;
	ft_vekt_difference(&(p_i->speed), &(n_j->part_j->speed), &(n_j->u_ij));
	n_j->mu_ij = ft_vekt_norm2(&(n_j->r_ij), &(n_j->u_ij))
	/ (ft_vekt_norm2(&(n_j->r_ij), &(n_j->r_ij)) + CONST_E * CONST_E * n_j->h_ij * n_j->h_ij);

	//вязкость
	tmp = 4 * g_param[p_i->type][F_VIS] * n_j->mu_ij / (p_i->density + n_j->part_j->density);
	if (p_i->press != 0.0 || n_j->part_j->press != 0.0)
	{
		tmp -= p_i->press / (p_i->density * p_i->density)
		+ n_j->part_j->press / (n_j->part_j->density * n_j->part_j->density)
		+ ft_return_fake_viscosity2(p_i, n_j);
	//	tmp -= (p_i->press + n_j->part_j->press) / (p_i->density * n_j->part_j->density)
	//	+ ft_return_fake_viscosity2(p_i, n_j);
	}

	ft_vekt_scalar_multiplication(&(n_j->nabla_w_ij), g_param[n_j->part_j->type][F_MASS] * tmp, &a_tmp);
	ft_vekt_summ(&(p_i->a), &a_tmp, &(p_i->a));

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




void	ft_new_speeds(void *p_i, void *param)
{
	t_part *part;
	REAL tmp;

	part = (t_part *)p_i;
	if (part->type != OBSTCL)
	{
		part->speed.x += (part->a.x + g.x - SIGMA * part->tension.k.x * part->tension.normal.x) * deltat;
		part->speed.y += (part->a.y + g.y - SIGMA * part->tension.k.y * part->tension.normal.y) * deltat;
		part->speed.z += (part->a.z + g.z - SIGMA * part->tension.k.z * part->tension.normal.z) * deltat;

		tmp = ft_vekt_norm(&(part->speed));
		if (tmp > 0.15 * g_param[part->type][F_C])
			ft_vekt_scalar_multiplication(&(part->speed), 0.12 * g_param[part->type][F_C] / tmp, &(part->speed));

	}
	else
		ft_fill_dpoint(&(part->speed), 0.0, 0.0, 0.0);
	ft_fill_dpoint(&(part->a), 0.0, 0.0, 0.0);

}


void	ft_calk_delta_coord_if_needs(void *neigh_j, void *part_i)
{
	REAL tmp;
	t_dpoint d_speed;
	t_part *p_i;
	t_neigh *n_j;

	p_i = part_i;
	n_j = neigh_j;
	if ((p_i->press + n_j->part_j->press) == 0.0)
		return ;
	//tmp = n_j->part_j->mass * 2.0 / (p_i->press + n_j->part_j->press) * n_j->w_ij;
	tmp = g_param[n_j->part_j->type][F_MASS] * 2.0 / (p_i->density + n_j->part_j->density) * n_j->w_ij;
	ft_vekt_difference(&(p_i->speed), &(n_j->part_j->speed), &d_speed);
	ft_vekt_scalar_multiplication(&d_speed, tmp, &d_speed);
	ft_vekt_summ(&(p_i->delta_pos), &d_speed, &(p_i->delta_pos));
}


#define COEFF_SPEED 0.5

void	ft_cheek_new_pos(t_part *part)
{
	if (part->pos.x < DELTA)
	{
		part->pos.x += DELTA - part->pos.x;
		part->speed.x = -part->speed.x * COEFF_SPEED;
	}
	if (part->pos.y < DELTA)
	{
		part->pos.y += DELTA - part->pos.y;
		part->speed.y = -part->speed.y * COEFF_SPEED;
	}
	if (part->pos.z < DELTA)
	{
		part->pos.z += DELTA - part->pos.z;
		part->speed.z = -part->speed.z * COEFF_SPEED;
	}
	if (part->pos.x >= (IMAX - 1) * DELTA)
	{
		part->pos.x += (IMAX - 1) * DELTA - part->pos.x;
		part->speed.x = -part->speed.x * COEFF_SPEED;
	}
	if (part->pos.y >= (JMAX - 1) * DELTA)
	{
		part->pos.y += (JMAX - 1) * DELTA - part->pos.y;
		part->speed.y = -part->speed.y * COEFF_SPEED;
	}
	if (part->pos.z >= (KMAX - 1) * DELTA)
	{
		part->pos.z += (KMAX - 1) * DELTA - part->pos.z;
		part->speed.z = -part->speed.z * COEFF_SPEED;
	}
}


void	ft_change_coordinates2(t_part *part, void *param)
{
	//if (part->type == WATER)
	//{
		part->pos.x += (part->speed.x + CONST_EP * part->delta_pos.x) * deltat;
		part->pos.y += (part->speed.y + CONST_EP * part->delta_pos.y) * deltat;
		part->pos.z += (part->speed.z + CONST_EP * part->delta_pos.z) * deltat;
		ft_cheek_new_pos(part);
	//}

	ft_fill_dpoint(&(part->delta_pos), 0.0, 0.0, 0.0);
}



void	ft_clear_all(void *part, void *param)
{
	//if (((t_part *)part)->del == TRUE)
	//	return (TRUE);
	//if (((t_part *)part)->type == WATER)
	//	printf("%d_", ((t_part *)part)->neigh->elems_used);
	((t_part *)part)->neigh->elems_used = 0;
	//((t_part *)part)->density = ((t_part *)part)->mass;
	//return (FALSE);
	//((t_part *)part)->delta_density = 0.0;
}

int		ft_clear_all2(void *part)
{
	t_part *p;

	p = *((void **)part);
	//if (ft_part_change_cell(p))
	  //ft_replace_part2(p);
	if (p->type == EMPTY)
		return (TRUE);
	p->neigh->elems_used = 0;
	//p->density = p->mass;
	//___//
	//p->c = max_c;
	return (FALSE);
	//((t_part *)part)->delta_density = 0.0;
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



void	ft_new_delta_density(void *p_i, void *param)
{
	//if (((t_part *)p_i)->type != WATER)
	//	return ;
	ft_for_each_elem(((t_part *)p_i)->neigh, &ft_calk_delta_density, p_i);
}

void	ft_new_delta_speeds(void *p_i, void *param)
{
	if (((t_part *)p_i)->type == OBSTCL)
		return ;
	ft_for_each_elem(((t_part *)p_i)->neigh, &ft_calk_delta_speed_if_needs, p_i);
	//ft_new_speeds(p_i, param);
}

void	ft_new_smoothed_color_func(void *param, int j, int i, int k)
{
	t_part *parts;

	parts = ((t_part ****)param)[j][i][k];
	if (!parts)
		return ;
	//ft_use_function_to_each_neighbors(parts, NULL, &ft_recalk_smoothed_color_func_if_needs);
}

void	ft_new_normal_to_surface(void *param, int j, int i, int k)
{
	t_part *parts;

	parts = ((t_part ****)param)[j][i][k];
	if (!parts)
		return ;
	//ft_use_function_to_each_neighbors(parts, NULL, &ft_recalk_normal_if_needs);
}

void	ft_new_unit_normal_to_surface(void *param, int j, int i, int k)
{
	t_part *parts;

	parts = ((t_part ****)param)[j][i][k];
	if (!parts)
		return ;
	//ft_use_function(parts, NULL, &ft_change_unit_normal);
}

void	ft_new_curvature_of_surface(void *param, int j, int i, int k)
{
	t_part *parts;

	parts = ((t_part ****)param)[j][i][k];
	if (!parts)
		return ;
	//ft_use_function_to_each_neighbors(parts, NULL, &ft_recalk_curvature_if_needs);
}


void	ft_new_delta_coordinates(void *p_i, void *param)
{
	if (((t_part *)p_i)->type == OBSTCL)
		return ;
	ft_for_each_elem(((t_part *)p_i)->neigh, &ft_calk_delta_coord_if_needs, p_i);
	ft_new_coordinates(p_i, param);
}







void	ft_new_coordinates(void *p_i, void *param)
{

	t_part *part;

	//if (((t_part *)p_i)->type != WATER)
	//	return ;
	part = p_i;
	part->pos.x += (part->speed.x + CONST_EP * part->delta_pos.x) * deltat;
	part->pos.y += (part->speed.y + CONST_EP * part->delta_pos.y) * deltat;
	part->pos.z += (part->speed.z + CONST_EP * part->delta_pos.z) * deltat;
	//ft_cheek_new_pos(part);
	ft_fill_dpoint(&(part->delta_pos), 0.0, 0.0, 0.0);
}





/*
**	высчитываем начальную плотность для каждой частицы
*/
void	ft_first_density(void *n_j, void *p_i)
{
	//if (((t_part *)p_i)->type != WATER)
	//	return ;

	((t_part *)p_i)->density += g_param[((t_neigh *)n_j)->part_j->type][F_MASS] * ((t_neigh *)n_j)->w_ij;
}

void	ft_init_density2(void *p_i, void *param)
{
	//t_part *part;
	//part = (t_part *)p_i;
	int type;

	type = ((t_part *)p_i)->type;
	if (type == EMPTY)
		return ;
	((t_part *)p_i)->density = g_param[type][F_MASS];

	ft_for_each_elem(((t_part *)p_i)->neigh, &ft_first_density, p_i);
}




/*
**	типовой расчет частиц
*/

void	ft_solve_and_move_parts(void)
{
	//надо допилить эту функцию
	g_clock2 = clock();

	//////
	//ft_for_each_ptr2(g_parts, &ft_new_neighbors2,(void *)parts);
	//////
	ft_clear_cells(g_cell);
	ft_del_elems_if(g_parts, &ft_cant_add_part_in_cell, (void *)g_cell);
	ft_for_each_ptr2(g_parts, &ft_new_neighbors3, NULL);
	//printf("таймер %ld\n", clock() - g_clock2);

	deltat = ft_time_control(g_param[WATER][F_C], norm_speed, PART_H);

	//считаем изменение плотности
	ft_for_each_ptr(g_parts, &ft_init_density2, NULL);



	ft_for_each_ptr(g_parts, &ft_new_delta_density, NULL);



	//пересчитываем плотность и давление
	ft_for_each_ptr(g_parts, &ft_new_density_and_press_and_color, NULL);
	//считаем изменение скорости
	//////
	ft_for_each_ptr(g_parts, &ft_new_delta_speeds, NULL);
	/////
	//считаем силу поверхостного натяжения
	/*ft_cycle_cube((void *)parts, &ft_new_smoothed_color_func, &start, &end);
	ft_cycle_cube((void *)parts, &ft_new_normal_to_surface, &start, &end);
	ft_cycle_cube((void *)parts, &ft_new_unit_normal_to_surface, &start, &end);
	ft_cycle_cube((void *)parts, &ft_new_curvature_of_surface, &start, &end);*/
	//считаем скорости
	/////
	ft_for_each_ptr(g_parts, &ft_new_speeds, NULL);
	/////

	//считаем изменение координат
	ft_for_each_ptr(g_parts, &ft_new_delta_coordinates, NULL);

	//ft_for_each_ptr(g_parts, &ft_new_coordinates, NULL);

	//ft_for_each_ptr(g_parts, &ft_clear_all, NULL);
	//ft_del_elems(g_parts, &ft_clear_all2);
	//printf("%lf\n", deltat);

}


//сделать правильную инициализацию!!!














//
