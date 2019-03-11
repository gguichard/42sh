/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   prompt.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gguichar <gguichar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/01/05 17:10:37 by gguichar          #+#    #+#             */
/*   Updated: 2019/01/30 12:22:03 by gguichar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>
#include <unistd.h>
#include "shell.h"
#include "vars.h"

static char	*get_custom_prompt_pwd(t_shell *shell)
{
	char	*pwd;
	char	*home;
	size_t	home_len;

	pwd = get_shell_var(shell, "PWD");
	if (pwd == NULL)
		return (NULL);
	home = get_shell_var(shell, "HOME");
	if (home == NULL)
		return (pwd);
	if (home[0] != '\0' && ft_strstr(pwd, home) == pwd)
	{
		home_len = ft_strlen(home);
		ft_memmove(pwd + 1, pwd + home_len, ft_strlen(pwd) - home_len + 1);
		pwd[0] = '~';
	}
	free(home);
	return (pwd);
}

static int	print_def_prompt(t_shell *shell)
{
	int		ret;
	char	*pwd;
	char	*user;
	char	*prompt;

	ret = 0;
	pwd = get_custom_prompt_pwd(shell);
	user = get_shell_var(shell, "USER");
	prompt = (char *)malloc((user == NULL ? 5 : ft_strlen(user))
			+ (pwd == NULL ? 7 : ft_strlen(pwd)) + 27);
	if (prompt != NULL)
	{
		prompt[0] = '\0';
		ft_strcat(prompt, "\033[33;1m");
		ft_strcat(prompt, (user == NULL) ? "guest" : user);
		ft_strcat(prompt, "\033[0m \033[34;1m");
		ft_strcat(prompt, (pwd == NULL) ? "unknown" : pwd);
		ft_strcat(prompt, "\033[0m > ");
		if ((ret = write(STDERR_FILENO, prompt, ft_strlen(prompt))) > 0)
			ret -= 22;
	}
	free(pwd);
	free(user);
	free(prompt);
	return (ret);
}

static int	print_prompt_with_type(t_shell *shell)
{
	char	*str;

	str = "";
	if (shell->term.prompt == PROMPT_ESCAPED)
		str = "> ";
	else if (shell->term.prompt == PROMPT_QUOTE)
		str = "quote> ";
	else if (shell->term.prompt == PROMPT_DQUOTE)
		str = "dquote> ";
	else if (shell->term.prompt == PROMPT_HEREDOC)
		str = "heredoc> ";
	else if (shell->term.prompt == PROMPT_BRACKET)
		str = "bracket> ";
	else if (shell->term.prompt == PROMPT_OPE)
		str = "pipe> ";
	return (write(STDERR_FILENO, str, ft_strlen(str)));
}

void		show_prompt(t_shell *shell)
{
	int	write_len;

	write_len = 0;
	if (shell->term.visual_mode)
		write_len += ft_max(0, write(STDERR_FILENO, "(visual) ", 9));
	if (shell->term.prompt == PROMPT_DEF)
		write_len += ft_max(0, print_def_prompt(shell));
	else
		write_len += ft_max(0, print_prompt_with_type(shell));
	shell->term.offset = write_len;
}
