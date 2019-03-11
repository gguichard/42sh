/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gguichar <gguichar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/01/12 15:09:50 by gguichar          #+#    #+#             */
/*   Updated: 2019/01/25 14:19:35 by gguichar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef LEXER_H
# define LEXER_H

# include "libft.h"
# include "token_inf.h"

void	next_token(t_list **curr);
int		accept_token(t_list **curr, t_token_type type);
int		expect_token(t_list **curr, t_token_type type);
int		is_redirection(t_list *token);
int		is_token(t_list **curr, t_token_type type);

int		lex_commands(t_list *tokens);

#endif
