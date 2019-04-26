# 42sh
Bash-like [shell](https://en.wikipedia.org/wiki/Unix_shell)
It's a command-line interpreter

# Features
  - Prompt with line edition
  - Builtins `alias`, `bg`, `cd`, `echo`, `exit`, `export`, `fg`, `hash`, `jobs`, `set`, `source`, `test`, `type`, `unalias`, `unset`
  - Executing simple commands
  - Redirection `>`, `>>`, `<` and `|`
  - Heredoc `<<`
  - Logical operands `&&` and `||`
  - Separator `;`
  - Inhibitors `"`, `'` and `\`
  - Environment and local variables `$VAR` or `${VAR}`
  - Command substitution `$(CMD)`
  - Dynamical autocompletion
  - History
  
# Installation
##  How to do
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

## Line editing

## History

## Autocompletion

## Hash table
Commands|Functions|
:-:|:--
`hash`|List hash table contents
`hash -r`|Clear the hash table

## Job control
Commands|Functions|
:-:|:--
`jobs`|List all the current running jobs
`fg`|Bring the most recent process to foreground
`fg n`|Bring the specified jobs to foreground where `n` is the numerical value of the process found in `jobs`

# Easter egg
If you know [konami code](https://en.wikipedia.org/wiki/Konami_Code)\
Just press <kbd>esc</kbd> and do it

# Author
Name|Mail|Github
:-:|:-:|:--
Gwennaël Guichard|gguichar@student.42.fr|https://github.com/gguichard
Franck Werner|fwerner@student.42.fr|https://github.com/FranckRJ
Jonas Cohen|jocohen@student.42.fr|https://github.com/jocohen
Thibault Collard|tcollard@student.42.fr|https://github.com/tcollard
