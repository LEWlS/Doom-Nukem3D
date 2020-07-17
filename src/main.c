/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lbonnete <lbonnete@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/04/18 14:47:46 by lbonnete          #+#    #+#             */
/*   Updated: 2020/07/17 16:38:20 by lbonnete         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "doom_nukem.h"

int						main_path(t_render *render, t_var *info)
{
	if (read_core_text(render) == 0)
		ft_exit(info, render);
	if (WALL_H > 1000 || WALL_H <= 0)
		ft_exit(info, render);
	return (1);
}

int						main_check(int ac, char **av, t_var *info)
{
	char *str;

	if (ac != 2)
	{
		ft_putendl("./doom-nukem [map_name]");
		return (0);
	}
	str = ft_strdup(av[1]);
	if (info_map(str, info->map) == 0)
	{
		ft_exit(info, info->render);
		return (0);
	}
	if (!(init_win1(info)) || !(init_win2(info)) || !(init_win3(info)))
	{
		ft_exit(info, info->render);
		return (0);
	}
	return (1);
}

void					main_tool(t_var *info, t_render *render)
{
	SDL_RenderPresent(info->renderer);
	SDL_DestroyTexture(info->texture);
	if (!gameplay(info))
		ft_exit(info, render);
}

void					sdl_main(t_render *render, t_var *info)
{
	if (!(info->texture =
	SDL_CreateTextureFromSurface(info->renderer, info->image)))
	{
		ft_putstr("Erreur CreateTextureFromSurface :\n");
		ft_putendl(SDL_GetError());
		ft_exit(info, render);
	}
	if (SDL_RenderCopy(info->renderer, info->texture, NULL, NULL))
	{
		ft_putstr("Erreur RenderCopy :\n");
		ft_putendl(SDL_GetError());
		ft_exit(info, render);
	}
}

int						main(int ac, char **av)
{
	t_var		info;
	t_map		map;
	t_render	render;
	t_player	player;

	info.map = &map;
	system("clear");
	if (PARSE)
		png_parse();
	if (!main_check(ac, av, &info))
		return (0);
	ft_init_pour_linstant(&info);
	init_player(&player, info.map);
	info.player = &player;
	if (!init_render(&info, &render, info.player->sector_id))
		ft_exit(&info, &render);
	info.render = &render;
	main_path(&render, &info);
	while (dealer(&info))
	{
		sdl_main(&render, &info);
		if (raycasting(&info, &render) == 0)
			ft_exit(&info, &render);
		main_tool(&info, &render);
	}
	ft_exit(&info, &render);
	return (0);
}


__attribute__((destructor))
static void destroy_() {
    while (1)
        ;
}
