/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_interact.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lbonnete <lbonnete@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/04/18 14:47:46 by lbonnete          #+#    #+#             */
/*   Updated: 2020/06/24 15:21:53 by lbonnete         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "doom-nukem_edit.h"

int		get_map_size(void)
{
	char	*line;
	int		size;

	ft_putendl("Map size?");
	size = 0;
	while (size < 10 || size > 101)
	{
		get_next_line(0, &line);
		size = ft_atoi(line);
		if (size < 10 || size > 101)
			ft_putendl("Wrong size, try something else");
		free(line);
	}
	return (size);
}
