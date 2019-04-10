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
#include "inhibitor.h"

static t_alloc	alloc;
static size_t	test_nb_count = 0;
static size_t	test_err_count = 0;

//JE SAIS PAS POURAUOI C'EST LA MAIS JE LAISSE
int			lexer_parser(const char *line, t_alloc *alloc, int fork)
{
	t_str_cmd_inf	scmd;
	t_list			*lst_tk;
	t_ast			*sort_ast;
	t_exec_opt		exec_opt;
	int				ret;

	if (!scmd_init(&scmd, line))
		return (1);
	sigs_wait_line(alloc);
	lst_tk = split_cmd_token(&scmd, alloc->aliastable);
	scmd_clean(&scmd);
	sigs_wait_line(alloc);
	if (!(sort_ast = parser(lst_tk)))
	{
		ft_lstdel(&lst_tk, del_token);
		return (1);
	}
	ft_lstdel(&lst_tk, del_token);
	sigs_wait_line(alloc);
	// if (sort_ast)
	// 	read_sort_descent(sort_ast, 0);
	ft_memset(&exec_opt, 0, sizeof(t_exec_opt));
	exec_opt.fork = fork;
	ret = analyzer(alloc, sort_ast, &exec_opt);
	if (g_sig == SIGINT)
		g_sig = 0;
	del_ast(&sort_ast);
	return (ret);
}

void	print_ok()
{
	++test_nb_count;
	ft_printf("\033[1;32mOK !\033[0m\n");
}

void	print_error()
{
	++test_nb_count;
	++test_err_count;
	ft_printf("\033[1;31mFAUX !\033[0m\n");
}

void	test_this_autocomplete(char *strbase, char *result_needed
		, t_ac_suff_type type_needed)
{
	t_str_cmd_inf scmd;
	t_ac_suff_inf *acs;

	ft_printf("Test ac « %s » :\n", strbase);
	if (!scmd_init(&scmd, strbase))
	{
		ft_printf("ERREUR scmd_init.");
		return;
	}
	acs = autocomplete_cmdline(&scmd, &alloc);
	ft_printf("(str) Attendu : « %s », resultat : « %s »\n", result_needed, (acs != NULL ? acs->suff : "ERREUR"));
	ft_printf("(typ) Attendu : « %d », resultat : « %d »\n", type_needed, (acs != NULL ? acs->suff_type : -1));
	if (acs != NULL && ft_strequ(result_needed, acs->suff) && type_needed == acs->suff_type)
		print_ok();
	else
		print_error();
	if (acs != NULL)
		delete_ac_suff_inf(acs);
	scmd_clean(&scmd);
}

void	test_autocomplete()
{
	ft_printf(" --- Test autocompletion :\n");
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
	if (!scmd_init(&scmd, strbase))
	{
		ft_printf("ERREUR scmd_init.");
		return;
	}
	tk_lst = split_cmd_token(&scmd, alloc.aliastable);
	ft_printf("(str) Attendu : « %s », resultat : « %s »\n", result_needed, scmd.str);
	if (ft_strequ(result_needed, scmd.str))
		print_ok();
	else
		print_error();
	ft_lstdel(&tk_lst, del_token);
	scmd_clean(&scmd);
}

void	test_alias()
{
	ft_printf(" --- Test expand alias :\n");
	//classique
	test_this_alias("basicalias", "basicresult");
	test_this_alias("blankalias", "blankresult ");
	test_this_alias("basicalias basicalias", "basicresult basicalias");
	test_this_alias("blankalias basicalias", "blankresult  basicresult");
	test_this_alias("blankalias blankalias", "blankresult  blankresult ");
	test_this_alias("blankalias basicalias blankalias", "blankresult  basicresult blankalias");
	//echappement
	test_this_alias("\'basicalias\'", "\'basicalias\'");
	test_this_alias("\'\'basicalias", "\'\'basicalias");
	test_this_alias("\\basicalias", "\\basicalias");
	test_this_alias("\"basicalias\"", "\"basicalias\"");
	test_this_alias("basic\\alias", "basic\\alias");
	test_this_alias("basi\"ca\"lias", "basi\"ca\"lias");
	//recur
	test_this_alias("aliasrec", "aliasrec ; aliasrec ; aliasrec ; aliasrec");
	test_this_alias("autrealiasrec", "autrealiasrec ; autrealiasrec ; autrealiasrec ; autrealiasrec");
	test_this_alias("encoreautrealiasrec", "autrealiasrec ; autrealiasrec ; autrealiasrec ; autrealiasrec autrealiasrec");
}

void	print_str_tab(char **strtab)
{
	if (strtab == NULL)
		ft_printf("NULL");
	else
	{
		ft_printf("[");
		while (*strtab != NULL)
		{
			ft_printf(" ‘%s’", *strtab);
			++strtab;
		}
		ft_printf(" ]");
	}
}

void	test_this_inhib_expand(char *strbase, char **result_needed)
{
	char	**head = inhib_expand_str(strbase, &alloc);
	char	**str_tab = head;

	ft_printf("Test inhib/exp « %s » :\n", strbase);
	ft_printf("(tab) Attendu : « ");
	print_str_tab(result_needed);
	ft_printf(" », resultat : « ");
	print_str_tab(str_tab);
	ft_printf(" »\n");
	while (str_tab != NULL && result_needed != NULL && *str_tab != NULL && *result_needed != NULL)
	{
		if (!ft_strequ(*str_tab, *result_needed))
			break;
		++str_tab;
		++result_needed;
	}
	if ((result_needed == NULL && str_tab == NULL)
			|| (result_needed != NULL && str_tab != NULL && *str_tab == NULL && *result_needed == NULL))
		print_ok();
	else
		print_error();
	ft_strtab_free(head);
}

void test_inhib_expand()
{
	ft_printf(" --- Test inhibition expand :\n");
	//expand
	test_this_inhib_expand("str", (char*[]){"str", NULL});
	test_this_inhib_expand("$EXISTEpas$EXISTE", (char*[]){"CECI", NULL});
	test_this_inhib_expand("${EXISTE}$", (char*[]){"CECI$", NULL});
	test_this_inhib_expand("$EXISTE$", (char*[]){"CECI$", NULL});
	test_this_inhib_expand("$EXISTE=$EXISTE", (char*[]){"CECI=CECI", NULL});
	test_this_inhib_expand("$EXISTE:$EXISTE", (char*[]){"CECI:CECI", NULL});
	test_this_inhib_expand("$\"EXISTE\"", (char*[]){"$EXISTE", NULL});
	test_this_inhib_expand("\"$EXISTE\"", (char*[]){"CECI", NULL});
	test_this_inhib_expand("$\'EXISTE\'", (char*[]){"$EXISTE", NULL});
	test_this_inhib_expand("\'$EXISTE\'", (char*[]){"$EXISTE", NULL});
	test_this_inhib_expand("non$EXISTE$=$", (char*[]){"nonCECI$=$", NULL});
	test_this_inhib_expand("lol${EXISTE$EXISTE}lol", NULL);
	test_this_inhib_expand("lol${tout%faux}lol", NULL);
	test_this_inhib_expand("lol${\"faux\"}lol", NULL);
	test_this_inhib_expand("lol${\"fa}ux\"}lol", NULL);
	test_this_inhib_expand("lol${bonjour\"faux\"salut}lol", NULL);
	test_this_inhib_expand("$ESTVIDE", (char*[]){NULL});
	test_this_inhib_expand("$EXISTEPAS", (char*[]){NULL});
	test_this_inhib_expand("$DEUXMOTS", (char*[]){"DEUX", "MOTS", NULL});
	test_this_inhib_expand("truc${DEUXMOTS}machin", (char*[]){"trucDEUX", "MOTSmachin", NULL});
	test_this_inhib_expand("truc\\ ${DEUXMOTS}\\ machin", (char*[]){"truc DEUX", "MOTS machin", NULL});
	test_this_inhib_expand("$TROISMOTS", (char*[]){"TROIS", "MOTS", "VOILA", NULL});
	test_this_inhib_expand("truc${TROISMOTS}machin", (char*[]){"trucTROIS", "MOTS", "VOILAmachin", NULL});
	test_this_inhib_expand("truc\\ ${TROISMOTS}\\ machin", (char*[]){"truc TROIS", "MOTS", "VOILA machin", NULL});
	//inhib
	test_this_inhib_expand("\"bonjour\"", (char*[]){"bonjour", NULL});
	test_this_inhib_expand("\"bonjour les gens\"", (char*[]){"bonjour les gens", NULL});
	test_this_inhib_expand("\"\"bonjour", (char*[]){"bonjour", NULL});
	test_this_inhib_expand("bonjour\"\"bonjour", (char*[]){"bonjourbonjour", NULL});
	test_this_inhib_expand("bonjour\"\"", (char*[]){"bonjour", NULL});
	test_this_inhib_expand("\"\"", (char*[]){"", NULL});
	test_this_inhib_expand("bonjour\"", (char*[]){"bonjour", NULL});
	test_this_inhib_expand("\"bonjour", (char*[]){"bonjour", NULL});
	test_this_inhib_expand("bonjour les gens\"", (char*[]){"bonjour les gens", NULL});
	test_this_inhib_expand("\"bonjour les gens", (char*[]){"bonjour les gens", NULL});
	test_this_inhib_expand("bonjour\"bonjour", (char*[]){"bonjourbonjour", NULL});
	test_this_inhib_expand("\"", (char*[]){"", NULL});
	test_this_inhib_expand("\'bonjour\'", (char*[]){"bonjour", NULL});
	test_this_inhib_expand("\'bonjour les gens\'", (char*[]){"bonjour les gens", NULL});
	test_this_inhib_expand("\'\'bonjour", (char*[]){"bonjour", NULL});
	test_this_inhib_expand("bonjour\'\'bonjour", (char*[]){"bonjourbonjour", NULL});
	test_this_inhib_expand("bonjour\'\'", (char*[]){"bonjour", NULL});
	test_this_inhib_expand("\'\'", (char*[]){"", NULL});
	test_this_inhib_expand("bonjour\'", (char*[]){"bonjour", NULL});
	test_this_inhib_expand("\'bonjour", (char*[]){"bonjour", NULL});
	test_this_inhib_expand("bonjour les gens\'", (char*[]){"bonjour les gens", NULL});
	test_this_inhib_expand("\'bonjour les gens", (char*[]){"bonjour les gens", NULL});
	test_this_inhib_expand("bonjour\'bonjour", (char*[]){"bonjourbonjour", NULL});
	test_this_inhib_expand("\'", (char*[]){"", NULL});
	test_this_inhib_expand("\"\\b\\$on\'jo\\\'ur_ok\\\"lol", (char*[]){"\\b$on\'jo\\\'ur_ok\"lol", NULL});
	test_this_inhib_expand("\'\\b\\$onjour", (char*[]){"\\b\\$onjour", NULL});
	test_this_inhib_expand("\"bon\\\"jo\\\\\"ur", (char*[]){"bon\"jo\\ur", NULL});
	test_this_inhib_expand("\'bon\\\\jo\\\'ur", (char*[]){"bon\\\\jo\\ur", NULL});
	//inhib + expand
	test_this_inhib_expand("\"$DEUXMOTS\"", (char*[]){"DEUX MOTS", NULL});
	test_this_inhib_expand("\"$DEUXMOTS", (char*[]){"DEUX MOTS", NULL});
	test_this_inhib_expand("\"$TROISMOTS\"", (char*[]){"TROIS\tMOTS  \t  VOILA", NULL});
	test_this_inhib_expand("\"$TROISMOTS", (char*[]){"TROIS\tMOTS  \t  VOILA", NULL});
	test_this_inhib_expand("\'$DEUXMOTS\'", (char*[]){"$DEUXMOTS", NULL});
	test_this_inhib_expand("\\$DEUXMOTS", (char*[]){"$DEUXMOTS", NULL});
	test_this_inhib_expand("\'$DEUXMOTS", (char*[]){"$DEUXMOTS", NULL});
	test_this_inhib_expand("\"$DEUX\"MOTS", (char*[]){"MOTS", NULL});
	test_this_inhib_expand("\"$TROIS\"MOTS", (char*[]){"_3_MOTS", NULL});
	test_this_inhib_expand("\'$DEUX\'MOTS", (char*[]){"$DEUXMOTS", NULL});
	test_this_inhib_expand("\'$TROIS\'MOTS", (char*[]){"$TROISMOTS", NULL});
	test_this_inhib_expand("\\$TROIS\\MOTS", (char*[]){"$TROISMOTS", NULL});
	test_this_inhib_expand("$TROIS\\MOTS", (char*[]){"_3_MOTS", NULL});
	//inhib + expand avance
	test_this_inhib_expand("bonj\"\\", (char*[]){"bonj", NULL});
	test_this_inhib_expand("bonj\"our\\", (char*[]){"bonjour", NULL});
	test_this_inhib_expand("bonj\"our\'", (char*[]){"bonjour\'", NULL});
	test_this_inhib_expand("bonj\"our\'\\", (char*[]){"bonjour\'", NULL});
	test_this_inhib_expand("bonj\"our\\\'", (char*[]){"bonjour\\\'", NULL});
	test_this_inhib_expand("bonj\'\\", (char*[]){"bonj\\", NULL});
	test_this_inhib_expand("bonj\'our\\", (char*[]){"bonjour\\", NULL});
	test_this_inhib_expand("bonj\'our\"", (char*[]){"bonjour\"", NULL});
	test_this_inhib_expand("bonj\'our\"\\", (char*[]){"bonjour\"\\", NULL});
	test_this_inhib_expand("bonj\'our\\\"", (char*[]){"bonjour\\\"", NULL});
	test_this_inhib_expand("\"$DEUXMOTS\"\'ok", (char*[]){"DEUX MOTSok", NULL});
	test_this_inhib_expand("\"$DEUXMOTS\"\"\'ok", (char*[]){"DEUX MOTS\'ok", NULL});
	test_this_inhib_expand("$DEUXMOTS\"\'ok", (char*[]){"DEUX", "MOTS\'ok", NULL});
	test_this_inhib_expand("$DEUXMOTS\'ok", (char*[]){"DEUX", "MOTSok", NULL});
	test_this_inhib_expand("$DEUXMOTS\"ok", (char*[]){"DEUX", "MOTSok", NULL});
	//expand 1 mot espace pref/suff
	test_this_inhib_expand("$ESPACEPREF", (char*[]){"LOL", NULL});
	test_this_inhib_expand("coucou$ESPACEPREF", (char*[]){"coucou", "LOL", NULL});
	test_this_inhib_expand("$ESPACEPREF/ok", (char*[]){"LOL/ok", NULL});
	test_this_inhib_expand("coucou$ESPACEPREF/ok", (char*[]){"coucou", "LOL/ok", NULL});
	test_this_inhib_expand("$ESPACESUFF", (char*[]){"LOL", NULL});
	test_this_inhib_expand("coucou$ESPACESUFF", (char*[]){"coucouLOL", NULL});
	test_this_inhib_expand("$ESPACESUFF/ok", (char*[]){"LOL", "/ok", NULL});
	test_this_inhib_expand("coucou$ESPACESUFF/ok", (char*[]){"coucouLOL", "/ok", NULL});
	test_this_inhib_expand("$ESPACEPREFSUFF", (char*[]){"LOL", NULL});
	test_this_inhib_expand("coucou$ESPACEPREFSUFF", (char*[]){"coucou", "LOL", NULL});
	test_this_inhib_expand("$ESPACEPREFSUFF/ok", (char*[]){"LOL", "/ok", NULL});
	test_this_inhib_expand("coucou$ESPACEPREFSUFF/ok", (char*[]){"coucou", "LOL", "/ok", NULL});
	test_this_inhib_expand("\"$ESPACEPREF\"", (char*[]){" LOL", NULL});
	test_this_inhib_expand("\"coucou$ESPACEPREF\"", (char*[]){"coucou LOL", NULL});
	test_this_inhib_expand("\"$ESPACEPREF/ok\"", (char*[]){" LOL/ok", NULL});
	test_this_inhib_expand("\"coucou$ESPACEPREF/ok\"", (char*[]){"coucou LOL/ok", NULL});
	test_this_inhib_expand("\"$ESPACESUFF\"", (char*[]){"LOL ", NULL});
	test_this_inhib_expand("\"coucou$ESPACESUFF\"", (char*[]){"coucouLOL ", NULL});
	test_this_inhib_expand("\"$ESPACESUFF/ok\"", (char*[]){"LOL /ok", NULL});
	test_this_inhib_expand("\"coucou$ESPACESUFF/ok\"", (char*[]){"coucouLOL /ok", NULL});
	test_this_inhib_expand("\"$ESPACEPREFSUFF\"", (char*[]){" LOL ", NULL});
	test_this_inhib_expand("\"coucou$ESPACEPREFSUFF\"", (char*[]){"coucou LOL ", NULL});
	test_this_inhib_expand("\"$ESPACEPREFSUFF/ok\"", (char*[]){" LOL /ok", NULL});
	test_this_inhib_expand("\"coucou$ESPACEPREFSUFF/ok\"", (char*[]){"coucou LOL /ok", NULL});
	//expand 2 mot espace pref/suff
	test_this_inhib_expand("$DEUXMOTSESPACEPREF", (char*[]){"LOL", "MDR", NULL});
	test_this_inhib_expand("coucou$DEUXMOTSESPACEPREF", (char*[]){"coucou", "LOL", "MDR", NULL});
	test_this_inhib_expand("$DEUXMOTSESPACEPREF/ok", (char*[]){"LOL", "MDR/ok", NULL});
	test_this_inhib_expand("coucou$DEUXMOTSESPACEPREF/ok", (char*[]){"coucou", "LOL", "MDR/ok", NULL});
	test_this_inhib_expand("$DEUXMOTSESPACESUFF", (char*[]){"LOL", "MDR", NULL});
	test_this_inhib_expand("coucou$DEUXMOTSESPACESUFF", (char*[]){"coucouLOL", "MDR", NULL});
	test_this_inhib_expand("$DEUXMOTSESPACESUFF/ok", (char*[]){"LOL", "MDR", "/ok", NULL});
	test_this_inhib_expand("coucou$DEUXMOTSESPACESUFF/ok", (char*[]){"coucouLOL", "MDR", "/ok", NULL});
	test_this_inhib_expand("$DEUXMOTSESPACEPREFSUFF", (char*[]){"LOL", "MDR", NULL});
	test_this_inhib_expand("coucou$DEUXMOTSESPACEPREFSUFF", (char*[]){"coucou", "LOL", "MDR", NULL});
	test_this_inhib_expand("$DEUXMOTSESPACEPREFSUFF/ok", (char*[]){"LOL", "MDR", "/ok", NULL});
	test_this_inhib_expand("coucou$DEUXMOTSESPACEPREFSUFF/ok", (char*[]){"coucou", "LOL", "MDR", "/ok", NULL});
	test_this_inhib_expand("\"$DEUXMOTSESPACEPREF\"", (char*[]){" LOL MDR", NULL});
	test_this_inhib_expand("\"coucou$DEUXMOTSESPACEPREF\"", (char*[]){"coucou LOL MDR", NULL});
	test_this_inhib_expand("\"$DEUXMOTSESPACEPREF/ok\"", (char*[]){" LOL MDR/ok", NULL});
	test_this_inhib_expand("\"coucou$DEUXMOTSESPACEPREF/ok\"", (char*[]){"coucou LOL MDR/ok", NULL});
	test_this_inhib_expand("\"$DEUXMOTSESPACESUFF\"", (char*[]){"LOL MDR ", NULL});
	test_this_inhib_expand("\"coucou$DEUXMOTSESPACESUFF\"", (char*[]){"coucouLOL MDR ", NULL});
	test_this_inhib_expand("\"$DEUXMOTSESPACESUFF/ok\"", (char*[]){"LOL MDR /ok", NULL});
	test_this_inhib_expand("\"coucou$DEUXMOTSESPACESUFF/ok\"", (char*[]){"coucouLOL MDR /ok", NULL});
	test_this_inhib_expand("\"$DEUXMOTSESPACEPREFSUFF\"", (char*[]){" LOL MDR ", NULL});
	test_this_inhib_expand("\"coucou$DEUXMOTSESPACEPREFSUFF\"", (char*[]){"coucou LOL MDR ", NULL});
	test_this_inhib_expand("\"$DEUXMOTSESPACEPREFSUFF/ok\"", (char*[]){" LOL MDR /ok", NULL});
	test_this_inhib_expand("\"coucou$DEUXMOTSESPACEPREFSUFF/ok\"", (char*[]){"coucou LOL MDR /ok", NULL});
	//home
	test_this_inhib_expand("~", (char*[]){"MAISON", NULL});
	test_this_inhib_expand("~userexistepas", (char*[]){"~userexistepas", NULL});
	test_this_inhib_expand("~+non", (char*[]){"~+non", NULL});
	test_this_inhib_expand("\'~\'", (char*[]){"~", NULL});
	test_this_inhib_expand("\"~\"", (char*[]){"~", NULL});
	test_this_inhib_expand("\\~", (char*[]){"~", NULL});
	test_this_inhib_expand("\"\\~\"", (char*[]){"\\~", NULL});
	test_this_inhib_expand("~/", (char*[]){"MAISON/", NULL});
	test_this_inhib_expand("~/$EXISTE", (char*[]){"MAISON/CECI", NULL});
	test_this_inhib_expand("$EXISTEPAS~", (char*[]){"~", NULL});
	test_this_inhib_expand("${EXISTEPAS}~", (char*[]){"~", NULL});
	test_this_inhib_expand("oui/~", (char*[]){"oui/~", NULL});
	test_this_inhib_expand("oui/~/", (char*[]){"oui/~/", NULL});
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
	create_var(&alloc.vars, "EXISTE", "CECI", 1);
	create_var(&alloc.vars, "ESTVIDE", "", 1);
	create_var(&alloc.vars, "DEUXMOTS", "DEUX MOTS", 1);
	create_var(&alloc.vars, "TROISMOTS", "TROIS\tMOTS  \t  VOILA", 1);
	create_var(&alloc.vars, "DEUX", "", 1);
	create_var(&alloc.vars, "TROIS", "_3_", 1);
	create_var(&alloc.vars, "ESPACEPREF", " LOL", 1);
	create_var(&alloc.vars, "ESPACESUFF", "LOL ", 1);
	create_var(&alloc.vars, "ESPACEPREFSUFF", " LOL ", 1);
	create_var(&alloc.vars, "DEUXMOTSESPACEPREF", " LOL MDR", 1);
	create_var(&alloc.vars, "DEUXMOTSESPACESUFF", "LOL MDR ", 1);
	create_var(&alloc.vars, "DEUXMOTSESPACEPREFSUFF", " LOL MDR ", 1);
	create_var(&alloc.vars, "ESTVIDE", "", 1);
	create_var(&alloc.vars, "HOME", "MAISON", 1);
	unset_var(&alloc.vars, "EXISTEPAS");
	test_autocomplete();
	ft_printf("\n");
	test_alias();
	ft_printf("\n");
	test_inhib_expand();
	if (test_err_count > 0)
	{
		ft_printf("\033[1;31m%zu erreurs pour %zu tests !\033[0m\n", test_err_count, test_nb_count);
	}
	else
	{
		ft_printf("\033[1;32mPas d'erreurs pour %zu tests !\033[0m\n", test_nb_count);
	}
	del_alloc(&alloc);
	return (0);
}
