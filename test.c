struct po
{
	char		*s;
	struct po *n;
};

int main()
{
	struct po *pl;

	pl->n->s = 0;
}
