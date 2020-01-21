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

# define MAX_NEIGHBORS 32
# define DELTA_H 0.78

int ppp = 0;

void ft_del_parts(void *ptr)
{
	t_part *part;

	part = (t_part *)(*((void **)ptr));
	if (part)
	{
		ft_cut_part(part);
		ft_del_arr(&(part->neigh));
		free(part);
	}
}


t_arr	*ft_init_all_clear_parts(void)
{
	t_arr *parts;
	int parts_count;

	parts_count = (int)(10);
	parts = ft_create_arr(sizeof(t_part *), parts_count, &ft_del_parts);
	if (!parts)
		ft_del_all_print_error("need more memory");
	return (parts);
}

void	ft_fill_part(t_part *part, t_dpoint *p, int type)
{
	if (!part || !p)
		return ;
	ft_fill_point(&(part->jik), (int)p->y + 0, (int)p->x + 0, (int)p->z + 0);
	ft_fill_dpoint(&(part->pos.abs), p->y, p->x, p->z);
	part->type = type;
	ft_fill_param_of_part(part, NULL);
	if (type == WATER)
		part->pos.color = WATER_COLOR;
	else if (type == BLOB)
		part->pos.color = 0xFFFFFF;
	else if (type == MAGMA)
		part->pos.color = 0xFF0000;
	else
		part->pos.color = OBSTACLES_TOP_COLOR;
	if (type == BLOB)
	{
		part->speed.y = -20.0;
		type = WATER;
	}
	if (type == MAGMA)
		type = WATER;
	part->type = type;

}

void	ft_insert_part(t_part *part)
{
	t_part *tmp;

	if (!part)
		return ;
	if (part->jik.x < I0 || part->jik.y < J0 || part->jik.z < K0
	|| part->jik.x > IMAX || part->jik.y > JMAX || part->jik.z > KMAX)
	{
		part->type = EMPTY;
		ft_fill_dpoint(&(part->pos.abs), JMAX + 1, IMAX + 1, KMAX + 1);
		ft_fill_point(&(part->jik), JMAX + 1, IMAX + 1, KMAX + 1);
	}
	tmp = parts[part->jik.y][part->jik.x][part->jik.z];
	part->next = tmp;
	if (tmp)
		tmp->prev = part;
	parts[part->jik.y][part->jik.x][part->jik.z] = part;
	part->prev = NULL;
}

void	ft_cut_part(t_part *part)
{
	t_part **tmp;

	if (!part)
		return ;
	tmp = &(parts[part->jik.y][part->jik.x][part->jik.z]);
	if (!part->prev && !part->next && part != *tmp)
		return ;
	if (!part->prev)
		*tmp = part->next;
	else
		part->prev->next = part->next;

	if (part->next)
		part->next->prev = part->prev;

	part->prev = NULL;
	part->next = NULL;
}

void	ft_create_new_water_in_area(t_arr **p_arr, t_dpoint *start, t_point *end, int type)
{
	REAL i;
	REAL k;
	t_part *part;
	t_dpoint pos;

	i = start->x;
	while (i < end->x + 1 && i < IMAX + 1)
	{
		k = start->z;
		while (k < end->z + 1 && k < KMAX + 1)
		{
			if (map[(int)(start->y)][(int)i][(int)k] == EMPTY)
			//if (cell_type == WATER || cell_type == BLOB)
			{
				ft_fill_dpoint(&pos, start->y, i, k);
				//ft_fill_part(&part, &pos, cell_type);
				part = ft_new_part(&pos, type);
				ft_insert_part(part);
				ft_arr_add(p_arr, (void *)(&part));
			}
			k++;
		}
		i++;
	}
}

void	ft_clear_map_from_water_and_blob(t_arr **parts)
{
	int i;
	int j;
	int k;

	j = J0 - 1;
	while (++j < JMAX + 1)
	{
		i = I0 - 1;
		while (++i < IMAX + 1)
		{
			k = K0 - 1;
			while (++k < KMAX + 1)
			{

			}
		}
	}
}
/*
void	ft_create_blob(int j, int i, int k)
{
	t_point cell;

	if (map[j][i][k] == OBSTACLES)
		return;
	ft_fill_point(&cell, j, i, k);
	if (!parts[j][i][k])
	{
		map[j][i][k] = BLOB;
		ft_create_new_area_of_water(&g_parts, &cell, &cell);
		map[j][i][k] = EMPTY;
	}
}*/


void	ft_create_new_area_of_water(t_arr **parts, t_point *start, t_point *end, int type)
{
	t_dpoint tmp;
	int n;

	n = 0;
	tmp.y = start->y + 0.5;
	while (tmp.y < end->y + 1 && tmp.y < JMAX + 1)
	{
		if (n % 2)
			ft_fill_dpoint(&tmp, tmp.y, start->x + 0.2, start->z + 0.2);
		else
			ft_fill_dpoint(&tmp, tmp.y, start->x + 0.7, start->z + 0.7);
		ft_create_new_water_in_area(parts, &tmp, end, type);
		tmp.y += DELTA_H;
		n++;
	}
}

t_part	*ft_new_part(t_dpoint *p, int type)
{
	t_part *tmp;

	if ((tmp = (t_part *)ft_memalloc(sizeof(t_part))))
	{
		ft_fill_part(tmp, p, type);
		tmp->neigh = ft_create_arr(sizeof(t_neigh), MAX_NEIGHBORS, NULL);
		if (!tmp->neigh)
			ft_memdel((void **)(&tmp));
	}
	if (!tmp)
		ft_del_all_print_error("need more memory2");
	return (tmp);
}

t_part	*ft_add_part(void *ptr, t_dpoint *p, int type)
{
	t_part *tmp;
	t_part **begin;

	begin = (t_part **)ptr;
	tmp = ft_new_part(p, type);
	if (!tmp)
		return (NULL);
	tmp->next = *begin;
	*begin = tmp;
	return (tmp);
}


void	ft_del_part(t_part **begin)
{
	t_part *tmp;

	tmp = *begin;
	while (tmp)
	{
		*begin = tmp->next;
		free(tmp);
		tmp = *begin;
	}
	*begin = NULL;
}

void	ft_replace_part(t_part *prev, t_part **part, t_part ****parts)
{
	int i;
	int j;
	int k;

	prev->next = (*part)->next;
	i = (*part)->jik.x;
	j = (*part)->jik.y;
	k = (*part)->jik.z;
	(*part)->next = parts[j][i][k];
	parts[j][i][k] = (*part);
	*part = prev;
}

void	ft_replace_part2(t_part *part)
{
	ft_insert_part(part);
}


#define COEFF_SPEED 0.5

void	ft_come_back_part(t_part *part, int *j, int *i, int *k)
{
	if (*i < I0)
	{
		part->pos.abs.x = I0;
		part->speed.x = -part->speed.x * COEFF_SPEED;
		part->jik.x = I0;
		*i = part->jik.x;
	}
	else if (*i > IMAX)
	{
		part->pos.abs.x = IMAX + 0.999;
		part->speed.x = -part->speed.x * COEFF_SPEED;
		part->jik.x = IMAX;
		*i = part->jik.x;
	}
	if (*j < J0)
	{
		part->pos.abs.y = J0;
		part->speed.y = -part->speed.y * COEFF_SPEED;
		part->jik.y = J0;
		*j = part->jik.y;
	}
	else if (*j > JMAX)
	{
		part->pos.abs.y = JMAX + 0.999;
		part->speed.y = -part->speed.y * COEFF_SPEED;
		part->jik.y = JMAX;
		*j = part->jik.y;
	}
	if (*k < K0)
	{
		part->pos.abs.z = K0;
		part->speed.z = -part->speed.z * COEFF_SPEED;
		part->jik.z = K0;
		*k = part->jik.z;
	}
	else if (*k > KMAX)
	{
		part->pos.abs.z = KMAX + 0.999;
		part->speed.z = -part->speed.z * COEFF_SPEED;
		part->jik.z = KMAX;
		*k = part->jik.z;
	}
}



void	ft_come_back_part2(t_part *part, int *j, int *i, int *k)
{
	if (map[part->jik.y][*i][part->jik.z] == OBSTACLES)
	{
		if (part->jik.x < *i)
			part->pos.abs.x = *i - 0.001;
		else
			part->pos.abs.x = part->jik.x;
		part->speed.x = -part->speed.x * COEFF_SPEED;
		*i = part->jik.x;
	}
	if (map[*j][part->jik.x][part->jik.z] == OBSTACLES)
	{
		if (part->jik.y < *j)
			part->pos.abs.y = *j - 0.001;
		else
			part->pos.abs.y = part->jik.y;
		part->speed.y = -part->speed.y * COEFF_SPEED;
		*j = part->jik.y;
	}
	if (map[part->jik.y][part->jik.x][*k] == OBSTACLES)
	{
		if (part->jik.z < *k)
			part->pos.abs.z = *k - 0.001;
		else
			part->pos.abs.z = part->jik.z;
		part->speed.z = -part->speed.z * COEFF_SPEED;
		*k = part->jik.z;
	}
}

int		ft_part_change_cell(t_part *part)
{
	int i;
	int j;
	int k;

	i = (int)(part->pos.abs.x / delta.x) + 0;
	j = (int)(part->pos.abs.y / delta.y) + 0;
	k = (int)(part->pos.abs.z / delta.z) + 0;
	if (i != part->jik.x || j != part->jik.y || k != part->jik.z)
	{
		if (i >= I0 && j >= J0 && k >= K0 && i <= IMAX && j <= JMAX && k <= KMAX && map[j][i][k] == OBSTACLES)
			ft_come_back_part2(part, &j, &i, &k);
		ft_cut_part(part);
		ft_fill_point(&(part->jik), j, i, k);
		return (TRUE);
	}
	return (FALSE);
}


int		ft_part_change_cell2(t_part *part)
{
	int i;
	int j;
	int k;

	i = (int)(part->pos.abs.x / delta.x) + 0;
	j = (int)(part->pos.abs.y / delta.y) + 0;
	k = (int)(part->pos.abs.z / delta.z) + 0;
	if (i != part->jik.x || j != part->jik.y || k != part->jik.z)
	{
		ft_cut_part(part);
		ft_fill_point(&(part->jik), j, i, k);
		return (TRUE);
	}
	return (FALSE);
}
