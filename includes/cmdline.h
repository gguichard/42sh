/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cmdline.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gguichar <gguichar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/03/11 14:02:53 by gguichar          #+#    #+#             */
/*   Updated: 2019/04/08 16:28:39 by gguichar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CMDLINE_H
# define CMDLINE_H

# include <string.h>
# include "history.h"
# include "struct_cmdline.h"

# define INPUT_SIZE_INCR 1024
# define VISUAL_STRING "(visual) "

typedef struct s_alloc			t_alloc;
typedef struct s_str_cmd_inf	t_str_cmd_inf;
typedef struct s_ac_suff_inf	t_ac_suff_inf;
typedef struct s_list			t_list;

t_cmdline	*g_cmdline;

int			init_cmdline(t_alloc *alloc, t_cmdline *cmdline);
char		*read_cmdline(t_alloc *alloc, t_cmdline *cmdline);
char		*get_history_file_path(t_alloc *alloc);
int			load_history_file_entries(t_alloc *alloc, t_history *history);
int			save_history_entries(t_alloc *alloc, t_history *history);
void		del_history_entries(t_history *history);

void		reset_cmdline(t_cmdline *cmdline, const char *prompt
		, size_t offset);

char		*get_replacement(char opt, t_list *vars);

/*
** Alloue et retourne le prompt a afficher, retourne NULL en cas d'erreur.
** Set tot_printed_char au nombre de chars printables.
*/
char		*create_prompt(t_list *vars, size_t *tot_printable_char);

char		*create_prompt_and_read_input(t_cmdline *cmdline, t_prompt type
		, t_rstate *state);

/*
** TERM INIT/MISC.
*/

int			setup_term(t_cmdline *cmdline);
int			reset_term(t_cmdline *cmdline);
int			update_winsize(t_cmdline *cmdline);
void		go_to_cursor_pos(t_cmdline *cmdline, t_cursor cursor);

/*
** INPUT/OUTPUT.
*/

void		print_line_by_line(t_cmdline *cmdline, const char *buffer
		, size_t buff_size, int refresh_needed);

void		update_cmdline_at_offset(t_cmdline *cmdline, char caller
		, int is_deletion);

void		print_prompt_and_cmdline(t_cmdline *cmdline);

void		add_char_to_input(t_cmdline *cmdline, char c);

char		*get_prompt(t_cmdline *cmdline, t_prompt type, size_t *offset);
t_rstate	read_input(t_cmdline *cmdline, const char *prompt, size_t offset);
char		*prompt_heredoc(t_cmdline *cmdline, const char *redir_word);

int			t_putchar(int c);

/*
** ESCAPE SEQUENCES.
*/

void		handle_sequence(t_cmdline *cmdline, const t_seq *seq);
const t_seq	*get_sequence(t_cmdline *cmdline, char c);

/*
** COMMON SEQUENCES.
*/

int			handle_end_of_text(t_cmdline *cmdline);

int			handle_move_left(t_cmdline *cmdline);
int			handle_move_right(t_cmdline *cmdline);
int			handle_prev_word(t_cmdline *cmdline);
int			handle_next_word(t_cmdline *cmdline);

int			handle_home_key(t_cmdline *cmdline);
int			handle_end_key(t_cmdline *cmdline);

int			handle_cursor_up(t_cmdline *cmdline);
int			handle_cursor_down(t_cmdline *cmdline);

int			handle_line_start(t_cmdline *cmdline);
int			handle_line_end(t_cmdline *cmdline);

int			handle_konami_code(t_cmdline *cmdline);

/*
** INSERT MODE.
*/

int			handle_backspace_key(t_cmdline *cmdline);
int			handle_delete_key(t_cmdline *cmdline);
int			handle_cmdline_end(t_cmdline *cmdline);
int			handle_eot_key(t_cmdline *cmdline);
int			handle_clear(t_cmdline *cmdline);
int			handle_bell(t_cmdline *cmdline);
int			handle_history_prev(t_cmdline *cmdline);
int			handle_history_next(t_cmdline *cmdline);
void		ac_append_to_cmdline(t_cmdline *cmdline, t_ac_suff_inf *acs_inf
		, t_str_cmd_inf *scmd, int at_end_of_line);
int			handle_autocomplete(t_cmdline *cmdline);

void		ac_print_list(t_list *lst, t_cmdline *cmdline);

/*
** VISUAL MODE.
*/

int			handle_toggle_visual(t_cmdline *cmdline);
int			handle_cut_key(t_cmdline *cmdline);
int			handle_copy_key(t_cmdline *cmdline);
int			handle_paste_key(t_cmdline *cmdline);
int			handle_paste_before_key(t_cmdline *cmdline);

/*
** UTILS.
*/

int			get_prompt_offset(t_cmdline *cmdline);

int			get_rightmost_column(t_cmdline *cmdline, int offset);
void		go_to_offset(t_cmdline *cmdline, int offset);

void		update_visual_select(t_cmdline *cmdline);
int			vm_copy(t_cmdline *cmdline, int cut_hook);
int			vm_paste(t_cmdline *cmdline, int paste_after_cursor);

/*
** SIGNALS.
*/

void		handle_sigwinch(int sig);

#endif
