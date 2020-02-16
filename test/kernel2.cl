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

__kernel void test2(__global int* message)
{
	// получаем текущий id.
	int gid = get_global_id(0);

	printf("%d_%d_%d\n", get_local_id(0), message[gid], message[gid + 1]);

	if (gid != 0)
		message[gid] += gid;
	printf("%d_%d_%d\n", get_local_id(0), message[gid], message[gid + 1]);

}
