#include <stdlib.h>
#include <string.h>

char	*p(char *s, char *str)
{
	char	buffer[4096];

	puts(str);
	read(0, buffer, 4096);
	*strchr(buffer, '\n') = 0;
	return (strncpy(s, buffer, 20));
}

char	*pp(char *buffer)
{
	char			s1[20];
	char			s2[20];
	unsigned int	len;

	p(s2, " - ");
	p(s1, " - ");
	strcpy(buffer, s2);
	len = strlen(buffer);
	buffer[len] = ' ';
	buffer[len + 1] = 0;
	return (strcat(buffer, s1));
}

int main() {
    char str[54];

    pp(str);
    puts(str);
    return 0;
}