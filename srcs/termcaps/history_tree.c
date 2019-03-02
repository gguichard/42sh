/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   history_tree.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jocohen <jocohen@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/09/26 14:34:12 by jocohen           #+#    #+#             */
/*   Updated: 2019/02/21 17:17:51 by tcollard         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/shell.h"

int		historic_opening(t_env *lst, int open_type)
{
	int		fd;
	char	*home;
	char	*s;

	if (!(home = get_env_value(lst, "$HOME")))
		return (-1);
	if (home[ft_strlen(home) - 1] != '/')
		s = ft_strjoin(home, "/.21sh_history");
	else
		s = ft_strjoin(home, ".21sh_history");
	if (!s)
		ft_exit_malloc();
	if (!open_type)
		fd = open(s, O_RDONLY);
	else
		fd = open(s, O_WRONLY | O_APPEND | O_CREAT, 0600);
	ft_memdel((void **)&s);
	return (fd);
}

int		init_hist(t_historic **history, t_env *lst)
{
	int			fd;
	t_historic	*tmp;

	tmp = 0;
	fd = historic_opening(lst, 0);
	if (!((*history) = ft_new_cmd_hist()))
		ft_exit(EXIT_FAILURE);
	if (fd == -1)
		return (-1);
	while (get_next_line(fd, &((*history)->origin)) > 0)
	{
		(*history)->prev = tmp;
		tmp = (*history);
		if (!(((*history)->next) = ft_new_cmd_hist()))
			ft_exit(EXIT_FAILURE);
		(*history) = (*history)->next;
	}
	(*history)->prev = tmp;
	close(fd);
	return (0);
}

void	change_quote_op(char *input)
{
	int		x;

	x = 0;
	while (input[x] && !ft_isquote(input[x]))
		x += 1;
	if (input[x])
	{
		while (input[x] && input[x] != '\n')
			x += 1;
		input[x] = 0;
		return ;
	}
	x = 0;
	while (input[x])
	{
		if (input[x] == '\n')
			ft_memmove(input + x, input + x + 1, ft_strlen(input + x + 1));
		else
			x += 1;
	}
}

void	historic_entry(char *input, t_historic **history, t_env *lst)
{
	int		fd;

	if (!input)
		ft_exit_malloc();
	while ((*history)->next)
		(*history) = (*history)->next;
	ft_memdel((void **)&((*history)->origin));
	ft_memdel((void **)&((*history)->modif));
	reset_hist((*history)->prev);
	change_quote_op(input);
	if (!input[0] || ((*history)->prev && !ft_strcmp(input,
		((*history)->prev)->origin)))
		return (ft_memdel((void **)&input));
	fd = historic_opening(lst, 1);
	write(fd, input, ft_strlen(input));
	write(fd, "\n", 1);
	close(fd);
	if (!((*history)->origin = ft_strdup(input))
		|| !(((*history)->next) = ft_new_cmd_hist()))
		ft_exit_malloc();
	((*history)->next)->prev = (*history);
	(*history) = (*history)->next;
	ft_memdel((void **)&input);
}
