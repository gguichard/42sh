
NAME	=	42sh

END		=	\x1b[0m
GREEN	=	\x1b[32m

INC_DIR	=	includes

CC		=	gcc
CFLAGS 	=	-Wall -g -Werror -Wextra -I libft/includes -I $(INC_DIR)
LDFLAGS	=	-Llibft
LDLIBS	=	-lft -ltermcap

PATH_BUILT		=	builtins
PATH_ERROR		=	error
PATH_EXEC		=	exec
PATH_LEXER		=	lexer
PATH_PARSER		=	parser
PATH_OPERATOR	=	operator
PATH_TOOLS		=	tools
PATH_HASHTABLE	=	hashtable
PATH_PATH		=	path
PATH_TOKEN		=	token
PATH_CMDLINE	=	cmdline
PATH_EXPAND		=	expand
PATH_INHIB		=	inhib
PATH_AUTOCOMPL	=	autocomplete
PATH_VARS		=	vars
PATH_JOB		=	job

SRC_DIR	=	srcs
SRC 	=	\
main.c		\
$(PATH_ERROR)/exec_error.c \
$(PATH_ERROR)/error_utils.c \
$(PATH_LEXER)/replace_quote.c \
$(PATH_LEXER)/str_cmd_inf.c \
$(PATH_LEXER)/str_cmd_inf_utils.c \
$(PATH_LEXER)/str_cmd_inf_escape.c \
$(PATH_LEXER)/str_cmd_inf_get.c \
$(PATH_PARSER)/parser.c \
$(PATH_PARSER)/sort_ast.c \
$(PATH_PARSER)/ast_creation.c \
$(PATH_PARSER)/token_analyzer.c \
$(PATH_PARSER)/redirect_inf.c \
$(PATH_EXEC)/analyzer.c \
$(PATH_EXEC)/exec_builtin.c \
$(PATH_EXEC)/exec_input.c \
$(PATH_EXEC)/redir_input.c \
$(PATH_EXEC)/redir_output.c \
$(PATH_EXEC)/redir_utils.c \
$(PATH_EXEC)/redirections.c \
$(PATH_EXEC)/source_file.c \
$(PATH_EXEC)/temp_file.c \
$(PATH_EXEC)/var_assigns.c \
$(PATH_OPERATOR)/job_control.c \
$(PATH_OPERATOR)/pipe.c \
$(PATH_OPERATOR)/pipe_fork.c \
$(PATH_TOOLS)/clean_tools.c \
$(PATH_TOOLS)/home_tools.c \
$(PATH_TOOLS)/lexer_tools.c \
$(PATH_TOOLS)/ast_tools.c \
$(PATH_TOOLS)/builtins_tools.c \
$(PATH_TOOLS)/alloc_starter_tools.c \
$(PATH_TOOLS)/execution_path.c \
$(PATH_BUILT)/cd.c \
$(PATH_BUILT)/cd_path.c \
$(PATH_BUILT)/cd_utils.c \
$(PATH_BUILT)/echo.c \
$(PATH_BUILT)/exit.c \
$(PATH_BUILT)/hash.c \
$(PATH_BUILT)/alias.c \
$(PATH_BUILT)/unalias.c \
$(PATH_BUILT)/bg.c \
$(PATH_BUILT)/export.c \
$(PATH_BUILT)/fg.c \
$(PATH_BUILT)/jobs.c \
$(PATH_BUILT)/set.c \
$(PATH_BUILT)/type.c \
$(PATH_BUILT)/unset.c \
$(PATH_BUILT)/test.c \
$(PATH_BUILT)/test/test_builtin_utils.c \
$(PATH_BUILT)/test/file_tests_1.c \
$(PATH_BUILT)/test/file_tests_2.c \
$(PATH_BUILT)/test/file_tests_flags.c \
$(PATH_BUILT)/test/file_tests_rights.c \
$(PATH_BUILT)/test/string_tests.c \
$(PATH_BUILT)/test/integer_tests.c \
$(PATH_BUILT)/exec.c \
$(PATH_BUILT)/source.c \
$(PATH_TOOLS)/waitline_pipes.c \
$(PATH_TOOLS)/clean_ast.c \
$(PATH_TOOLS)/signal_management.c \
$(PATH_TOOLS)/signal_handlers.c \
$(PATH_HASHTABLE)/exectable.c \
$(PATH_HASHTABLE)/aliastable.c \
$(PATH_HASHTABLE)/hashtable.c \
$(PATH_HASHTABLE)/hashtable_delete.c \
$(PATH_HASHTABLE)/hashtable_utils.c \
$(PATH_PATH)/check_path.c \
$(PATH_PATH)/convert_path_to_tab.c \
$(PATH_PATH)/join_path.c \
$(PATH_PATH)/search_in_path.c \
$(PATH_TOKEN)/token_inf.c \
$(PATH_TOKEN)/split_cmd_token.c \
$(PATH_TOKEN)/split_cmd_token_internal.c \
$(PATH_TOKEN)/split_cmd_token_add.c \
$(PATH_TOKEN)/split_cmd_token_add_alias_utils.c \
$(PATH_TOKEN)/split_cmd_token_utils.c \
$(PATH_CMDLINE)/cmdline.c \
$(PATH_CMDLINE)/create_prompt.c \
$(PATH_CMDLINE)/create_prompt_utils.c \
$(PATH_CMDLINE)/cursor_utils.c \
$(PATH_CMDLINE)/handle_sigs.c \
$(PATH_CMDLINE)/heredoc.c \
$(PATH_CMDLINE)/history.c \
$(PATH_CMDLINE)/history_events.c \
$(PATH_CMDLINE)/history_expand.c \
$(PATH_CMDLINE)/history_utils.c \
$(PATH_CMDLINE)/input.c \
$(PATH_CMDLINE)/print_cmdline.c \
$(PATH_CMDLINE)/print_cmdline_utils.c \
$(PATH_CMDLINE)/prompt.c \
$(PATH_CMDLINE)/seq_keys.c \
$(PATH_CMDLINE)/setup_cmdline.c \
$(PATH_CMDLINE)/setup_term.c \
$(PATH_CMDLINE)/tcaps_utils.c \
$(PATH_CMDLINE)/mode_common/cursor_home_end.c \
$(PATH_CMDLINE)/mode_common/cursor_line.c \
$(PATH_CMDLINE)/mode_common/cursor_moves.c \
$(PATH_CMDLINE)/mode_common/cursor_updown.c \
$(PATH_CMDLINE)/mode_common/cursor_words.c \
$(PATH_CMDLINE)/mode_common/enter_exit_visual.c \
$(PATH_CMDLINE)/mode_common/mode_utils.c \
$(PATH_CMDLINE)/mode_common/special_hooks.c \
$(PATH_CMDLINE)/mode_insert/append_ac_cmdline.c \
$(PATH_CMDLINE)/mode_insert/autocomplete.c \
$(PATH_CMDLINE)/mode_insert/del_hooks.c \
$(PATH_CMDLINE)/mode_insert/history_hooks.c \
$(PATH_CMDLINE)/mode_insert/konami_code.c \
$(PATH_CMDLINE)/mode_insert/misc_hooks.c \
$(PATH_CMDLINE)/mode_insert/print_autocomplete.c \
$(PATH_CMDLINE)/mode_insert/stop_reading.c \
$(PATH_CMDLINE)/mode_visual/copy_paste_clipboard.c \
$(PATH_CMDLINE)/mode_visual/select_utils.c \
$(PATH_CMDLINE)/mode_visual/visual_utils.c \
$(PATH_AUTOCOMPL)/autocomplete.c \
$(PATH_AUTOCOMPL)/autocomplete_utils.c \
$(PATH_AUTOCOMPL)/ac_check_for.c \
$(PATH_AUTOCOMPL)/ac_cmdline.c \
$(PATH_AUTOCOMPL)/ac_cmdline_tk.c \
$(PATH_AUTOCOMPL)/ac_cmdline_find.c \
$(PATH_AUTOCOMPL)/ac_rdir_utils.c \
$(PATH_AUTOCOMPL)/ac_suff_utils.c \
$(PATH_AUTOCOMPL)/utils.c \
$(PATH_VARS)/env.c \
$(PATH_VARS)/shell_vars.c \
$(PATH_VARS)/unset_var_utils.c \
$(PATH_VARS)/var_utils.c \
$(PATH_VARS)/vars.c \
$(PATH_JOB)/job_utils.c \
$(PATH_JOB)/delete_jobs.c \
$(PATH_JOB)/init_job_state.c \
$(PATH_JOB)/pid_lst.c \
$(PATH_JOB)/print_jobs_tools.c \
$(PATH_JOB)/print_state_tools.c \
$(PATH_JOB)/redirect_terminal_control.c \
$(PATH_JOB)/refresh_jobs.c \
$(PATH_JOB)/simple_display_job.c \
$(PATH_JOB)/state_jobs_tools.c \
$(PATH_EXPAND)/expand.c \
$(PATH_EXPAND)/expand_tools.c \
$(PATH_EXPAND)/expand_multi_tk.c \
$(PATH_EXPAND)/ft_splitwhitespace.c \
$(PATH_EXPAND)/cmd_substitute.c \
$(PATH_EXPAND)/subcmd_wait_n_signal.c \
$(PATH_INHIB)/inhibitor.c \
$(PATH_INHIB)/inhibitor_remove.c \
$(PATH_INHIB)/inhibitor_utils.c \
$(PATH_INHIB)/inhibitor_tools.c \
$(PATH_INHIB)/inhibitor_update_pos.c

OBJ_DIR		=	.obj
OBJ			=	$(SRC:.c=.o)
DEP			=	$(OBJ:.o=.d)

all: $(NAME)

$(NAME): $(addprefix $(OBJ_DIR)/,$(OBJ))
	$(MAKE) -C libft
	$(CC) $(CFLAGS) $(LDFLAGS) $(LDLIBS) -o $@ $(addprefix $(OBJ_DIR)/,$(OBJ))
	@echo "\n$(NAME):\t\t\t$(GREEN)[READY]\n\t\t¯\_(ツ)_/¯$(END)"

-include $(addprefix $(OBJ_DIR)/,$(DEP))

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c | $(OBJ_DIR)
	$(CC) $(CFLAGS) -MMD -o $@ -c $<

$(OBJ_DIR):
	@/bin/mkdir $(OBJ_DIR) 2> /dev/null || true
	@/bin/mkdir $(OBJ_DIR)/$(PATH_BUILT) 2> /dev/null || true
	@/bin/mkdir $(OBJ_DIR)/$(PATH_BUILT)/test 2> /dev/null || true
	@/bin/mkdir $(OBJ_DIR)/$(PATH_ERROR) 2> /dev/null || true
	@/bin/mkdir $(OBJ_DIR)/$(PATH_EXEC) 2> /dev/null || true
	@/bin/mkdir $(OBJ_DIR)/$(PATH_LEXER) 2> /dev/null || true
	@/bin/mkdir $(OBJ_DIR)/$(PATH_PARSER) 2> /dev/null || true
	@/bin/mkdir $(OBJ_DIR)/$(PATH_OPERATOR) 2> /dev/null || true
	@/bin/mkdir $(OBJ_DIR)/$(PATH_JOB) 2> /dev/null || true
	@/bin/mkdir $(OBJ_DIR)/$(PATH_TOOLS) 2> /dev/null || true
	@/bin/mkdir $(OBJ_DIR)/$(PATH_HASHTABLE) 2> /dev/null || true
	@/bin/mkdir $(OBJ_DIR)/$(PATH_PATH) 2> /dev/null || true
	@/bin/mkdir $(OBJ_DIR)/$(PATH_TOKEN) 2> /dev/null || true
	@/bin/mkdir $(OBJ_DIR)/$(PATH_CMDLINE) 2> /dev/null || true
	@/bin/mkdir $(OBJ_DIR)/$(PATH_CMDLINE)/mode_common 2> /dev/null || true
	@/bin/mkdir $(OBJ_DIR)/$(PATH_CMDLINE)/mode_insert 2> /dev/null || true
	@/bin/mkdir $(OBJ_DIR)/$(PATH_CMDLINE)/mode_visual 2> /dev/null || true
	@/bin/mkdir $(OBJ_DIR)/$(PATH_EXPAND) 2> /dev/null || true
	@/bin/mkdir $(OBJ_DIR)/$(PATH_INHIB) 2> /dev/null || true
	@/bin/mkdir $(OBJ_DIR)/$(PATH_AUTOCOMPL) 2> /dev/null || true
	@/bin/mkdir $(OBJ_DIR)/$(PATH_VARS) 2> /dev/null || true

clean:
	$(MAKE) -C libft clean
	/bin/rm -rf $(OBJ_DIR)

fclean: clean
	$(MAKE) -C libft fclean
	/bin/rm -f $(NAME)
	@echo "$(NAME):\t\t\t$(GREEN)[CLEAN]$(END)"

re:
	$(MAKE) fclean
	$(MAKE) all

debug: $(addprefix $(OBJ_DIR)/,$(OBJ))
	$(MAKE) -C libft
	$(CC) $(CFLAGS) -fsanitize=address -g $(LDFLAGS) $(LDLIBS) -o $@ $^

.PHONY: all clean fclean $(OBJ_RULE)
