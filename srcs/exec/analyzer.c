/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   analyzer.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gguichar <gguichar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/04/08 09:58:00 by gguichar          #+#    #+#             */
/*   Updated: 2019/04/08 16:24:31 by jocohen          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <unistd.h>
#include <stdlib.h>
#include "shell.h"
#include "parser_lexer.h"
#include "redirect_inf.h"
#include "execution.h"
#include "inhibitor.h"
#include "signals.h"
#include "vars.h"

static int	dispatch_logic(t_alloc *alloc, t_ast *elem, t_exec_opt *opt)
{
	int	ret;

	ret = 1;
	if (ft_strequ(elem->input[0], "&&"))
	{
		ret = analyzer(alloc, elem->left, opt);
		if (ret == 0)
			ret = analyzer(alloc, elem->right, opt);
	}
	else if (ft_strequ(elem->input[0], "||"))
	{
		ret = analyzer(alloc, elem->left, opt);
		if (ret != 0)
			ret = analyzer(alloc, elem->right, opt);
	}
	return (ret);
}

static int	dispatch_redirection(t_alloc *alloc, t_ast *elem, t_exec_opt *opt)
{
	t_redirect_inf	redirect_inf;
	int				ret;

	if (!ft_strequ("<<", elem->input[0]) && !inhib_expand_tab(elem, alloc))
		return (1);
	if (!fill_redirect_inf(&redirect_inf, elem->input))
	{
		ft_dprintf(STDERR_FILENO, "42sh: ambiguous redirect\n");
		return (1);
	}
	ret = 0;
	if (!setup_redirection(alloc, &redirect_inf)
			|| !process_redirection(&redirect_inf, opt))
		ret = 1;
	clean_redirect(&redirect_inf);
	if (ret == 0)
		ret = analyzer(alloc, elem->left, opt);
	return (ret);
}

static int	dispatch_command(t_alloc *alloc, t_ast *elem, t_exec_opt *opt)
{
	int	ret;

	if (!ft_strequ(elem->input[0], "exit"))
		alloc->exit_rdy = 0;
	ret = try_builtin_execution(alloc, elem, opt);
	if (ret == -1)
		ret = exec_input(alloc, elem, opt);
	update_var(&alloc->vars, "_"
			, elem->input[ft_strtab_count(elem->input) - 1]);
	return (ret);
}

static int	assign_analyzer(t_alloc *alloc, t_ast *elem, t_exec_opt *opt)
{
	if (elem->type != AST_REDIR && !inhib_expand_tab(elem, alloc))
		return (1);
	if (elem->type == AST_JOB)
		return (job_control(alloc, elem, opt));
	else if (elem->type == AST_LOGIC)
		return (dispatch_logic(alloc, elem, opt));
	else if (elem->type == AST_PIPE)
		return (do_pipe(alloc, elem, opt));
	else if (elem->type == AST_ASSIGN)
		return (process_assigns(alloc, elem, opt));
	else if (elem->type == AST_CMD)
		return (dispatch_command(alloc, elem, opt));
	else if (elem->type == AST_REDIR)
		return (dispatch_redirection(alloc, elem, opt));
	else
		return (1);
}

int			analyzer(t_alloc *alloc, t_ast *elem, t_exec_opt *opt)
{
	sigs_wait_line(alloc);
	if (elem == NULL || opt->sigint || g_sig == SIGINT)
	{
		if (g_sig == SIGINT)
			g_sig = 0;
		if (elem == NULL && opt->red_save != NULL && !opt->from_builtin)
			use_rc_on_shell(opt);
		return (0);
	}
	if (elem->type != AST_CMD_SEP)
		return (assign_analyzer(alloc, elem, opt));
	else
	{
		alloc->ret_val = analyzer(alloc, elem->left, opt);
		return (analyzer(alloc, elem->right, opt));
	}
}
