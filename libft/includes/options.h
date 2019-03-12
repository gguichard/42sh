/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   options.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gguichar <gguichar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/12/20 20:32:03 by gguichar          #+#    #+#             */
/*   Updated: 2019/03/12 15:32:46 by fwerner          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef OPTIONS_H
# define OPTIONS_H

# define MAX_OPTS 32

typedef struct	s_opts
{
	int			value;
	int			index;
	char		error;
	const char	*args[MAX_OPTS];
	const char	*optstring;
}				t_opts;

/*
** PARSING.
*/
t_opts			*parse_opts(char **argv, const char *optstring);

/*
** UTILS.
*/
void			check_optstring(const char *optstring);
int				find_opt_offset(const char *optstring, char option);
int				has_opt(t_opts *opts, char option);
const char		*get_optarg(t_opts *opts, char option);

#endif
