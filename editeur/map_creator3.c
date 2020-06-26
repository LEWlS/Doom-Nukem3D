/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   map_creator3.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lbonnete <lbonnete@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/04/18 14:47:46 by lbonnete          #+#    #+#             */
/*   Updated: 2020/06/26 14:40:59 by lbonnete         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "doom-nukem_edit.h"

void		get_nbr_walls_sector_textures2(t_sector *sector)
{
	char	*line;
	int		size;

	ft_putendl("Texture id of the Ceilling?");
	size = -1;
	while (size < 0 || size > 10)
	{
		get_next_line(0, &line);
		size = ft_atoi(line);
		if (size < 0 || size > 10)
			ft_putendl("Wrong id, try something else");
		free(line);
	}
	sector->celling.text_id = size;
	ft_putendl("lighting in this sector (0-100)?");
	size = -1;
	while (size < 0 || size > 100)
	{
		get_next_line(0, &line);
		size = ft_atoi(line);
		if (size < 0 || size > 100)
			ft_putendl("Wrong value, try something else");
		free(line);
	}
	sector->light = size;
}

void		get_nbr_walls_sector_textures(t_sector *sector)
{
	char	*line;
	int		size;

	ft_putendl("How many walls?");
	size = 0;
	while (size < 3 || size > 25)
	{
		get_next_line(0, &line);
		if ((size = ft_atoi(line)) && (size < 3 || size > 25))
			ft_putendl("Wrong number of walls, try something else");
		free(line);
	}
	sector->nbr_walls = size;
	ft_putendl("Texture id of the Floor?");
	size = -1;
	while (size < 0 || size > 10)
	{
		get_next_line(0, &line);
		size = ft_atoi(line);
		if (size < 0 || size > 10)
			ft_putendl("Wrong id, try something else");
		free(line);
	}
	sector->floor.text_id = size;
	get_nbr_walls_sector_textures2(sector);
}

void		get_height_sector(t_map *map, int *height)
{
	char	*line;

	ft_putendl("How low is you sector?");
	while (height[0] < 1 || height[0] > map->size - 2)
	{
		get_next_line(0, &line);
		height[0] = ft_atoi(line);
		if (height[0] < 1 || height[0] > map->size - 2)
			ft_putendl("Wrong height, try something else");
		free(line);
	}
	ft_putendl("How high is you sector?");
	while (height[1] <= height[0] || height[1] > map->size - 1)
	{
		get_next_line(0, &line);
		height[1] = ft_atoi(line);
		if (height[1] <= height[0] || height[1] > map->size - 1)
			ft_putendl("Wrong height, try something else");
		free(line);
	}
}

int			create_first_wall_edit(t_sector *sector, int *height,
int i, SDL_Event event)
{
	float temp;

	sector->walls[i].sector_id = sector->sector_id;
	sector->walls[i].wall_id = i;
	temp = (float)event.button.x / (float)(WINDOW_H - 50) * sector->map->size;
	temp = round(temp);
	sector->walls[i].a.x = temp;
	sector->walls[i].d.x = temp;
	temp = (float)event.button.y / (float)(WINDOW_H - 50) * sector->map->size;
	temp = round(temp);
	sector->walls[i].a.y = temp;
	sector->walls[i].d.y = temp;
	sector->walls[i].d.z = height[0];
	sector->walls[i].a.z = height[1];
	return (1);
}

void		wall_fusion(t_sector *sector, int i)
{
	sector->walls[i - 1].b.x = sector->walls[i].a.x;
	sector->walls[i - 1].c.x = sector->walls[i].d.x;
	sector->walls[i - 1].b.y = sector->walls[i].a.y;
	sector->walls[i - 1].c.y = sector->walls[i].d.y;
	sector->walls[i - 1].c.z = sector->walls[i].d.z;
	sector->walls[i - 1].b.z = sector->walls[i].a.z;
}
