make -C libft
echo "make cmdline"
gcc -fsanitize=address -g -ltermcap -I . -I libft/includes *.c **/**.c libft/libft.a
