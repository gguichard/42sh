/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cmdline.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gguichar <gguichar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/03/11 14:02:53 by gguichar          #+#    #+#             */
/*   Updated: 2019/03/14 15:43:49 by gguichar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CMDLINE_H
# define CMDLINE_H

# include <sys/ioctl.h>
# include "struct_cmd_inf.h"

# define INPUT_SIZE_INCR 1024

/*
** TERM INIT/MISC.
*/

int				setup_term(void);
int				reset_term(void);
int				update_winsize(t_cmdline *cmdline);
int				set_cursor_pos(t_cursor *cursor);
void			go_to_cursor_pos(t_cursor cursor);

/*
** INPUT/OUTPUT.
*/

void			add_char_to_input(struct s_input *input, char c);
void			write_char_in_cmdline(t_cmdline *cmdline, char c);
void			update_cmdline_after_offset(t_cmdline *cmdline);

void			print_prompt(t_cmdline *cmdline);
void			read_input(t_cmdline *cmdline);

int				t_putchar(int c);

/*
** ESCAPE SEQUENCES.
*/

void			handle_sequence_char(t_cmdline *cmdline, const char *seq
		, char c);
const char		*get_sequence(t_cmdline *cmdline, char c);

/*
** COMMON moves.
*/
int				handle_move_left(t_cmdline *cmdline);
int				handle_move_right(t_cmdline *cmdline);
int				handle_prev_word(t_cmdline *cmdline);
int				handle_next_word(t_cmdline *cmdline);

int				handle_home_key(t_cmdline *cmdline);
int				handle_end_key(t_cmdline *cmdline);

int				handle_cursor_up(t_cmdline *cmdline);
int				handle_cursor_down(t_cmdline *cmdline);

int				handle_line_start(t_cmdline *cmdline);
int				handle_line_end(t_cmdline *cmdline);

/*
** INSERT MODE.
*/
int				handle_backspace_key(t_cmdline *cmdline);
int				handle_delete_key(t_cmdline *cmdline);
int				handle_test_newline(t_cmdline *cmdline);

/*
** VISUAL MODE.
*/
int				handle_toggle_visual(t_cmdline *cmdline);
int				handle_cut_key(t_cmdline *cmdline);
int				handle_copy_key(t_cmdline *cmdline);
int				handle_paste_key(t_cmdline *cmdline);
int				handle_paste_before_key(t_cmdline *cmdline);

/*
** UTILS.
*/
int				get_rightmost_column(t_cmdline *cmdline, int offset);
void			go_to_offset(t_cmdline *cmdline, int offset);
void			update_visual_select(t_cmdline *cmdline);
int				vm_copy(t_cmdline *cmdline, int cut_hook);
int				vm_paste(t_cmdline *cmdline, int paste_after_cursor);

#endif
