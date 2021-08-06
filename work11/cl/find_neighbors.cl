/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   kernel.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ssoraka <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/08/25 17:13:40 by ssoraka           #+#    #+#             */
/*   Updated: 2019/08/25 17:13:40 by ssoraka          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "./includes/ft_cl.h"

int			ft_get_cell_index(int j, int i, int k);
t_real		ft_kernel_function(t_real h, t_real r);
t_real		ft_derivative_kernel_function(t_real h, t_real r);
void		ft_find_neighbor_in_cell(__global t_part *p, __global t_neighs *n, int i, int j);

int			ft_get_cell_index(int j, int i, int k)
{
	if (i < I0 || i > IMAX || j < J0 || j > JMAX || k < K0 || k > KMAX)
		return (0);
	return (j * (IMAX + 2) * (KMAX + 2) + i * (KMAX + 2) + k);
}

t_real		ft_kernel_function(t_real h, t_real r)
{
	t_real r_h;
	t_real k;
	t_real answer;

	r_h = r / h;
	k = 1.0 / (PI * h * h * h);
	answer = 0.0;
	if (r_h < 1.0 && r_h > 0)
		answer = k * (1.0 - 1.5 * r_h * r_h + 0.75 * r_h * r_h * r_h);
	else if (r_h <= 2.0 && r_h >= 1.0)
		answer = k * 0.25 * (2.0 - r_h) * (2.0 - r_h) * (2.0 - r_h);
	return (answer);
}

t_real		ft_derivative_kernel_function(t_real h, t_real r)
{
	t_real r_h;
	t_real k;
	t_real answer;

	r_h = r / h;
	k = 1.0 / (PI * h * h * h * h);
	answer = 0.0;
	if (r_h < 1.0 && r_h > 0)
		answer = k * (-3.0 * r_h + 2.25 * r_h * r_h);
	else if (r_h <= 2.0 && r_h >= 1.0)
		answer = -k * 0.75 * (2.0 - r_h) * (2.0 - r_h);
	return (answer);
}

void		ft_find_neighbor_in_cell(__global t_part *p, __global t_neighs *n, int i, int j)
{
	t_neigh		tmp;
	t_real		len;
	t_real		tmp2;

	tmp.h_ij = PART_H;

	tmp.r_ij = (t_dpoint){
		p[i].pos.y - p[j].pos.y,
		p[i].pos.x - p[j].pos.x,
		p[i].pos.z - p[j].pos.z};

	len = sqrt(
		tmp.r_ij.x * tmp.r_ij.x +
		tmp.r_ij.y * tmp.r_ij.y +
		tmp.r_ij.z * tmp.r_ij.z);

	if (len >= 1.0 || len < 0.1)
		return ;
	tmp.w_ij = ft_kernel_function(tmp.h_ij, len);
	tmp2 = ft_derivative_kernel_function(tmp.h_ij, len) / len;

	tmp.nabla_w_ij = (t_dpoint){
		tmp.r_ij.y * tmp2,
		tmp.r_ij.x * tmp2,
		tmp.r_ij.z * tmp2};

	tmp.j = j;

	n[i].j[n[i].count] = tmp;
	n[i].count++;
}

__kernel	void find_neighbors(__global t_part *part, __global t_cell_map *cell, __global t_neighs *neigh)
{
	// получаем текущий id.
	int gid;
	int c_gid;
	int j_gid;
	t_point pos;
	int j;
	int i;
	int k;
	int n;


	gid = get_global_id(0);
	pos = part[gid].jik;

	neigh[gid].count = 0;
	part[gid].not_empty_near_cell = 0;

	if (ft_get_cell_index(pos.y, pos.x, pos.z) == 0)
		return ;

	j = pos.y - 1;
	while (j <= pos.y + 1)
	{
		i = pos.x - 1;
		while (i <= pos.x + 1)
		{
			k = pos.z - 1;
			while (k <= pos.z + 1)
			{
				c_gid = ft_get_cell_index(j, i, k);
				part[gid].not_empty_near_cell += cell[c_gid].full | cell[c_gid].obstacle;
				if (cell[c_gid].full == 1)
				{
					n = 0;
					while ((j_gid = cell[c_gid].index[n]) != -1 && neigh[gid].count < MAX_NEIGH_COUNT)
					{
						ft_find_neighbor_in_cell(part, neigh, gid, j_gid);
						n++;
					}
				}
				k++;
			}
			i++;
		}
		j++;
	}
}
