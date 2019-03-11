make -C libft
echo "make cmdline"
gcc -I libft/includes *.c libft/libft.a -o cmdline
