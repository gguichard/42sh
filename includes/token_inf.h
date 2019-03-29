#ifndef TOKEN_INF_H
# define TOKEN_INF_H

# include "libft.h"

typedef enum		e_token_type
{
	TK_NOTHING,
	TK_ASSIGN,
	TK_CMD,
	TK_PARAM,
	TK_RED_OPE,
	TK_RED_LOPT_FD,
	TK_RED_ROPT_FILE,
	TK_CMD_SEP
}					t_token_type;

typedef struct		s_token_inf
{
	char			*token;
	t_token_type	type;
}					t_token_inf;

void				del_token(void *content, size_t content_size);

t_token_inf			*get_tk(t_list *lst);

#endif
