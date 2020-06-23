/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   enemy.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lbonnete <lbonnete@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/04/18 14:47:46 by lbonnete          #+#    #+#             */
/*   Updated: 2020/06/23 16:48:57 by lbonnete         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "doom-nukem_edit.h"

int		enemy_checks(t_point new, t_map *map)
{
	if (is_in_sectors_spawn(new, map))
	{
		ft_putendl("enemys must be in a sector");
		return (0);
	}
	if (!is_new_point_in_sectors(new, map))
	{
		ft_putendl("enemys can't be inside a wall");
		return (0);
	}
	return (1);
}

t_enemy	*go_to_last_enemy(t_enemy *enemys)
{
	t_enemy *enemy;

	enemy = enemys;
	while (enemy->next_enemy)
	{
		enemy = enemy->next_enemy;
	}
	return (enemy);
}

int		add_enemy(t_map *map, char *name, int x, int y)
{
	t_enemy	*enemy;

	if (map->enemys)
		enemy = go_to_last_enemy(map->enemys);
	else
	{
		if (!(map->enemys = (t_enemy*)malloc(sizeof(t_enemy))))
			return (0);
		map->enemys->next_enemy = NULL;
		map->enemys->x = x;
		map->enemys->y = y;
		map->enemys->name = name;
		return (1);
	}
	if (!(enemy->next_enemy = (t_enemy*)malloc(sizeof(t_enemy))))
		return (0);
	enemy->next_enemy->next_enemy = NULL;
	enemy->next_enemy->x = x;
	enemy->next_enemy->y = y;
	enemy->next_enemy->name = name;
	return (1);
}

int		del_enemy2(t_map *map, int x, int y, t_enemy *enemy)
{
	t_enemy *previous;

	previous = map->enemys;
	while (enemy)
	{
		if (enemy->x == x && enemy->y == y)
		{
			previous->next_enemy = enemy->next_enemy;
			ft_putstr(enemy->name);
			free(enemy->name);
			free(enemy);
			return (1);
		}
		enemy = enemy->next_enemy;
		previous = previous->next_enemy;
	}
	return (0);
}

int		del_enemy(t_map *map, int x, int y)
{
	t_enemy *enemy;

	enemy = map->enemys;
	if (enemy)
	{
		if (enemy->x == x && enemy->y == y)
		{
			map->enemys = map->enemys->next_enemy;
			ft_putstr(enemy->name);
			free(enemy->name);
			free(enemy);
			return (1);
		}
		enemy = enemy->next_enemy;
		if (del_enemy2(map, x, y, enemy) == 1)
			return (1);
	}
	return (0);
}
