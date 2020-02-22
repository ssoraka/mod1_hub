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



int		ft_get_cell_index(int j, int i, int k)
{
	if (i < I0 || i > IMAX || j < J0 || j > JMAX || k < K0 || k > KMAX)
		return (0);
	return (j * (IMAX + 2) * (KMAX + 2) + i * (KMAX + 2) + k);
}

REAL	ft_kernel_function(REAL h, REAL r)
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
		answer = k * 0.25 * (2.0 - r_h) * (2.0 - r_h) * (2.0 - r_h);
	return (answer);
}

REAL	ft_derivative_kernel_function(REAL h, REAL r)
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
		answer = -k * 0.75 * (2.0 - r_h) * (2.0 - r_h);
	return (answer);
}

t_dpoint	ft_vekt_difference(t_dpoint v1, t_dpoint v2)
{
	t_dpoint answer;

	answer.x = v1.x - v2.x;
	answer.y = v1.y - v2.y;
	answer.z = v1.z - v2.z;
	return (answer);
}

t_dpoint	ft_vekt_scalar_multiplication(t_dpoint v1, REAL param)
{
	t_dpoint answer;

	answer.x = v1.x * param;
	answer.y = v1.y * param;
	answer.z = v1.z * param;
	return (answer);
}

void ft_find_neighbors_in_cell(t_part p_i, t_part p_j)
{
	t_neigh		tmp;
	REAL		len;
	REAL		tmp2;

	tmp.h_ij = PART_H;
	tmp.r_ij = ft_vekt_difference(p_i->pos, p_j->pos);
	len = tmp.r_ij.x * tmp.r_ij.x + tmp.r_ij.y * tmp.r_ij.y + tmp.r_ij.z * tmp.r_ij.z;
	if (len >= 1.0 || len < 0.001)
		return ;
	len = sqrt(len);
	tmp.w_ij = ft_kernel_function(tmp.h_ij, len);
	tmp2 = ft_derivative_kernel_function(tmp.h_ij, len) / len;
	tmp.nabla_w_ij = ft_vekt_scalar_multiplication(tmp.r_ij, tmp2);
	tmp.part_j = p_j;


	return (tmp)
}

void ft_find_neighbors_in_cell(__global t_part *part, t_cell cell)
{
	int i;

	i = 0;
	while (cell.arr[i] != -1)
	{

		i++;
	}
}


__kernel void ft_find_neighbors(__global t_part* part, __global t_cell* cell)
{
	// получаем текущий id.
	int gid;
	int cell_id;
	int j;
	int i;
	int k;

	gid = get_global_id(0);

	j = (int)part[gid].pos.y - 1;
	while (j <= (int)part[gid].pos.y + 1)
	{
		i = (int)part[gid].pos.x - 1;
		while (i <= (int)part[gid].pos.x + 1)
		{
			k = (int)part[gid].pos.z - 1;
			while (k <= (int)part[gid].pos.z + 1)
			{
				cell_id = ft_get_cell_index(j, i, k);
				if (!cell[cell_id].obstacle)
					ft_find_neighbors_in_cell(part[gid], cell[cell_id]);
				k++;
			}
			i++;
		}
		j++;
	}
}
