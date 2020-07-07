#include "doom_nukem.h"

void		update_render(t_var *info, t_render *render)
{
	render->scalar = scalar(info->player->dx, info->player->dy,
	render->ray->x2 - render->ray->x, render->ray->y2 - render->ray->y);
	render->y2 = render->scalar * info->player->dy;
	render->x2 = render->scalar * info->player->dx;
	render->wall_sqdist = norm2(render->x2, render->y2);
	render->wall_dist = sqrt(render->wall_sqdist);
	render->wall_height = WALL_H * (double)render->wall->height
	/ (double)render->wall_dist;
	render->wall_y0 = WINDOW_H / 2 - render->wall_height / 2;
	render->wall_y1 = WINDOW_H / 2 + render->wall_height / 2;
	if (info->player->posz != render->wall->a.z)
	{
		render->wall_y0 += DECALLAGE * (render->wall->a.z - info->player->posz)
		/ render->wall_dist;
		render->wall_y1 += DECALLAGE * (render->wall->a.z - info->player->posz)
		/ render->wall_dist;
	}
}

void		go_to_sector(t_sector *sec_0, int id, t_render *render)
{
	if (sec_0->sector_id < id)
		go_to_sector(sec_0->next_sector, id, render);
	else
		render->s = sec_0;
}

int			calc_item_wall(t_render *render, t_item *item, t_var *info)
{
	if (!(render->wall_item = (t_wall *)ft_memalloc(sizeof(t_wall))))
		return (0);
	render->wall_item->a.x = 0.3 * info->player->planex + item->x;
	render->wall_item->a.y = 0.3 * info->player->planey + item->y;
	render->wall_item->b.x = item->x - 0.3 * info->player->planex;
	render->wall_item->b.y = item->y - 0.3 * info->player->planey;
	return (1);
}

void		check_intersect(t_var *info, t_render *render, t_item *item)
{
	t_point	p;
	t_point	w;
	int	a;

	if (calc_item_wall(render, item, info) == 1)
	{
		if (intersect(render->ray, render->wall_item) == 1
			&& item->cap == 0)
		{
			render->itab[render->k].pv = item->pv;
			a = 0;
			p.x = info->player->posx;
			p.y = info->player->posy;
			w.x = item->x;
			w.y = item->y;
			render->wall_dist = calc_dist(p, w);
			if (is_in_sector(w, render->s) != is_in_sector(p, render->s))
			{
				render->n = -1;
				while (++render->n < render->s->nbr_walls)
				{
					render->wall = &render->s->walls[render->n];
					if (intersect(render->ray, render->wall) == 1)
						if (!render->wall->is_portal)
							a = 1;
				}
			}
				if (a == 0)
			{
				render->nb_item_to_draw++;
				render->itab[render->k].dist = render->wall_dist;
				render->itab[render->k].name = ft_strdup(item->name);
				render->itab[render->k].item_x = item->x;
				render->itab[render->k].item_y = item->y;
				render->itab[render->k].start = render->x;
				render->itab[render->k].h = item->h;
				render->itab[render->k].w = item->w;
				render->itab[render->k].text_id = item->text_id;
				render->itab[render->k].id = item->id;
				item->cap = 1;
				render->x++;
				while (intersect(render->ray, render->wall_item) == 1)
				{
					++render->x;
					update_ray(info, render);
				}
				render->itab[render->k].end = render->x;
				render->x = render->itab[render->k].start;
				update_ray(info, render);
				++render->k;
			}
		}
	}
	if (item->next_item)
		check_intersect(info, render, item->next_item);
}

void		draw_column(t_var *info, t_render *render, int *tab)
{
	int		id_sec;

	if (render->nbr_items > 0)
		check_intersect(info, render, render->item_0);
	render->n = -1;
	while (++render->n < render->s->nbr_walls)
	{
		render->wall = &render->s->walls[render->n];
		if (intersect(render->ray, render->wall) == 1)
		{
			if (render->wall->is_portal == 1
			&& tab[render->wall->sector_id_it_leads_to] == 0)
			{
				tab[render->wall->sector_id_it_leads_to] = 1;
				tab[render->s->sector_id] = 1;
				id_sec = render->wall->sector_id_it_leads_to;
				go_to_sector(render->sec_0, id_sec, render);
				draw_column(info, render, tab);
			}
			else if (render->wall->is_portal == 1
			&& tab[render->wall->sector_id_it_leads_to] == 1)
				break;
			else
			{
				update_render(info, render);
				draw_textures(info, render);
				break;
			}
		}
	}
}

void	draw_item_2(t_render *render, t_var *info, int k, t_item *item)
{
	t_point	p;
	t_point	w;
	float	ty;
	int		y;
	int		i;
	Uint32	color;
	t_item	*weapon;

	p.x = info->player->posx;
	p.y = info->player->posy;
	w.x = render->itab[k].item_x;
	w.y = render->itab[k].item_y;
	render->distance = calc_dist(p, w);
	if (render->distance < 0.5 && (render->itab[k].text_id == 0
	|| render->itab[k].text_id == 4 || render->itab[k].text_id == 5))
	{
		if (render->itab[k].text_id == 4)
		{
			item->cap = 3;
			info->player->hp += 25;
		}
		if (info->player->hp > 100)
			info->player->hp = 100;
		if (render->itab[k].text_id == 0)
			item->cap = 2;
		if (render->itab[k].text_id == 5)
		{
			if (may_weapon(render->item_0))
			{
				weapon = go_to_item(render->item_0, 'a');
				while (weapon && weapon->hold != 1)
					weapon = go_to_item(weapon->next_item, 'a');
				if (!weapon)
					weapon = go_to_item(render->item_0, 'a');
				weapon->mun += 25;
				item->cap = 3;//ici rajouter les munitions
			}
			else
				item->cap = 0;
		}
	}
	else
		item->cap = 0;
	if (render->distance <= 0)
		render->distance = 0.1;
	if (is_in_sector(w, render->s) != is_in_sector(p, render->s) && item->name[0] == 'c')
		return;
	if (is_in_sector(w, render->s) != is_in_sector(p, render->s) && item->name[0] == 's')
		render->itab[k].text_id = 6;
	render->height_item = render->itab[k].h / render->distance;
	render->widht_item = render->itab[k].w / render->distance;
	render->step_height = render->tab_sdl_item[render->itab[k].text_id]->h / render->height_item;
	render->step_width = render->tab_sdl_item[render->itab[k].text_id]->w / render->widht_item;
	render->tx = 0;
	render->itab[k].end /= render->distance / 2;
	if (render->itab[k].start == 0 && render->widht_item > render->itab[k].end)
	{
		render->tx = (render->widht_item - render->itab[k].end) * render->step_width;
		render->widht_item -= (render->widht_item - render->itab[k].end);
	}
	render->x = render->itab[k].start - 1;
	render->p_0 = render->x + 2;
	while (++render->x <= render->widht_item + render->p_0)
	{
		ty = 0;
		y = WINDOW_H / 2 + WINDOW_H / 2 / render->distance;
		i = -1;
		while (++i <= render->height_item)
		{
			color = get_pixel(render->tab_sdl_item[render->itab[k].text_id], render->tab_sdl_item[render->itab[k].text_id]->h - (int)ty, (int)render->tx);
			ty += render->step_height;
			render->wall_dist = render->distance;
			if (color != 0)
			{
				if (item->name[0] == 's')
				{
					if (color != -6815608)
						put_pixel(darken_wall(info, color, render, y),
						render->x, y, info->image);
				}
				else
					put_pixel(darken_wall(info, color, render, y), render->x, y, info->image);
			}
			--y;
		}
		render->tx += render->step_width;
	}
}

int		little_check(t_render *render, t_var *info, int k)
{
	if (render->itab[k].name == 0)
		if (k >= render->nb_item_to_draw && little_check(render, info, k + 1) == 0)
			return (0);
	return (1);
}

void	put_item(int k, t_item *src, t_render *render, t_var *info)
{
	if (render->itab[k].name[0] == src->name[0] && src->cap == 1)
	{
		if ((src->name[0] == 'c' && src->pv > 0)
			|| (src->name[0] == 's' && src->pv > 0))
			draw_item_2(render, info, k, src);
		else if (src->name[0] == 'c' || src->name[0] == 's')
			src->cap = 3;
		else
			draw_item_2(render, info, k, src);
	}
	else if (src->next_item)
		put_item(k, src->next_item, render, info);
}

void	draw_item(t_render *render, t_var *info)
{
	int		j;
	int		k;
	int		i;

	if (!render->itab)
		return;
	if (render->nb_item_to_draw <= 0)
		return;
	i = -1;
	while (++i < render->nb_item_to_draw + 1)
		if (render->itab[i].name == 0)
		{
			render->itab[i].dist = -1;
			render->itab[i].name = "-1";
		}
	j = -1;
	while (++j < render->nb_item_to_draw)
	{
		k = 0;
		while (render->itab[k].name && render->itab[k].name[0] == '-'
				&& render->itab[k].name[1] == '1')
			++k;
		if (!render->itab[k].name)
			break;
		i = -1;
		while (render->itab[++i].name && i < render->nb_item_to_draw + 1)
		{
			if (render->itab[k].dist < render->itab[i].dist
				&& render->itab[i].name[0] != '-' && render->itab[i].name[1] != '1')
				k = i;
		}
		if (render->itab[k].name[0] != '-' && render->itab[k].name[1] != '1')
		{
			put_item(k, render->item_0, render, info);
			render->itab[k].name = "-1";
			render->itab[k].dist = -1;
		}
	}
	i = -1;
	while (++i < render->nb_item_to_draw + 1)
		render->itab[i].name = 0;
}

static	void	ft_put_weapon(t_var *info, t_render *render)
{
	double		x;
	double		y;
	Uint32		color;

	ft_putendl("put?");

	x = 0;
	while (x < render->tab_sdl[3]->w)
	{
		y = 0;
		while (y < render->tab_sdl[3]->h)
		{
			color = get_pixel(render->tab_sdl[3], y, x);
			if (color != 0)
				put_pixel(color, (int)x + WINDOW_W / 2 - 45,
				WINDOW_H + (int)y - 125 + info->d_gun, info->image);
			y++;
		}
		x++;
	}
}

int			may_weapon(t_item *item)
{
	if (item && item->cap == 2 && item->name[0] == 'a')
		return (1);
	else if (item && item->next_item)
		return(may_weapon(item->next_item));
	ft_putendl("out?");
	return (0);
}

int			raycasting(t_var *info, t_render *render)
{
	t_ray	ray;
	int		*tab;
	int		i;
	ft_putendl("ALLO?");

	info->player->sector_id = player_sec(render->sec_0, info);
	if (info->player->sector_id)
		go_to_sector(render->sec_0, info->player->sector_id, render);
	ft_putendl("1?");
	init_cast(info, render, &ray);
	ft_putendl("2?");
	tex_floor_ciel(info, render);
	if (render->nbr_items > 100)
		return (0);
	if (render->nbr_items > 0)
	{
	ft_putendl("3?");
		if (!(render->itab = (t_itab *)ft_memalloc(sizeof(t_itab) * (render->nbr_items + 1))))
			return (0);
		i = -1;
		while (++i < render->nbr_items + 1)
		{
			render->itab[i].name = 0;
			render->itab[i].dist = 0;
		}
	ft_putendl("4?");
	}
	if (!(tab = (int *)ft_memalloc((sizeof(int) * (render->nb_sec + 1)))))
		return (0);
	ft_putendl("5?");
	while (render->x < WINDOW_W)
	{
		go_to_sector(render->sec_0, info->player->sector_id, render);
		init_tab(tab, render->nb_sec);
		update_ray(info, render);
		draw_column(info, render, tab);
		render->x++;
	}
	ft_putendl("6?");
	draw_item(render, info);
	ft_putendl("6.5?");
	if (may_weapon(render->item_0) == 1)
		ft_put_weapon(info, render);
	ft_putendl("6.6?");
	hud(info, info->player, info->map);
	ft_putendl("6.7?");
	rain_gen(info, render);
	ft_putendl("7?");
	free(tab);
	if (render->itab)
		free(render->itab);
	return (1);
}
