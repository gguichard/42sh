/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   alias.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fwerner <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/04/08 13:29:35 by fwerner           #+#    #+#             */
/*   Updated: 2019/04/11 12:59:02 by fwerner          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>
#include <unistd.h>
#include "libft.h"
#include "shell.h"
#include "options.h"
#include "aliastable.h"
#include "builtins.h"

/*
** Affiche les informations sur l'alias_entry passe en parametre ou un
** message d'erreur s'il vaut NULL. Retourne 0 en cas d'erreur (NULL est
** une erreur) et 1 en cas de succes.
*/

static int	print_alias_hashentry(t_hashentry *alias_entry
		, const char *searched_name)
{
	char	*escaped_str;

	if (alias_entry == NULL)
	{
		ft_dprintf(STDERR_FILENO, "42sh: alias: %s: not found\n"
				, searched_name);
		return (0);
	}
	else
	{
		escaped_str = ft_strreplace(alias_entry->value, "\'", "\'\\\'\'");
		if (ft_printf("alias %s='%s'\n", alias_entry->key, escaped_str) == -1)
		{
			free(escaped_str);
			return (0);
		}
		free(escaped_str);
		return (1);
	}
}

/*
** Affiche les informations sur les alias de l'aliastable. Retourne 0 en cas
** d'erreur et 1 en cas de succes.
*/

static int	print_aliastable(t_hashtable *aliastable)
{
	t_list			*hashentry_lst;
	size_t			bucket_idx;

	bucket_idx = 0;
	while (bucket_idx < aliastable->bucket_count)
	{
		hashentry_lst = aliastable->buckets[bucket_idx];
		while (hashentry_lst != NULL)
		{
			if (!print_alias_hashentry((t_hashentry*)hashentry_lst->content
						, ""))
				return (0);
			hashentry_lst = hashentry_lst->next;
		}
		++bucket_idx;
	}
	return (1);
}

static int	add_or_print_aliases(char **params, t_hashtable *aliastable)
{
	const char	*error_msg;
	char		*equ_pos;
	int			no_error;

	no_error = 1;
	while (*params != NULL)
	{
		equ_pos = ft_strchr(*params, '=');
		if (equ_pos == NULL || equ_pos == *params)
			no_error = (print_alias_hashentry(get_hashentry(aliastable
							, *params), *params) && no_error);
		else
		{
			*equ_pos = '\0';
			no_error = (set_alias_if_valid(aliastable, *params, equ_pos + 1
						, &error_msg) && no_error);
			if (error_msg != NULL)
				ft_dprintf(STDERR_FILENO, "42sh: alias: %s: %s\n", *params
						, error_msg);
		}
		++params;
	}
	return (no_error);
}

int			builtin_alias(t_ast *elem, t_alloc *alloc)
{
	t_opts	opts;

	parse_opts(&opts, elem->input, "");
	if (opts.error != 0)
	{
		ft_dprintf(STDERR_FILENO, "42sh: alias: -%c: invalid option\n"
				, opts.error);
		ft_dprintf(STDERR_FILENO
				, "42sh: alias: usage: alias [name[=value] ...]\n");
		return (2);
	}
	if (elem->input[opts.index] == NULL)
		return (!print_aliastable(alloc->aliastable));
	return (!add_or_print_aliases(elem->input + opts.index
				, alloc->aliastable));
}
