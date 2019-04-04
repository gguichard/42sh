#include <unistd.h>
#include <stdlib.h>
#include "libft.h"
#include "shell.h"
#include "cmdline.h"
#include "autocomplete.h"
#include "execution.h"
#include "aliastable.h"
#include "parser_lexer.h"
#include "str_cmd_inf.h"
#include "split_cmd_token.h"
#include "builtins.h"
#include "job.h"
#include "signals.h"
#include "vars.h"

static t_alloc	alloc;

//JE SAIS PAS POURAUOI C'EST LA MAIS JE LAISSE
void	lexer_parser(const char *line, t_alloc *alloc)
{
	t_str_cmd_inf	scmd;
	t_list			*lst_tk;
	t_ast			*sort_ast;
	t_exec_opt		exec_opt;

	if (!scmd_init(&scmd, line))
		return ;
	sigs_wait_line(alloc);
	lst_tk = split_cmd_token(&scmd, alloc->aliastable);
	scmd_clean(&scmd);
	sigs_wait_line(alloc);
	if (!(sort_ast = parser(lst_tk)))
		return ;
	ft_lstdel(&lst_tk, del_token);
	sigs_wait_line(alloc);
	// if (sort_ast)
	// 	read_sort_descent(sort_ast, 0);
	ft_memset(&exec_opt, 0, sizeof(t_exec_opt));
	alloc->ret_val = analyzer(alloc, sort_ast, &exec_opt);
	if (g_sig == SIGINT)
		g_sig = 0;
	del_ast(&sort_ast);
}

void	test_this_autocomplete(char *strbase, char *result_needed
		, t_ac_suff_type type_needed)
{
	t_str_cmd_inf scmd;
	t_ac_suff_inf *acs;

	printf("Test ac « %s » :\n", strbase);
	scmd_init(&scmd, strbase);
	acs = autocomplete_cmdline(&scmd, &alloc);
	printf("(str) Attendu : « %s », resultat : « %s »\n", result_needed, acs->suff);
	printf("(typ) Attendu : « %d », resultat : « %d »\n", type_needed, acs->suff_type);
	if (ft_strequ(result_needed, acs->suff) && type_needed == acs->suff_type)
		printf("\033[1;32mOK !\033[0m\n");
	else
		printf("\033[1;31mFAUX !\033[0m\n");
	delete_ac_suff_inf(acs);
	scmd_clean(&scmd);
}

void	test_autocomplete()
{
	test_this_autocomplete("ech", "o", ACS_TYPE_FILE);
	test_this_autocomplete("ec\\h", "o", ACS_TYPE_FILE);
	test_this_autocomplete("ech\\", "o", ACS_TYPE_FILE);
	test_this_autocomplete("ech\\\n", "o", ACS_TYPE_FILE);
	test_this_autocomplete("ech\n", "", ACS_TYPE_NOTHING);
	test_this_autocomplete("e\nch", "", ACS_TYPE_NOTHING);
	test_this_autocomplete("ech\'\n", "", ACS_TYPE_NOTHING);
	test_this_autocomplete("\'ech\n", "", ACS_TYPE_NOTHING);
	test_this_autocomplete("ech\'\\\n", "", ACS_TYPE_NOTHING);
	test_this_autocomplete("\'ech\\\n", "", ACS_TYPE_NOTHING);
	test_this_autocomplete("ech\"\n", "", ACS_TYPE_NOTHING);
	test_this_autocomplete("\"ech\n", "", ACS_TYPE_NOTHING);
	test_this_autocomplete("ech\"\\\n", "o", ACS_TYPE_FILE);
	test_this_autocomplete("\"ech\\\n", "o", ACS_TYPE_FILE);
}

int		main(int argc, char **argv, char **environ)
{
	(void)argc;
	(void)argv;

	setup_alloc(&alloc, argc, argv, environ);
	//set_alias_if_valid(alloc.aliastable, "ceciestalias", "coucouvaleur", NULL);
	//set_alias_if_valid(alloc.aliastable, "ceciestautre", "kokokoko", NULL);
	//set_alias_if_valid(alloc.aliastable, "autretest", "nonpe", NULL);
	test_autocomplete();
	del_alloc(&alloc);
	return (0);
}
