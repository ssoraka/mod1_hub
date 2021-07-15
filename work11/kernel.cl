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

typedef struct		s_cell
{
	int				obstacle;
	int				index[27];
}					t_cell;

__kernel void test(__global t_cell *message)
{
	// получаем текущий id.
	int gid = get_global_id(0);

	//if (gid != 0)
	//	message[gid].obstacle += gid;
	printf("%d_%d\n", get_local_id(0), message[gid].obstacle);

}
