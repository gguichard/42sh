/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cmdline.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gguichar <gguichar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/03/11 14:02:53 by gguichar          #+#    #+#             */
/*   Updated: 2019/03/13 11:19:31 by gguichar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CMDLINE_H
# define CMDLINE_H

# include <sys/ioctl.h>

# define INPUT_SIZE_INCR 1024

enum			e_prompt
{
	PROMPT_DEFAULT = 0,
	PROMPT_BACKSLASH,
	PROMPT_QUOTE,
	PROMPT_DQUOTE,
	PROMPT_BRACKET,
	PROMPT_HEREDOC
};

typedef struct	s_cursor
{
	int	x;
	int	y;
}				t_cursor;

struct			s_input
{
	char	*buffer;
	int		capacity;
	int		size;
	int		offset;
};

typedef enum	e_esc_mode
{
	MODE_INSERT,
	MODE_VISUAL,
	MODE_COMMON
}				t_esc_mode;

typedef struct	s_esc_keys
{
	char	buffer[8];
	int		size;
	int		offset;
}				t_esc_keys;

struct			s_prompt
{
	enum e_prompt	type;
	int				offset;
};

typedef struct	s_cmdline
{
	struct s_prompt	prompt;
	struct s_input	input;
	struct winsize	winsize;
	t_cursor		cursor;
	t_esc_keys		esc_keys;
	int				row;
}				t_cmdline;

typedef struct	s_esc_seq
{
	char		*str;
	t_esc_mode	mode;
	int			(*fn)(t_cmdline *cmdline);
}				t_esc_seq;

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
const char		*get_escape_sequence(t_esc_keys *keys, char c);

/*
 ** COMMON moves.
 */
int				handle_move_left(t_cmdline *cmdline);
int				handle_move_right(t_cmdline *cmdline);
int				handle_prev_word(t_cmdline *cmdline);
int				handle_next_word(t_cmdline *cmdline);

int				handle_home_key(t_cmdline *cmdline);
int				handle_end_key(t_cmdline *cmdline);

/*
 * INSERT MODE.
 */
int				handle_backspace_key(t_cmdline *cmdline);
int				handle_delete_key(t_cmdline *cmdline);
int				handle_test_newline(t_cmdline *cmdline);

#endif
