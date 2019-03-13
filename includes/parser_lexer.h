#ifndef PARSER_LEXER_H
# define PARSER_LEXER_H

# include "shell.h"
# include "error.h"
/*
************************************ LEXER *************************************
*/

void	lexer(char *input, t_var *lst_env, t_alloc *alloc);
void	clean_input(char *str, t_ast *lst, t_var *lst_env, t_alloc *alloc);
void	replace_str(char **str, char *insert, int pos);
void	read_lexer(char **lexer, t_var *lst_env, t_ast *lst, t_alloc *alloc);
int		replace_val_ret(char **str, int i, int x);
int		remove_quote(char **s, int *i, t_var *lst_env, t_alloc *alloc);
int		convert_quote(char **s, t_var *lst_env, t_alloc *alloc);
int		check_opening_quote(char **str, t_alloc *alloc);
int		find_closing(char **str, int *i, t_alloc *alloc);
int		check_cmd_pipe(char **input, t_alloc *alloc);
char	**ft_splitwhitespace_shell(char *s);
char	**ft_strsplit_shell(char *str, char c);
int		ret_status(void);


/*
********************************* LEXER TOOLS **********************************
*/

int		ft_isquote(char c);
int		ft_isoperator(char c);
int		ft_str_is_empty(char *s);
void	ft_insert(char **source, char *insert,
		int pos1, int pos2);
void	ft_delete_inside(char **source, int start, int len);
void	replace_str(char **str, char *insert, int pos);
void	get_position(char const *s, int *i, int wn, int *iw);
void	in_quote(char const *s, int *i);
void	lettre_in_quote(char const *s, int *i, int *nb_lettre);
void	nb_lettre_operator(char *s, int i, int *nb_lettre);
int		replace_val_ret(char **str, int i, int x);
int		check_pos_operator(char *s, int *i, int wn, int *wd_search);
int		check_operator(char *s, int *i, unsigned int *nb_word, size_t len);
int		type_operator(char const *s, int *i);
int		check_redir(char *s, int *i, unsigned int *nb_word);
int		position_redir(char const *s, int *i, int wn, int *iw);
char	*ft_back_quote(char *sub, t_var *lst_env, t_alloc *alloc);

/*
************************************ PARSER ************************************
*/

void	parser(char **input, t_ast *lst, t_var *lst_env, t_alloc *alloc);
void	fill_ast(char **s, t_ast *lst, int save, int i);
int		analyzer(t_ast *sort, t_var *lst_env, t_alloc *alloc);
int		dispatch_agreg(t_ast *elem, t_var *lst_env, char **tab_path,
		t_alloc *alloc);
int		dispatch_operator(t_ast *elem, t_var *lst_env, char **tab_path,
		t_alloc *alloc);
int		dispatch_redir(t_ast *elem, t_var *lst_env, char **tab_path,
		t_alloc *alloc);
int		dispatch_logic(t_ast *elem, t_var *lst_env, char **tab_path,
		t_alloc *alloc);
int		dispatch_cmd(t_ast *elem, t_var *lst_env, char **tab_path,
		t_alloc *alloc);

/*
********************************* PARSER TOOLS *********************************
*/

t_ast	*create_new_elem(t_ast *lst);
t_ast	*get_last_elem(t_ast *lst);
t_ast	*add_new_elem(t_ast *lst);
void	check_quote(char *s);
void	replace_quote(char *s, int *i);
void	ft_custom_memmove(char *dst, char *src, size_t len);

/*
*********************************** AST TOOLS **********************************
*/


void	get_last_index_split(int *i, char *s, int *wd);
int		get_last_index(int *i, char *s, int *save, char **input);
int		get_last_operator(int *i, char *s, int *save, char **input);
int		get_last_digit(int *i, char *s, int *save, char **input);
void	fill_input(char **s, int end, int start, t_ast *elem);
void	fill_last_elem(char **s, int i, int save, t_ast *new);
void	link_new_node(t_ast *sort, t_ast *tmp, t_ast *node);
void	add_input_prev_cmd(char **s, int end, int start, t_ast *elem);

/*
********************************* SPLIT TOOLS **********************************
*/
void	in_quote(char const *s, int *i);
void	lettre_in_quote(char const *s, int *i, int *nb_lettre);
void	nb_lettre_operator(char *s, int i, int *nb_lettre);
int		check_redir(char *s, int *i, unsigned int *nb_word);
int		position_redir(char const *s, int *i, int wn, int *iw);



#endif
