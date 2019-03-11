/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   shell.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jocohen <jocohen@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/06/24 12:01:28 by jocohen           #+#    #+#             */
/*   Updated: 2019/03/07 11:47:40 by tcollard         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SHELL_H
# define SHELL_H

/*
*********************************** INCLUDES ***********************************
*/

# include <curses.h>
# include <term.h>
# include <unistd.h>
# include <termios.h>
# include <stdio.h>
# include <signal.h>
# include <sys/ioctl.h>
# include <stdlib.h>
# include <limits.h>
# include <sys/stat.h>
# include "../libft/includes/ft_printf.h"
# include "../libft/includes/get_next_line.h"
# include "../libft/includes/libft.h"

/*
************************************ DEFINE ************************************
*/

# define LOGIC		5
# define OPERATOR	4
# define AGREG		2
# define REDIR		2
# define HEREDOC	1
# define CMD		0
# define NO_TYPE 	-1

/*
********************************** STRUCTURES **********************************
*/

typedef struct			s_cursor
{
	size_t				l;
	size_t				c;
}						t_cursor;

typedef struct			s_buf
{
	char				*s;
	size_t				x;
	t_cursor			pos;
	size_t				buf_size;
}						t_buf;

typedef struct			s_historic
{
	char				*origin;
	char				*modif;
	struct s_historic	*next;
	struct s_historic	*prev;
}						t_historic;

typedef struct			s_ast
{
	int					print;
	int					fd[2];
	int					type;
	char				*heredoc;
	char				**input;
	struct s_ast		*next;
	struct s_ast		*back;
	struct s_ast		*left;
	struct s_ast		*right;
}						t_ast;

typedef	struct			s_env
{
	char				*key;
	char				*value;
	struct s_env		*next;
}						t_env;

typedef struct			s_alloc
{
	t_historic			**history;
	t_buf				*input;
	t_ast				**ast;
	t_env				**env;
	int					fd[10];
}						t_alloc;

typedef int				(*t_dispatch)(t_ast*, t_env **lst_env, char **tab_path,
						t_alloc **alloc);
typedef int				(*t_builtins)(t_ast *elem, t_env **lst_env,
						t_alloc **alloc);

/*
*********************************** GLOBALS ***********************************
*/

int						g_in_exec;
int						g_pid;
int						g_ret[2];
int						g_resize;
char					*g_clip;

/*
********************************************************************************
******************************** FUNC PROTOTYPES *******************************
********************************************************************************
*/

/*
************************************* ENV **************************************
*/

int						env_to_lst(char **s, t_env **fp);
char					*find_var_value(t_env *fp, char *var);
int						lst_deletion(t_env **fp);
char					*get_end_pwd(char *pwd);

/*
*********************************** PROMPTER ***********************************
*/

void					prompt(t_alloc *al);
char					*read_input(t_alloc *al);
char					*read_and_sig(t_alloc *al, char *k, int stdin_cpy);
char					*kill_or_give(t_alloc *al, char k);
char					check_prev_read(char k);
void					check_over_buffer(t_buf *input, char *into_buff);
void					input_character(t_buf *input, char k);
void					check_last_char_column(t_buf *input);
char					*recall_prompt(t_alloc *al, int type);
void					analyse_input(t_alloc *al, char k);
void					input_visual_char(t_alloc *al, char k);
char					*enter_section(t_alloc *al, int read);

/*
***************************** EXEC OF SPECIAL KEYS *****************************
*/

void					escape_analysis(t_alloc *al);
int						check_wrong_key(char *key);
int						input_keys_def(t_alloc *al, char *key);
void					del_char(t_buf *input, int type, t_env **lst);
void					page_move(t_buf *input, char k, t_historic **history,
									t_env **lst);
void					arrow_move(t_buf *input, char k, t_historic **history,
									t_env **lst);
void					home_end_move(t_buf *input, char k);
void					ctrl_arrow_move(t_buf *input, char k);
void					selection_cmd(t_buf *input, char k, t_env **lst);
void					selection_input(t_buf *input, char k, t_env **lst);

/*
************************************ TERMIOS ***********************************
*/

void					set_terminal(int reset);

/*
*********************************** HISTORIC ***********************************
*/

void					historic_move(t_buf *input, t_historic **history,
										int direction, t_env **lst);
void					move_front_or_back(t_historic **lst, int direction);
void					cmd_spe_search(t_historic **history, t_buf *input,
										int direction, t_env **lst);
void					install_new_line(t_historic **history, t_buf *input,
										t_env **env);
int						search_cmd_common(const char *s, const char *sch);

int						init_hist(t_historic **history, t_env *lst);
int						historic_opening(t_env *lst, int open_type);
void					historic_entry(char *input, t_historic **history,
										t_env *lst);
t_historic				*ft_new_cmd_hist(void);
int						ft_del_hist(t_historic **fp);
void					reset_hist(t_historic *tmp);
void					end_hist(t_historic **tmp);

/*
************************************ CURSOR ************************************
*/

void					cursor_movement(t_buf *input, int dir);
int						window_width_size(void);
void					check_resize_curs_pos(t_buf *input);
void					vertical_cursor(t_buf *input, int direction);
void					cursor_one_down(t_buf *input);
void					up_into_final_pos(t_buf *input);
void					replace_cursor(t_buf *input, size_t c, size_t l);
void					put_term_rout(char *s);

/*
*********************************** SELECTION **********************************
*/

t_buf					*selec_buffer(int t, t_buf *input, t_env **lst);
t_buf					*alloc_selec_buff(int dir);
void					selection_init(t_buf *input, int dir, t_env **lst);
void					expand_selec(t_buf *selec, t_buf *input, t_env **lst,
										int dir);
void					paste_intra_clip(t_buf *input, t_env **lst);
void					cut_selection(t_buf *input, t_buf *selec, t_env **lst);

/*
*********************************** DISPLAYS ***********************************
*/

void					caller_display(t_env *fp, t_buf *input,
										int change_pos);
int						ret_status(void);
void					fancy_display(char *pwd);
void					classic_display(char *prompt);
void					reactualize_output(t_buf *input, t_env **lst);
int						display_sizing(int size);
void					write_str(char *s, int c);
void					delete_line_pos(t_buf *input, t_env **lst);
void					display_spe_line(t_buf *selec, t_buf *input);
void					redisplay_line_selec(t_buf *selec, t_buf *input,
											t_env **lst);
size_t					check_prev_char(t_buf *input);

/*
*********************************** SIGNALS ************************************
*/

void					sig_kill(int sig);
void					sig_window(int t);
int						quit_signal(char **av);

/*
************************************* MISC *************************************
*/

void					ft_exit(int status);
void					*ft_realloc(void *ptr, size_t size, size_t len);
int						ft_writestdin(int c);

/*
************************************ LEXER *************************************
*/
void					lexer(char *input, t_env **lst_env, t_alloc *alloc);
void					clean_input(char *str, t_ast *lst, t_env **lst_env,
						t_alloc **alloc);
void					check_closing_quote(char *c, char *s, char **input,
											t_alloc *alloc);
int						check_cmd_pipe(char **input, t_alloc *alloc);
int						find_closing(char **str, int *i, t_alloc *alloc);
char					**ft_splitwhitespace_shell(char *s);
char					**ft_strsplit_shell(char *str, char c);
int						convert_quote(char **s, t_env **lst_env,
						t_alloc **alloc);
int						remove_quote(char **s, int *i, t_env *lst_env,
						t_alloc **alloc);
void					read_lexer(char **lexer, t_env **lst_env, t_ast *lst,
						t_alloc **alloc);

/*
************************************ PARSER ************************************
*/
void					parser(char **input, t_ast *lst, t_env **lst_env,
						t_alloc **alloc);
void					fill_ast(char **s, t_ast **lst, int save, int i);
void					replace_quote(char *s, int *i);
int						analyzer(t_ast *sort, t_env **lst_env, t_alloc **alloc);

/*
*********************************** BUILTINS ***********************************
*/
int						echo_builtins(t_ast *elem, t_env **lst_env,
						t_alloc **alloc);
int						cd_builtins(t_ast *elem, t_env **lst_env,
						t_alloc **alloc);
int						setenv_builtins(t_ast *elem, t_env **lst_env,
						t_alloc **alloc);
int						unsetenv_builtins(t_ast *elem, t_env **lst_env,
						t_alloc **alloc);
int						env_cp(char **env, t_env **lst_env);
int						env_builtins(t_ast *elem, t_env **lst_env,
						t_alloc **alloc);
char					*get_env_value(t_env *lst_env, char *str);
void					convert_lst_tab(t_env *lst_env, char ***tab_str);
int						exec_input(t_ast *elem, t_env *lst_env,
						char **tab_path);
int						exec_rights(t_ast *elem, char **tab_path,
						char ***path_all);
int						exit_builtins(t_ast *elem, t_env **lst_env,
						t_alloc **alloc);

/*
********************************** OPERATOR ************************************
*/
int						do_pipe(t_ast *elem, t_env **lst_env, t_alloc **alloc);
int						job_control(t_ast *elem, t_env *lst_env);
void					redirection(t_ast *elem, t_env **lst_env,
						t_alloc **alloc);
void					heredoc(t_ast *elem, t_env **lst_env, t_alloc **alloc);
int						complete_heredoc(t_ast *lst, t_alloc **alloc);

/*
******************************** REDIRECTION ***********************************
*/
void					redirection_1(t_ast *elem, t_env **lst_env,
						t_alloc **alloc);
void					redirection_2(t_ast *elem, t_env **lst_env,
						t_alloc **alloc);
void					redirection_3(t_ast *elem, t_env **lst_env,
						t_alloc **alloc);

/*
******************************** REDIRECTION ***********************************
*/
int						agreg_1(t_ast *elem, t_env **lst_env, char **tab_path,
						t_alloc **alloc);
int						agreg_2(t_ast *elem, t_env **lst_env, char **tab_path,
						t_alloc **alloc);
int						agreg_3(t_ast *elem, t_env **lst_env, char **tab_path,
						t_alloc **alloc);
int						agreg_4(t_ast *elem, t_env **lst_env, char **tab_path,
						t_alloc **alloc);
int						agreg_5(t_ast *elem, t_env **lst_env, char **tab_path,
						t_alloc **alloc);

/*
*********************************** ERROR **************************************
*/
int						exec_error(int err, char *files);
int						ft_error_parse_redir(char **input);
int						ft_error_splitshell(void);
int						ft_error_redir_format(char *ope, int len);
int						check_arg_cd(t_ast *elem, int i);
int						error_cd(char *err, int type);
int						error_setenv(int i);
int						error_unsetenv(int i, char *s);
int						ft_fd_exist(char *str_fd);
void					error_redir(char *file);
void					error_access(char *file);
int						exec_right_error(int err, char *files,
						char ***path_all);
void					ft_exit_malloc(void);
int						check_error_lst(t_ast *lst);

/*
********************************************************************************
*********************************** TOOLS **************************************
********************************************************************************
*/

/*
*********************************** LEXER **************************************
*/
int						ft_isquote(char c);
int						ft_isoperator(char c);
int						ft_str_is_empty(char *s);
void					ft_insert(char **source, char *insert,
						int pos1, int pos2);
void					ft_delete_inside(char **source, int start, int len);
char					*ft_back_quote(char *sub, t_env *lst_env,
						t_alloc **alloc);
int						replace_val_ret(char **str, int i, int x);
void					replace_str(char **str, char *insert, int pos);

/*
********************************** PARSER **************************************
*/
t_ast					*create_new_elem(t_ast **lst);
t_ast					*get_last_elem(t_ast **lst);
t_ast					*add_new_elem(t_ast **lst);
void					check_quote(char *s);
void					replace_quote(char *s, int *i);
void					ft_custom_memmove(char *dst, char *src, size_t len);

/*
********************************* ANALYZER *************************************
*/
int						dispatch_cmd(t_ast *elem, t_env **lst_env,
						char **tab_path, t_alloc **alloc);
int						dispatch_logic(t_ast *elem, t_env **lst_env,
						char **tab_path, t_alloc **alloc);
int						dispatch_redir(t_ast *elem, t_env **lst_env,
						char **tab_path, t_alloc **alloc);
int						dispatch_operator(t_ast *elem, t_env **lst_env,
						char **tab_path, t_alloc **alloc);
int						dispatch_agreg(t_ast *elem, t_env **lst_env,
						char **tab_path, t_alloc **alloc);

/*
*********************************** SPLIT **************************************
*/
void					in_quote(char const *s, int *i);
void					lettre_in_quote(char const *s, int *i, int *nb_lettre);
void					nb_lettre_operator(char *s, int i, int *nb_lettre);
int						check_redir(char *s, int *i, unsigned int *nb_word);
int						check_operator(char *s, int *i, unsigned int *nb_word,
						size_t len);
int						position_redir(char const *s, int *i, int wn, int *iw);
int						type_operator(char const *s, int *i);
int						check_pos_operator(char *s, int *i, int wn,
						int *wd_search);
void					get_position(char const *s, int *i, int wn, int *iw);

/*
***********************************  ENV  **************************************
*/
t_env					*find_elem_env(t_env **lst, char *key);
t_env					*get_last_elem_env(t_env **lst);
void					add_shlvl(t_env **lst);
void					add_elem_env(t_env **lst, char *key, char *value);
void					display_env(t_env *lst);

/*
*********************************** CLEAN **************************************
*/
void					delete_str_tab(char **tab_str);
void					del_lst_env(t_env **lst);
void					del_double_tab(char **tab1, char **tab2);
void					del_lst_ast(t_ast **lst);
void					del_alloc(t_alloc **alloc);

/*
*********************************** OTHER **************************************
*/
char					*get_dir(char *pwd, char **tab_path, int options,
						char *buf_pwd);
char					*cd_predef(char *elem, t_env *lst_env,
						int options, char *buf);
void					add_new_folder(char **dir, char *folder);
char					*delete_last_folder(char *dir);
void					clean_slash_path(char *s);
char					*cd_slash(char *dir, int options, char *buf_pwd);
char					*missing_quote_prompt(char c, t_alloc *alloc);
void					init_ast(char **input, char *s);
void					reinit_fd(int fd[3], t_alloc *alloc);
void					link_new_node(t_ast **sort, t_ast *tmp, t_ast *node);
void					fill_input(char **s, int end, int start, t_ast *elem);
void					add_input_prev_cmd(char **s, int end, int start,
						t_ast *elem);
void					get_last_index_split(int *i, char *s, int *wd);
int						main(int argc, char **argv, char **env);
int						check_opening_quote(char **str, t_alloc *alloc);
int						ft_is_redir(t_ast *elem, int fd[3], t_alloc *alloc,
						t_env **lst_env);
int						ft_is_agreg(t_ast *elem, int fd[3], t_alloc *alloc);
int						heredoc_content(t_alloc *alloc, t_ast *elem, char *s);
int						get_last_index(int *i, char *s, int *save,
						char **input);
int						get_last_operator(int *i, char *s, int *save,
						char **input);
int						get_last_digit(int *i, char *s, int *save,
						char **input);
void					fill_last_elem(char **s, int i, int save, t_ast *new);
#endif
