/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_lexer.h                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jocohen <jocohen@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/04/08 15:02:12 by jocohen           #+#    #+#             */
/*   Updated: 2019/04/10 15:40:12 by tcollard         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PARSER_LEXER_H
# define PARSER_LEXER_H

# include "libft.h"
# include "shell.h"
# include "str_cmd_inf.h"

typedef enum	e_recall_prompt
{
	PR_SUCCESS,
	PR_ERROR,
	PR_PIPE,
	PR_AND,
	PR_OR
}				t_recall_prompt;

int				lexer_parser(const char *line, t_alloc *alloc, int fork);

/*
************************************ LEXER *************************************
*/

int				expand_home_shortcut(char **s, t_list *vars);
void			check_expand_home(char **s, t_list *vars, t_str_cmd_inf *str_cmd
		, size_t *pos);
int				check_expand_home_assign(char **s, t_list *vars
		, t_str_cmd_inf *str_cmd, size_t *pos);
char			*get_path_tilde(const char *log_search, int join);

/*
************************************ PARSER ************************************
*/

t_ast			*parser(t_list *lst_tk);

/*
********************************* PARSER TOOLS *********************************
*/

t_recall_prompt	token_analyser(t_list *lst_tk, int prompt_heredoc);
int				link_branch(t_ast **elem, t_ast **branch, t_ast **sort
		, t_list **lst_tk);
t_ast			*create_ast_branch(t_list **lst_tk);

/*
*********************************** AST TOOLS **********************************
*/

void			sort_ast(t_ast *lst, t_ast **sort);
void			link_new_node(t_ast **sort, t_ast *tmp, t_ast *node);
t_ast			*fill_cmd(t_list **lst_tk, t_ast *elem);
t_ast			*fill_ope(t_list **lst_tk, t_ast *elem);
t_ast			*fill_assign(t_list **lst_tk, t_ast *elem);
t_ast			*fill_cmd_sep(t_list **lst_tk, t_ast *elem);
t_ast			*create_elem(t_list **lst_tk);
t_ast			*set_new_elem(void);
int				init_input(t_ast *elem, int len, t_list *lst_tk);

#endif
