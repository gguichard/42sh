/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token_inf.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fwerner <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/01/11 14:15:47 by fwerner           #+#    #+#             */
/*   Updated: 2019/01/23 10:36:36 by fwerner          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef TOKEN_INF_H
# define TOKEN_INF_H

# include "libft.h"

typedef enum		e_token_type
{
	TK_NOTHING,
	TK_WORD,
	TK_OPE,
	TK_NUM_OPT,
	TK_STR_OPT,
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
