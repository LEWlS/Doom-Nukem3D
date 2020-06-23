/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   editer.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lbonnete <lbonnete@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/04/18 14:47:46 by lbonnete          #+#    #+#             */
/*   Updated: 2020/06/23 16:40:12 by lbonnete         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "doom-nukem_edit.h"

int		editer_help(t_var *info)
{
	if (!(info->texture = SDL_CreateTextureFromSurface(info->renderer,
	info->image)))
	{
		ft_putstr("Erreur CreateTextureFromSurface :\n");
		ft_putendl(SDL_GetError());
		SDL_DestroyWindow(info->window);
		SDL_Quit();
		return (0);
	}
	if (SDL_RenderCopy(info->renderer, info->texture, NULL, NULL))
	{
		ft_putstr("Erreur RenderCopy :\n");
		ft_putendl(SDL_GetError());
		SDL_DestroyWindow(info->window);
		SDL_Quit();
		return (0);
	}
	return (1);
}

int		editer(t_var *info, t_map *map)
{
	t_input_edit input_edit;

	info->input_edit = &input_edit;
	dealers_init(&input_edit);
	map->size = get_map_size();
	init_map(map);
	init_box(map);
	draw_grid(info, map);
	while (dealers_edit(info, map, &input_edit))
	{
		draw_map_edit(info, map);
		hud(info);
		if (editer_help(info) == 0)
			return (0);
		SDL_RenderPresent(info->renderer);
		SDL_DestroyTexture(info->texture);
	}
	return (1);
}
