/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   valid_wall2.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lbonnete <lbonnete@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/04/18 14:47:46 by lbonnete          #+#    #+#             */
/*   Updated: 2020/06/26 14:38:04 by lbonnete         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "doom-nukem_edit.h"

int		is_new_point_in_sector(t_point new, t_wall *walls)
{
	int i;

	i = 0;
	while (walls[i].a.x > 0)
	{
		if (new.x == walls[i].a.x && new.y == walls[i].a.y)
			return (0);
		i++;
	}
	return (1);
}

int		center_in_poly(t_point a, t_point b, t_sector *sector)
{
	float center_x;
	float center_y;

	center_x = (float)(a.x + b.x) / 2;
	center_y = (float)(a.y + b.y) / 2;
	if (!is_in_sectors_float(center_x, center_y, sector->map))
	{
		ft_putendl("Can't build a sector inside another (mid)");
		return (0);
	}
	return (1);
}

int		verify_crossing(t_point new, t_point old, t_sector *sector)
{
	t_sector	*current_sector;
	int			i;

	current_sector = sector->map->sectors;
	while (current_sector != sector && current_sector->next_sector != NULL)
	{
		i = 0;
		while (i < current_sector->nbr_walls)
		{
			if (!check_wall_intersections(new, old, current_sector->walls[i]))
			{
				ft_putendl("Wall intersection");
				printf("%i:%i | %i:%i\n", new.x, new.y, old.x, old.y);
				printf("%i:%i | %i:%i\n", current_sector->walls[i].a.x, current_sector->walls[i].a.y, current_sector->walls[i].b.x, current_sector->walls[i].b.y);
				return (0);
			}
			i++;
		}
		current_sector = current_sector->next_sector;
	}
	return (1);
}
