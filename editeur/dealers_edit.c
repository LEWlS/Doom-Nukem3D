/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   dealers_edit.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lbonnete <lbonnete@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/04/18 14:47:46 by lbonnete          #+#    #+#             */
/*   Updated: 2020/07/03 14:32:29 by lbonnete         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "doom_nukem_edit.h"

int		dealers_edit_3(t_map *map, SDL_Event event)
{
	if (event.key.keysym.sym == SDLK_v)
	{
		if (event.key.state == SDL_PRESSED)
		{
			if (validate(map))
				return (0);
		}
	}
	return (1);
}

void	dealers_edit_2(t_input_edit *input_edit, SDL_Event event)
{
	if (event.type == SDL_MOUSEMOTION)
	{
		input_edit->mouse_x = event.button.x;
		input_edit->mouse_x = event.button.x;
	}
	if (event.type == SDL_MOUSEBUTTONDOWN)
	{
		if (event.button.button == SDL_BUTTON_LEFT)
			input_edit->left_click = 1;
		if (event.button.button == SDL_BUTTON_RIGHT)
			input_edit->right_click = 1;
	}
	if (event.type == SDL_MOUSEBUTTONUP)
	{
		if (event.button.button == SDL_BUTTON_LEFT)
			input_edit->left_click = 0;
		if (event.button.button == SDL_BUTTON_RIGHT)
			input_edit->right_click = 0;
	}
}

void	dealers_edit_4(SDL_Event event, t_map *map)
{
	if (event.key.keysym.sym == SDLK_e)
	{
		if (event.key.state == SDL_PRESSED)
		{
			if (map->sectors == NULL)
				ft_putendl("No sectors yet");
			else
				create_enemy(map);
		}
	}
	if (event.key.keysym.sym == SDLK_r)
	{
		if (event.key.state == SDL_PRESSED)
		{
			if (map->items == NULL && map->props == NULL
			&& map->enemys == NULL)
				ft_putendl("No items or props yet");
			else
				del_item_or_prop(map);
		}
	}
}

void	dealers_edit_5(SDL_Event event, t_map *map)
{
	if (event.key.keysym.sym == SDLK_i)
	{
		if (event.key.state == SDL_PRESSED)
		{
			if (map->sectors == NULL)
				ft_putendl("No sectors yet");
			else
				create_item(map);
		}
	}
	if (event.key.keysym.sym == SDLK_p)
	{
		if (event.key.state == SDL_PRESSED)
		{
			if (map->sectors == NULL)
				ft_putendl("No sectors yet");
			else
				create_prop(map);
		}
	}
}

int		dealers_edit(t_var *info, t_map *map, t_input_edit *input_edit)
{
	SDL_Event event;

	if (SDL_PollEvent(&event) == 1)
	{
		if (event.type == SDL_QUIT)
			return (0);
		if (event.key.keysym.sym == SDLK_ESCAPE && event.type == SDL_KEYDOWN)
			return (0);
		dealers_edit_2(input_edit, event);
		if (event.key.keysym.sym == SDLK_d)
		{
			if (event.key.state == SDL_PRESSED)
				del_sector(info, map);
		}
		if (event.key.keysym.sym == SDLK_s)
		{
			if (event.key.state == SDL_PRESSED)
				create_sector(info, map);
		}
		dealers_edit_5(event, map);
		dealers_edit_4(event, map);
	}
	return (dealers_edit_3(map, event));
}
