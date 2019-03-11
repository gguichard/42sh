/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   shell.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gguichar <gguichar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/01/03 13:33:39 by gguichar          #+#    #+#             */
/*   Updated: 2019/01/30 12:02:40 by gguichar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SHELL_H
# define SHELL_H

# include "libft.h"
# include <term.h>
# include "hashtable.h"

# define ERR_PREFIX "21sh"
# define MAX_ESC_SEQ_BYTES 8
# define CMDLINE_DEF_CAPACITY 1024

typedef struct s_shell	t_shell;

typedef enum			e_prompt
{
	PROMPT_DEF = 0,
	PROMPT_ESCAPED,
	PROMPT_QUOTE,
	PROMPT_DQUOTE,
	PROMPT_HEREDOC,
	PROMPT_BRACKET,
	PROMPT_OPE
}						t_prompt;

typedef struct			s_select
{
	size_t				begin;
	size_t				end;
	char				*clipboard;
}						t_select;

typedef struct			s_term
{
	struct termios		default_term;
	struct termios		curr_term;
	size_t				win_cols;
	int					legacy_mode;
	int					esc_seq;
	char				seq[MAX_ESC_SEQ_BYTES];
	int					seq_off;
	t_prompt			prompt;
	char				*prev_lines;
	char				*line;
	char				*def_line;
	size_t				capacity;
	size_t				size;
	size_t				cursor;
	size_t				col;
	size_t				row;
	size_t				rows;
	size_t				offset;
	int					ac_flag;
	int					visual_mode;
	t_select			select;
}						t_term;

typedef struct			s_history
{
	char				*content;
	struct s_history	*prev;
	struct s_history	*next;
}						t_history;

typedef struct			s_builtin
{
	char				*name;
	int					(*builtin_fun)(t_shell *, int, char **);
}						t_builtin;

struct					s_shell
{
	int					argc;
	char				**argv;
	int					is_atty;
	t_list				*env;
	t_list				*local;
	t_builtin			*builtins;
	t_hashtable			*exec_hashtable;
	int					last_status;
	t_term				term;
	t_history			*history;
	t_history			*history_off;
	t_list				*fork_pids;
};

void					setup_def_vars(t_shell *shell);

t_list					*parse_env(char **environ);
void					setup_signals(void);
void					kill_forked_pids(t_shell *shell, int sig);
void					destroy_shell(t_shell *shell);

void					show_prompt(t_shell *shell);

#endif
