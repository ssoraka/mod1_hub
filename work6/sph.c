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



REAL kernel_function_xyz(REAL ri, REAL rj, REAL h)
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
		answer = k * 0.25 * power((2.0 - r_h), 3);
	return (answer);
}

REAL ft_distanse(t_parts pi, t_parts pj)
{



}

REAL kernel_function(REAL r, REAL h)
{
	REAL r_h;
	REAL k;
	REAL answer;

	if (r < 0.0)
		r = -r;
	r_h = r / h;
	if (r_h >= 2.0)
		return (0);
	k = 1.0 / (3.14 * h);
	if (r_h < 1.0)
		answer = k * (1.0 - 1.5 * r_h * r_h + 0.75 * r_h * r_h * r_h);
	else
		answer = k * 0.25 * power((2.0 - r_h), 3);
	return (answer);
}

//высчитываем плотность для каждой частицы
while (i <= parts_count)
{
	parts[i].p = 0.0;
	j = 0;
	while (j <= parts_count)
	{
		parts[i].p += parts[j].m * kernel_function(ft_distanse(parts[i], parts[j]), H);
		j++;
	}
	i++;
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
