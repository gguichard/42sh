make -C libft
echo "make cmdline"
gcc -Wall -Wextra -Werror -g -fsanitize=address -ltermcap -I . -I libft/includes *.c **/**.c libft/libft.a
