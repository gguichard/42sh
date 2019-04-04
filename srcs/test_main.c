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

	ft_printf("Test ac « %s » :\n", strbase);
	scmd_init(&scmd, strbase);
	acs = autocomplete_cmdline(&scmd, &alloc);
	ft_printf("(str) Attendu : « %s », resultat : « %s »\n", result_needed, acs->suff);
	ft_printf("(typ) Attendu : « %d », resultat : « %d »\n", type_needed, acs->suff_type);
	if (ft_strequ(result_needed, acs->suff) && type_needed == acs->suff_type)
		ft_printf("\033[1;32mOK !\033[0m\n");
	else
		ft_printf("\033[1;31mFAUX !\033[0m\n");
	delete_ac_suff_inf(acs);
	scmd_clean(&scmd);
}

void	test_autocomplete()
{
	ft_printf(" --- Test autocompletion : ");
	//classique
	test_this_autocomplete("ech", "o", ACS_TYPE_FILE);
	test_this_autocomplete("dos", "sier", ACS_TYPE_DIR);
	test_this_autocomplete("dossier/", "executable", ACS_TYPE_FILE);
	test_this_autocomplete("lol dos", "sier", ACS_TYPE_NOTHING);
	test_this_autocomplete("lol dossier/", "", ACS_TYPE_NOTHING);
	test_this_autocomplete("dossier_", "", ACS_TYPE_NOTHING);
	test_this_autocomplete("lol dossier_", "lolfake", ACS_TYPE_FILE);
	//echappement
	test_this_autocomplete("ec\\h", "o", ACS_TYPE_FILE);
	test_this_autocomplete("ech\\", "o", ACS_TYPE_FILE);
	test_this_autocomplete("lol\\ dos", "", ACS_TYPE_NOTHING);
	//newline
	test_this_autocomplete("ech\\\n", "o", ACS_TYPE_FILE);
	test_this_autocomplete("ech\n", "", ACS_TYPE_NOTHING);
	test_this_autocomplete("e\nch", "", ACS_TYPE_NOTHING);
	//quote
	test_this_autocomplete("ech\'\n", "", ACS_TYPE_NOTHING);
	test_this_autocomplete("\'ech\n", "", ACS_TYPE_NOTHING);
	test_this_autocomplete("ech\'\\\n", "", ACS_TYPE_NOTHING);
	test_this_autocomplete("\'ech\\\n", "", ACS_TYPE_NOTHING);
	//double quote
	test_this_autocomplete("ech\"\n", "", ACS_TYPE_NOTHING);
	test_this_autocomplete("\"ech\n", "", ACS_TYPE_NOTHING);
	test_this_autocomplete("ech\"\\\n", "o", ACS_TYPE_FILE);
	test_this_autocomplete("\"ech\\\n", "o", ACS_TYPE_FILE);
}

void	test_this_alias(char *strbase, char *result_needed)
{
	t_str_cmd_inf	scmd;
	t_list			*tk_lst;

	ft_printf("Test alias « %s » :\n", strbase);
	scmd_init(&scmd, strbase);
	tk_lst = split_cmd_token(&scmd, alloc.aliastable);
	ft_printf("(str) Attendu : « %s », resultat : « %s »\n", result_needed, scmd.str);
	if (ft_strequ(result_needed, scmd.str))
		ft_printf("\033[1;32mOK !\033[0m\n");
	else
		ft_printf("\033[1;31mFAUX !\033[0m\n");
	ft_lstdel(&tk_lst, del_token);
	scmd_clean(&scmd);
}

void	test_alias()
{
	ft_printf(" --- Test expand alias : ");
	test_this_alias("basicalias", "basicresult");
	test_this_alias("blankalias", "blankresult ");
	test_this_alias("basicalias basicalias", "basicresult basicalias");
	test_this_alias("blankalias basicalias", "blankresult  basicresult");
	test_this_alias("blankalias blankalias", "blankresult  blankresult ");
	test_this_alias("blankalias basicalias blankalias", "blankresult  basicresult blankalias");
}

int		main(int argc, char **argv, char **environ)
{
	(void)argc;
	(void)argv;

	setup_alloc(&alloc, argc, argv, environ);
	set_alias_if_valid(alloc.aliastable, "aliasrec", "autrealiasrec ; autrealiasrec", NULL);
	set_alias_if_valid(alloc.aliastable, "autrealiasrec", "aliasrec ; aliasrec", NULL);
	set_alias_if_valid(alloc.aliastable, "encoreautrealiasrec", "autrealiasrec autrealiasrec", NULL);
	set_alias_if_valid(alloc.aliastable, "basicalias", "basicresult", NULL);
	set_alias_if_valid(alloc.aliastable, "blankalias", "blankresult ", NULL);
	test_autocomplete();
	ft_printf("\n");
	test_alias();
	del_alloc(&alloc);
	return (0);
}
