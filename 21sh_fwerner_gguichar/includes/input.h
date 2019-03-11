/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   input.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gguichar <gguichar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/01/03 20:36:02 by gguichar          #+#    #+#             */
/*   Updated: 2019/01/30 10:21:10 by gguichar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef INPUT_H
# define INPUT_H

# include "shell.h"
# include "autocomplete.h"
# include "str_cmd_inf.h"

# define EOT_KEY 4
# define BELL_KEY 7
# define BACKSPACE_KEY 8
# define FORM_FEED_KEY 12
# define CTRL_R_KEY 18
# define DELETE_KEY 127
# define ESC_SEQ_LEFT "\033[D"
# define ESC_SEQ_RIGHT "\033[C"
# define ESC_SEQ_UP "\033[A"
# define ESC_SEQ_DOWN "\033[B"
# define ESC_SEQ_PAGEUP "\033[5~"
# define ESC_SEQ_PAGEDOWN "\033[6~"
# define ESC_DEL_KEY "\033[3~"
# define ESC_HOME_KEY "\033[H"
# define ESC_END_KEY "\033[F"
# define ESC_SHIFT_LEFT "\033[1;2D"
# define ESC_SHIFT_RIGHT "\033[1;2C"
# define ESC_SHIFT_UP "\033[1;2A"
# define ESC_SHIFT_DOWN "\033[1;2B"
# define ESC_FN_F1 "\033OP"

typedef struct	s_seq
{
	char		*str;
	int			(*f)(t_shell *shell, t_term *term);
}				t_seq;

typedef struct	s_ac_format
{
	int			elems;
	int			col_width;
	int			max_col;
	int			max_row;
	t_list		**cols;
}				t_ac_format;

/*
** SETUP TERM.
*/
void			setup_term(t_shell *shell);
void			reset_term(t_shell *shell);
void			update_winsize(t_term *term);

/*
** COMMAND LINE.
*/
char			*get_command_line(t_term *term);
int				is_command_complete(t_str_cmd_inf *scmd_inf, t_list *token_lst);
t_prompt		get_prompt_type(t_str_cmd_inf *scmd_inf);
int				read_input(t_shell *shell);
int				wait_for_command(t_shell *shell);
void			reset_cmdline(t_shell *shell, t_prompt prompt);
int				realloc_cmdline(t_term *term);
void			go_to_prompt(t_term *term);
void			print_input(t_shell *shell, t_term *term);
void			insert_cmdline(t_shell *shell, t_term *term, char key);

/*
** ESCAPE sequences.
*/
const t_seq		*get_valid_esc_sequence(t_term *term);
int				handle_esc_key(t_shell *shell, t_term *term, char key);
void			handle_esc_sequence(t_shell *shell, t_term *term
		, const t_seq *seq);

/*
** INSERT MODE.
*/
void			insert_spestr_cmdline(t_shell *shell, t_term *term
		, const char *spe_str, char replace);
void			add_char_and_escape_if_needed(char char_to_add
		, int already_escaped, t_shell *shell, t_str_cmd_inf *scmd);
char			*init_scmd_with_realline(t_str_cmd_inf *scmd, t_term *term
		, int *at_end_of_line);
void			ac_print_list(t_list *lst, t_term *term);
int				handle_ac(t_shell *shell, t_term *term);
int				handle_eot_key(t_shell *shell, t_term *term);
int				handle_bs_key(t_shell *shell, t_term *term);
int				handle_del_key(t_shell *shell, t_term *term);
int				handle_key(t_shell *shell, t_term *term, char key);

/*
** VISUAL MODE.
*/
void			print_select_line(t_term *term);
int				vm_toggle(t_shell *shell, t_term *term);
int				vm_copy_hook(t_shell *shell, t_term *term, int cut);
int				vm_paste_hook(t_shell *shell, t_term *term, int before_cursor);
int				handle_vm_key(t_shell *shell, t_term *term, char key);

/*
** HOOKS.
*/
int				move_cursor_to_col(t_term *term);
int				move_cursor_left(t_shell *shell, t_term *term);
int				move_cursor_right(t_shell *shell, t_term *term);
int				move_cursor_home(t_shell *shell, t_term *term);
int				move_cursor_end(t_shell *shell, t_term *term);
int				move_cursor_prev_word(t_shell *shell, t_term *term);
int				move_cursor_next_word(t_shell *shell, t_term *term);
int				move_cursor_up(t_shell *shell, t_term *term);
int				move_cursor_down(t_shell *shell, t_term *term);

/*
** MISC HOOKS.
*/
int				handle_screen_clear(t_shell *shell, t_term *term);
int				handle_bell(t_shell *shell, t_term *term);

/*
** UTILS.
*/
size_t			get_max_col(t_term *term);
size_t			get_total_rows(t_term *term);
void			update_cursor_data(t_term *term);
void			update_pos_data(t_term *term);

#endif
