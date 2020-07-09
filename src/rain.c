/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   rain.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: trabut <trabut@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/06/20 17:04:28 by lewis             #+#    #+#             */
/*   Updated: 2020/07/09 14:47:02 by trabut           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "doom_nukem.h"

static	void	draw_col(t_var *info, t_point a, int h)
{
	int i;

	i = 0;
	while (i < h)
	{
		put_pixel(rgb_calc(205, 210, 225), a.x, a.y + i, info->image);
		put_pixel(rgb_calc(205, 210, 225), a.x + 1, a.y + i, info->image);
		i++;
	}
}

int				rain_gen(t_var *info)
{
	int			i;
	int			tmp;
	t_point		p;

	i = 0;
	tmp = rand() % 20;
	while (i++ < tmp && RAIN)
	{
		p.x = rand() % (WINDOW_W - 20) + 10;
		p.y = rand() % (WINDOW_H - 10);
		draw_col(info, p, rand() % 25 + 5);
	}
	return (0);
}
