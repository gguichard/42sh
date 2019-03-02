/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   caller_display.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jocohen <jocohen@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/05/28 15:53:10 by jocohen           #+#    #+#             */
/*   Updated: 2019/02/21 16:19:00 by jocohen          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/shell.h"

int		display_sizing(int size)
{
	static int	g;

	if (!size)
		return (g);
	g = size;
	return (0);
}

char	*get_end_pwd(char *pwd)
{
	int		x;

	if (!pwd && !pwd[0])
		return (0);
	if (pwd[0] == '/' && !pwd[1])
		return (pwd);
	x = ft_strlen(pwd) - 1;
	(pwd[x] == '/') ? pwd[x] = 0 : 0;
	while (x >= 0 && pwd[x] != '/')
		x -= 1;
	return (pwd + x + 1);
}

void	fancy_display(char *pwd)
{
	write_str(ANSI_RESET, 0);
	write_str(ANSI_BOLD, 0);
	(!ret_status()) ? write_str(ANSI_GREEN, 0) : write_str(ANSI_RED, 0);
	write_str("δ  ", 0);
	if (pwd)
	{
		write_str(ANSI_RESET, 0);
		write_str(ANSI_BOLD, 0);
		write_str(ANSI_WHITE, 0);
		pwd = get_end_pwd(pwd);
		display_sizing(ft_strlen(pwd) + 6);
		write_str(pwd, 0);
	}
	write_str(ANSI_RESET, 0);
	write_str(" > ", 0);
}

void	classic_display(char *prompt)
{
	write_str(prompt, 0);
	write_str("> ", 0);
	display_sizing(ft_strlen(prompt) + 2);
}

void	caller_display(t_env *fp, t_buf *input, int change_pos)
{
	char		*pwd;
	int			mode;
	int			del;

	del = 0;
	mode = 1;
	if ((pwd = recall_prompt(0, -1)))
		mode = 0;
	else if (!(pwd = get_env_value(fp, "$PWD")))
	{
		pwd = getcwd(0, 0);
		del = 1;
	}
	if (!isatty(0))
		return ;
	(mode) ? fancy_display(pwd) : 0;
	(!mode) ? classic_display(pwd) : 0;
	if (change_pos)
	{
		input->pos.l = (display_sizing(0)) / window_width_size();
		input->pos.c = display_sizing(0) % (window_width_size());
	}
	(del == 1) ? ft_memdel((void **)&pwd) : 0;
}
