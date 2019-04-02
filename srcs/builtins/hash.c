#include <stdlib.h>
#include <unistd.h>
#include "options.h"
#include "shell.h"
#include "check_path.h"
#include "exectable.h"
#include "builtins.h"

/*
** Affiche le contenu du bucket passe en parametre. Retourne 0 en cas d'erreur
** et 1 en cas de succes.
*/

static int	print_bucket_hash_inf(t_list *hashentry_lst)
{
	t_execentry		*execentry;

	while (hashentry_lst != NULL)
	{
		execentry = (t_execentry*)((t_hashentry*)hashentry_lst->content)->value;
		if (execentry->hits < 0 || execentry->hits > 999)
		{
			if (ft_printf("999+    %s\n", execentry->path) == -1)
				return (0);
		}
		else
		{
			if (ft_printf("%4d    %s\n", execentry->hits, execentry->path)
					== -1)
				return (0);
		}
		hashentry_lst = hashentry_lst->next;
	}
	return (1);
}

/*
** Affiche les informations sur la table de hash des executables. Retourne 0
** en cas d'erreur et 1 en cas de succes.
*/

static int	print_exectable(t_hashtable *exectable)
{
	size_t	bucket_idx;
	int		hash_found;

	hash_found = 0;
	bucket_idx = 0;
	while (bucket_idx < exectable->bucket_count)
	{
		if (exectable->buckets[bucket_idx] != NULL)
		{
			if (!hash_found && ft_printf("hits    command\n") == -1)
				return (0);
			hash_found = 1;
			if (!print_bucket_hash_inf(exectable->buckets[bucket_idx]))
				return (0);
		}
		++bucket_idx;
	}
	if (!hash_found && ft_printf("hash: hash table empty\n") == -1)
		return (0);
	return (1);
}

static int	add_execs_to_hashtable(char **exec_name_tab, t_list *vars
		, t_hashtable *exectable)
{
	t_error		error;
	char		*exec_path;
	int			has_error;

	has_error = 0;
	while (*exec_name_tab != NULL)
	{
		if ((exec_path = search_in_path(vars, *exec_name_tab, X_OK
						, &error)) != NULL)
		{
			if (!set_exec_path(exectable, *exec_name_tab, exec_path, 0))
			{
				ft_dprintf(STDERR_FILENO, "42sh: hash: unknown error\n");
				has_error = 1;
			}
			free(exec_path);
		}
		else
		{
			ft_dprintf(STDERR_FILENO, "42sh: hash: %s: not found\n"
					, *exec_name_tab);
			has_error = 1;
		}
		++exec_name_tab;
	}
	return (!has_error);
}

int			builtin_hash(t_ast *elem, t_alloc *alloc)
{
	t_opts	opts;

	parse_opts(&opts, elem->input, "r");
	if (opts.error != 0)
	{
		ft_dprintf(STDERR_FILENO, "42sh: hash: -%c: invalid option\n"
				, opts.error);
		ft_dprintf(STDERR_FILENO, "42sh: hash: usage: hash [-r] [name ...]\n");
		return (2);
	}
	if (opts.value == 0 && elem->input[opts.index] == NULL)
		return (!print_exectable(alloc->exectable));
	if (has_opt(&opts, 'r'))
		delete_hashentries(alloc->exectable);
	return (!add_execs_to_hashtable(elem->input + opts.index, alloc->vars
				, alloc->exectable));
}
