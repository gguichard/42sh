#ifndef TOKEN_INF_H
# define TOKEN_INF_H

# include "libft.h"

typedef enum		e_token_type
{
	TK_NOTHING,
	TK_WORD,
	TK_OPE,
	TK_LRED_OPT,
	TK_RRED_OPT,
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
