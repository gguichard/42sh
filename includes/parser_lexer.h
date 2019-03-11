#ifndef PARSER_LEXER_H
# define PARSER_LEXER_H

/*
************************************ LEXER *************************************
*/

void	lexer(char *input, t_env **lst_env, t_alloc *alloc);
void	clean_input(char *str, t_ast *lst, t_env **lst_env, t_alloc **alloc);
int		replace_val_ret(char **str, int i, int x);
int		remove_quote(char **s, int *i, t_env *lst_env, t_alloc **alloc);
int		convert_quote(char **s, t_env **lst_env, t_alloc **alloc);
int		check_opening_quote(char **str, t_alloc *alloc);
int		find_closing(char **str, int *i, t_alloc *alloc);
int		check_cmd_pipe(char **input, t_alloc *alloc);
char	**ft_splitwhitespace_shell(char *s);
void	replace_str(char **str, char *insert, int pos);
void	read_lexer(char **lexer, t_env **lst_env, t_ast *lst, t_alloc **alloc);

/*
************************************ PARSER ************************************
*/

void	parser(char **input, t_ast *lst, t_env **lst_env, t_alloc **alloc);
void	fill_ast(char **s, t_ast **lst, int save, int i);
int		analyzer(t_ast *sort, t_env **lst_env, t_alloc **alloc);
int		dispatch_agreg(t_ast *elem, t_env **lst_env, char **tab_path,
		t_alloc **alloc);
int		dispatch_operator(t_ast *elem, t_env **lst_env, char **tab_path,
		t_alloc **alloc);
int		dispatch_redir(t_ast *elem, t_env **lst_env, char **tab_path,
		t_alloc **alloc);
int		dispatch_logic(t_ast *elem, t_env **lst_env, char **tab_path,
		t_alloc **alloc);
int		dispatch_cmd(t_ast *elem, t_env **lst_env, char **tab_path,
		t_alloc **alloc);

#endif
