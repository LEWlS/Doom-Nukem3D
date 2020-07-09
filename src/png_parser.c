
# include "doom_nukem.h"

int		png_parse()
{
	int	i;
	SDL_Surface *tab[12];
	Uint32	*pixels;
	Uint32 tmp;
	int fd;
	int j;

	if ((tab[0] = IMG_Load("./xpm_textures/stone2.png")) == 0)
		return (0);
	if ((tab[1] = IMG_Load("./xpm_textures/dark_wall.png")) == 0)
		return (0);
	if ((tab[2] = IMG_Load("./xpm_textures/stone3.png")) == 0)
		return (0);
	if ((tab[3] = IMG_Load("./xpm_textures/Jagpistol.png")) == 0)
		return (0);
	if ((tab[4] = IMG_Load("./xpm_textures/ak47.png")) == 0)
		return (0);
	if ((tab[5] = IMG_Load("./xpm_textures/creeper.png")) == 0)
		return (0);
	if ((tab[6] = IMG_Load("./xpm_textures/barrel.png")) == 0)
		return (0);
	if ((tab[7] = IMG_Load("./xpm_textures/soldier.png")) == 0)
		return (0);
	if ((tab[8] = IMG_Load("./xpm_textures/heal.png")) == 0)
		return (0);
	if ((tab[9] = IMG_Load("./xpm_textures/balle.png")) == 0)
		return (0);
	if ((tab[10] = IMG_Load("./xpm_textures/dos_soldier.png")) == 0)
		return (0);
	if ((tab[11] = IMG_Load("./xpm_textures/portal.png")) == 0)
		return (0);

	if ((fd = open("core", O_RDWR)) == -1)
		return (0);
	j =  0;
	while (j < 12)
	{
		pixels = tab[j]->pixels;
		i = 0;
		ft_putnbr_fd(tab[j]->h, fd);
		ft_putstr_fd(";", fd);
		while (i < tab[j]->h * tab[j]->w)
		{
			tmp = pixels[i];
			if (tmp == 0)
				ft_putnbr_fd(tmp, fd);
			while(tmp)
			{
				ft_putnbr_fd(tmp % 10, fd);
				tmp = tmp / 10;
			}
			ft_putstr_fd(";", fd);
			i++;
		}
		ft_putendl_fd("", fd);
		j++;
	}
	j = 0;
	while (++j < 12)
	{
		free(tab[j]);
	}
	return (0);
}

SDL_Surface	*read_text(char *line)
{
	SDL_Surface	*text;
	Uint32		nb;
	int 		i;
	int			p;
	int			debut;

	if (!(text = (SDL_Surface*)ft_memalloc(sizeof(SDL_Surface))))
		return (0);
	i = 0;
	p = 0;
	while (line[i] != ';')
		i++;
	line[i] = '\0';
	text->h = ft_atoi(line);
	text->w = text->h;
	if (!(text->pixels = (Uint32*)ft_memalloc(sizeof(Uint32) * text->w * text->h)))
		return (0);
	i++;
	while(line[i])
	{
		debut = i;
		while (line[i] != ';')
		{
			i++;
		}
		line[i] = '\0';
		((Uint32*)(text->pixels))[p] = ft_atoi(&(line[debut]));
		p++;
		i++;
	}
	return(text);
}

int			read_core_text(t_render *renderer)
{
	int fd;
	int i;
	char *line;

	if ((fd = open("core", O_RDWR)) == -1)
		return (0);
	i = 0;
	while(get_next_line(fd, &line) > 0)
	{
		if (i < 4)
		{
			if (!(renderer->tab_sdl[i] = read_text(line)))
				return (0);
		}
		else
		{
			if (!(renderer->tab_sdl_item[i] = read_text(line)))
				return (0);
		}
		i++;
		free(line);
	}
	return (1);
}