#include<stdio.h>
#include<string.h>
#include<unistd.h>
#include<stdlib.h>
#include<linux/kernel.h>
#include<sys/syscall.h>
#include<errno.h>

#define true 1
#define _POSIX_ARG_MAX 50
#define PATH_MAX 50

char **split(char str[])
{
	char **splitted = malloc(2 * sizeof(char *));
	char *token = strtok(str, " ");

	splitted[0] = malloc(_POSIX_ARG_MAX * sizeof(char));
	strcpy(splitted[0], token);
	while (true) {
		token = strtok(NULL, " ");
		if (token == NULL)
			break;
		splitted[1] = malloc(_POSIX_ARG_MAX * sizeof(char));
		strcpy(splitted[1], token);
	}
	printf("%s\n%s\n", splitted[0], splitted[1]);
	return splitted;
}


int parse_input(char str[])
{
	// printf("str in parse_input: %s\n", str);
	char **splitted = split(str);
	int sig = 1;
	
	if (strcmp(splitted[0], "exit") == 0) {
		sig = 0;
	} else if (strcmp(splitted[0], "cd") == 0) {
		chdir(splitted[1]);
	} else if (strcmp(splitted[0], "getcpu") == 0) {
		int cpuid = syscall(436);
		
		if(cpuid == -1) {
			fprintf(stderr, "error: %s\n", strerror(errno));
		} else {
			printf("cpuid: %d\n", cpuid);
		}
	}
	for(int i = 0; i < 2; i++)
		free(splitted[i]);
	free(splitted);
	return sig;
}

void print_cwd(void)
{
	char cwd[PATH_MAX];

	if (getcwd(cwd, sizeof(cwd)) != NULL) {
		printf("%s", cwd);
	} else {
		printf("getcwd() error.");
	}
}

int main(void)
{
	char str[_POSIX_ARG_MAX];
	int sig = 1;

	while (true) {
		print_cwd();
		printf("$ ");
		fgets(str, sizeof(str), stdin);
		str[strcspn(str, "\n")] = 0;
		// printf("get input: %sE\n", str);
		sig = parse_input(str);
		// printf("sig: %d\n", sig);
		if (sig == 0)
			break;
	}
	return 0;
}
