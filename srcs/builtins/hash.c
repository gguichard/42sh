#include "shell.h"
#include "options.h"
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

//TODO gerer "hash utility"
int			hash_builtins(t_ast *elem, t_var **lst_env, t_alloc *alloc)
{
	int		par_idx;
	t_opts	*opts;

	(void)lst_env;
	if ((opts = parse_opts(elem->input, "r")) == NULL)
		return (1);
	if (opts->error != 0)
	{
		ft_dprintf(STDERR_FILENO, "42sh: hash: -%c: invalid option\n"
				, opts->error);
		ft_dprintf(STDERR_FILENO, "42sh: hash: usage: hash [-r] [name ...]\n");
		return (1);
	}
	if (opts->value == 0)
		return (!print_exectable(alloc->exectable));
	if (has_opt(opts, 'r'))
		delete_hashentries(alloc->exectable);
	par_idx = opts->index;
	while (elem->input[par_idx] != NULL)
	{
		//TODO a faire
		ft_printf("add %s to hashtable\n");
		++par_idx;
	}
	return (0);
}
