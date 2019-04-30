# 42sh
Bash-like [shell](https://en.wikipedia.org/wiki/Unix_shell)
It's a command-line interpreter

# Features
  - Prompt with line edition
  - Builtins `alias`, `bg`, `cd`, `echo`, `exit`, `export`, `fg`, `hash`, `jobs`, `set`, `source`, `test`, `type`, `unalias`, `unset`
  - Executing simple commands
  - Execute in background `&`
  - Redirection `>`, `>>`, `<` and `|`
  - Heredoc `<<`
  - Logical operands `&&` and `||`
  - Separator `;`
  - Inhibitors `"`, `'` and `\`
  - Environment and local variables `$VAR` or `${VAR}`
  - Command substitution `$(CMD)`
  - Dynamical autocompletion
  - History (saved in `~/.42sh_history`)
  - PS1 environment variable to custom shell prompt ([wiki](https://ss64.com/bash/syntax-prompt.html))
  
# Installation
```
$>  git clone https://github.com/tcollard/42sh.git
$>  cd 42sh && make
$>  ./42sh
```
## Rules make
  - `make` compilation of binary executable
  - `make clean` delete all `.o` files
  - `make fclean` delete all `.o` files and `./42sh`
  - `make re` do `fclean` and `make`

# Shell

## History 
Commands|Functions|
:-:|:--
`!!`|Last command in history
`!word`|Look for the last command starting by `word`
`![-]number`|Look for the command at index `number`, `-` define that the search start from the end

## Alias
Commands|Functions|
:-:|:--
`alias`|List all the current aliases set
`alias [name[=value] ...]`|Sets an alias for `name` so it replaces as `value` internally 
`unalias [-a] name [name ...]`|Remove the `name` arguments of the alias table. `-a` removes all aliases

## Hash table
Commands|Functions|
:-:|:--
`hash`|List hash table contents
`hash -r`|Clear the hash table

## Job control
Commands|Functions|
:-:|:--
`jobs [n]`|List all the current running jobs, `n` specify the job you want (the form is in [job id form](http://pubs.opengroup.org/onlinepubs/9699919799/basedefs/V1_chap03.html#tag_03_204) or it can be a numerical value) 
`fg [n]`|Bring the specified job to foreground
`bg [n]`|Bring the specified job to background

All of our shell functionalities are in [Posix norm](https://pubs.opengroup.org/onlinepubs/9699919799.2018edition/).

# Author
Name|Mail|Github
:-:|:-:|:--
Gwennaël Guichard|gguichar@student.42.fr|https://github.com/gguichard
Franck Werner|fwerner@student.42.fr|https://github.com/FranckRJ
Jonas Cohen|jocohen@student.42.fr|https://github.com/jocohen
Thibault Collard|tcollard@student.42.fr|https://github.com/tcollard
