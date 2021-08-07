/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   map_math.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ssoraka <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/10/17 10:38:09 by ssoraka           #+#    #+#             */
/*   Updated: 2019/10/17 10:38:10 by ssoraka          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/ft_mod1.h"

char	*skip_space(char *str)
{
	while (*str && ft_isspace(*str))
		str++;
	return (str);
}

t_bool	skip_char(char **string, char chr)
{
	char	*str;

	str = *string;
	str = skip_space(str);
	if (*str == chr)
		str++;
	else
		return (FALSE);
	str = skip_space(str);
	*string = str;
	return (TRUE);
}

t_bool	get_int_num(char **string, int *num)
{
	char	*str;
	int		n;

	str = *string;
	str = skip_space(str);
	*num = ft_atoi(str);
	if (*str == '-')
		str++;
	if (!ft_isdigit(*str))
		return (FALSE);
	while (*str && *str == '0')
		str++;
	n = 0;
	while (*str && ft_isdigit(*str))
	{
		str++;
		n++;
	}
	str = skip_space(str);
	if (n > 8 || (*str != ',' && *str != '(' && *str != ')'))
		return (FALSE);
	*string = str;
	return (TRUE);
}

t_bool	ft_get_nums_from_string(char **string, t_point *p)
{
	char	*str;

	str = *string;
	if (!skip_char(&str, '(')
		|| !get_int_num(&str, &p->x)
		|| !skip_char(&str, ',')
		|| !get_int_num(&str, &p->z)
		|| !skip_char(&str, ',')
		|| !get_int_num(&str, &p->y)
		|| !skip_char(&str, ')'))
		return (FALSE);
	*string = str;
	return (TRUE);
}
