/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cmdline.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gguichar <gguichar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/03/11 14:02:53 by gguichar          #+#    #+#             */
/*   Updated: 2019/03/24 11:37:24 by gguichar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CMDLINE_H
# define CMDLINE_H

# include <string.h>
# include <sys/ioctl.h>
# include "struct_cmdline.h"

# define INPUT_SIZE_INCR 1024

t_cmdline	*g_cmdline;

/*
** TERM INIT/MISC.
*/

int			setup_term(t_cmdline *cmdline);
int			reset_term(t_cmdline *cmdline);
int			update_winsize(t_cmdline *cmdline);
int			set_cursor_pos(t_cursor *cursor);
void		go_to_cursor_pos(t_cursor cursor);
void		clear_after_cursor(t_cursor cursor, struct winsize winsize);

/*
** INPUT/OUTPUT.
*/

void		print_cmdline_str(t_cmdline *cmdline, const char *buffer
		, size_t len);
void		print_next_line_tcaps(void);
int			print_big_cmdline_prompt(t_cmdline *cmdline);

void		update_cmdline_at_offset(t_cmdline *cmdline);
void		print_cmdline(t_cmdline *cmdline);

void		add_char_to_input(t_cmdline *cmdline, char c);

const char	*get_prompt(t_cmdline *cmdline, t_prompt type);
int			read_input(t_cmdline *cmdline, const char *prompt);

int			t_putchar(int c);

/*
** ESCAPE SEQUENCES.
*/

void		handle_sequence(t_cmdline *cmdline, const t_seq *seq);
const t_seq	*get_sequence(t_cmdline *cmdline, char c);

/*
** COMMON moves.
*/
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

int			get_rightmost_column(t_cmdline *cmdline, int offset);
t_cursor	go_to_offset(t_cmdline *cmdline, int offset);
void		recompute_cursor(t_cmdline *cmdline);

void		update_visual_select(t_cmdline *cmdline);
int			vm_copy(t_cmdline *cmdline, int cut_hook);
int			vm_paste(t_cmdline *cmdline, int paste_after_cursor);

/*
** SIGNALS.
*/

void		handle_sigwinch(int sig);

#endif
