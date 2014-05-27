/**
 *	Tema 2 SO: Mini-shell
 *
 *	nume: Bojin
 *	prenume: Andrei Florian
 *	grupa/seria: 333CA
 */

#include <assert.h>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <sys/types.h>
#include <sys/stat.h>
#include <sys/wait.h>

#include <fcntl.h>
#include <unistd.h>

#include "utils.h"

#define READ		0
#define WRITE		1

/**
 * Internal change-directory command.
 */
static bool shell_cd(word_t *dir)
{
	/* TODO execute cd */
	int ret;
	// sanity check
	if(dir != NULL) {
		// schimba directorul curent in directorul parametru
		ret = chdir(dir->string);	
		if(ret == 0)
			return true;
	}
	return false;
}

/**
 * Internal exit/quit command.
 */
static int shell_exit()
{
	/* TODO execute exit/quit */
	exit(0);

	return SHELL_EXIT; /* TODO replace with actual exit code */
}

/**
 * Concatenate parts of the word to obtain the command
 */
static char *get_word(word_t *s)
{
	int string_length = 0;
	int substring_length = 0;

	char *string = NULL;
	char *substring = NULL;

	while (s != NULL) {
		substring = strdup(s->string);

		if (substring == NULL) {
			return NULL;
		}

		if (s->expand == true) {
			char *aux = substring;
			substring = getenv(substring);

			/* prevents strlen from failing */
			if (substring == NULL) {
				substring = calloc(1, sizeof(char));
				if (substring == NULL) {
					free(aux);
					return NULL;
				}
			}

			free(aux);
		}

		substring_length = strlen(substring);

		string = realloc(string, string_length + substring_length + 1);
		if (string == NULL) {
			if (substring != NULL)
				free(substring);
			return NULL;
		}

		memset(string + string_length, 0, substring_length + 1);

		strcat(string, substring);
		string_length += substring_length;

		if (s->expand == false) {
			free(substring);
		}

		s = s->next_part;
	}

	return string;
}

/**
 * Concatenate command arguments in a NULL terminated list in order to pass
 * them directly to execv.
 */
static char **get_argv(simple_command_t *command, int *size)
{
	char **argv;
	word_t *param;

	int argc = 0;
	argv = calloc(argc + 1, sizeof(char *));
	assert(argv != NULL);

	argv[argc] = get_word(command->verb);
	assert(argv[argc] != NULL);

	argc++;

	param = command->params;
	while (param != NULL) {
		argv = realloc(argv, (argc + 1) * sizeof(char *));
		assert(argv != NULL);

		argv[argc] = get_word(param);
		assert(argv[argc] != NULL);

		param = param->next_word;
		argc++;
	}

	argv = realloc(argv, (argc + 1) * sizeof(char *));
	assert(argv != NULL);

	argv[argc] = NULL;
	*size = argc;

	return argv;
}

/**
 * Parse a simple command (internal, environment variable assignment,
 * external command).
 */
static int parse_simple(simple_command_t *s, int level, command_t *father)
{
	int ret, fd, statusPtr, size;
	char *filename, **argv;
	pid_t pid;

	/* TODO sanity checks */
	// verifica legitimitatea comenzii date ca parametru
	if(s == NULL || s->verb == NULL) {
		fprintf(stderr, "Nu trebuie o comanda nula\n");
		return 0;
	}
	/* TODO if builtin command, execute the command */
	// daca avem de a face cu comanda 'cd'
	if(strcmp(s->verb->string, "cd") == 0) {
		// verificam daca este cazul unei redirectari a comenzii
		if(s->out != NULL || s->err != NULL) {
			// identificam unde se face redirectarea: out sau err
			if(s->out != NULL)
				filename = get_word(s->out);
			else
				filename = get_word(s->err);
			// deschidem fisierul respectiv in modul corespunzator
			if(s->io_flags == IO_OUT_APPEND || s->io_flags == IO_ERR_APPEND)
				fd = open(filename, O_CREAT | O_WRONLY | O_APPEND, 0644);
			else
				fd = open(filename, O_CREAT | O_WRONLY | O_TRUNC, 0644);
			// se elibereaza memoria si se inchid fisierele deschise
			free(filename);
			close(fd);
		}
		// se executa comanda 'cd' si se returneaza valoarea ei
		ret = shell_cd(s->params);
		return ret;	
	}
	// daca avem de a face cu comanda 'quit' sau 'exit'
	if(strcmp(s->verb->string, "quit") == 0 || 
	   strcmp(s->verb->string, "exit") == 0) {
		// se executa comanda 'exit' si se returneaza valoarea ei
		ret = shell_exit();
		return ret;	
	}
	/* TODO if variable assignment, execute the assignment and return
         * the exit status */
	// daca avem de a face cu o atribuire de variabila identificata prin prezenta
	// '=' in partea urmatoare a comenzii
	if(s->verb->next_part != NULL && 
	   strcmp(s->verb->next_part->string, "=") == 0 &&
	   s->verb->next_part->next_part != NULL) {
		// se executa adaugarea variabilei si se returneaza valoarea functiei
		ret = setenv(s->verb->string, s->verb->next_part->next_part->string, 1);
		return ret;
	}
	/* TODO if external command:
         *   1. fork new process
		 *     2c. perform redirections in child
         *     3c. load executable in child
         *   2. wait for child
         *   3. return exit status
	 */
	// se creaza un nou proces	
	pid = fork();
    // daca este procesul copil
	if(pid == 0) {
		// se verifica cazul un redirectari
		// cazul unei redirectari a intrarii
		if(s->in != NULL) {
			// deschidem fisierul respectiv in modul corespunzator
			fd = open(get_word(s->in), O_RDONLY);
			// duplicam file-descriptorul, redirectand intrarea standard
			dup2(fd, STDIN_FILENO);
			close(fd);
		}
		// cazul unei redirectari a iesirii standard si a iesirii erorii
		// in acelasi file-descriptor
		if(s->out != NULL && s->err != NULL && 
		   strcmp(get_word(s->out), get_word(s->err)) == 0) {
			// deschidem fisierul respectiv in modul corespunzator
			if(s->io_flags == IO_OUT_APPEND)
				fd = open(get_word(s->out), O_CREAT | O_WRONLY | O_APPEND, 0644);
			else
				fd = open(get_word(s->out), O_CREAT | O_WRONLY | O_TRUNC, 0644);
			// duplicam file-descriptorul, redirectand iesirea standard si
			// iesirea erorilor
			dup2(fd, STDOUT_FILENO);
			dup2(fd, STDERR_FILENO);
			close(fd);
		}
		// cazul unei redirectari a iesirii standard sau a iesirii erorii
		else {
			if(s->out != NULL) {
				// deschidem fisierul respectiv in modul corespunzator
				if(s->io_flags == IO_OUT_APPEND)
					fd = open(get_word(s->out), O_CREAT | O_WRONLY | O_APPEND, 0644);
				else
					fd = open(get_word(s->out), O_CREAT | O_WRONLY | O_TRUNC, 0644);
				// duplicam file-descriptorul, redirectand iesirea standard
				dup2(fd, STDOUT_FILENO);
				close(fd);
		    }
			if(s->err != NULL) {
				// deschidem fisierul respectiv in modul corespunzator
				if(s->io_flags == IO_ERR_APPEND)
					fd = open(get_word(s->err), O_CREAT | O_WRONLY | O_APPEND, 0644);
				else
					fd = open(get_word(s->err), O_CREAT | O_WRONLY | O_TRUNC, 0644);
				// duplicam file-descriptorul, redirectand iesirea erorii
				dup2(fd, STDERR_FILENO);
				close(fd);
		    }
		}
		// obtinem o concatenare a parametrilor comenzii 
		argv = get_argv(s, &size);
		// executam comanda cu concatenarea obtinuta a parametrilor
		ret = execvp(argv[0], argv);
		// tratam cazul unei esuari a executiei comenzii
		if (ret == -1) {
			fprintf(stderr, "Execution failed for '%s'\n", argv[0]);
			exit(0);
		}
		exit(1);
	}
	// se asteapta terminarea executiei procesului
	waitpid(pid, &statusPtr, 0);

	ret = -1;
	// se evalueaza statusul terminarii executiei procesului
	if(WIFEXITED(statusPtr))
		ret = WEXITSTATUS(statusPtr);
	if(WIFSIGNALED(statusPtr))
		ret = WTERMSIG(statusPtr);
	if(WIFSTOPPED(statusPtr))
		ret = WSTOPSIG(statusPtr);
	if(ret != -1)
		return ret;

	return 0; /* TODO replace with actual exit status */
}

/**
 * Process two commands in parallel, by creating two children.
 */
static bool do_in_parallel(command_t *cmd1, command_t *cmd2, int level, command_t *father)
{
	/* TODO execute cmd1 and cmd2 simultaneously */
	int ret, statusPtr;
	pid_t pid;
	// se creaza un nou proces	
	pid = fork();
	// daca este procesul copil
	if(pid == 0)
		// se parseaza una din comenzi
		ret = parse_command(cmd1, level+1, father);
	else // pentru un proces parinte
		// se parseaza cealalta comanda
		ret = parse_command(cmd2, level+1, father);
	// se asteapta terminarea executiei procesului
	waitpid(pid, &statusPtr, 0);

	if(ret)
		return true; /* TODO replace with actual exit status */
	return false;
}

/**
 * Run commands by creating an anonymous pipe (cmd1 | cmd2)
 */
static bool do_on_pipe(command_t *cmd1, command_t *cmd2, int level, command_t *father)
{
	/* TODO redirect the output of cmd1 to the input of cmd2 */
	int ret = 0, fd[2], statusPtr, statusPtr1;
	pid_t pid, pid1;
	
	pid = fork(); // se creaza un nou proces
	pipe(fd);	// se creaza un pipeline de vectorul ce contine cei 2 file-descriptori

	if(pid == 0) { 	// daca este procesul copil
			
		pid1 = fork();	// se creaza un nou proces

		if(pid1 == 0) { // daca este procesul copil
			// se inchide celalalt file-descriptor
			close(fd[0]);
			// duplicam file-descriptorul, redirectand iesirea standard
			dup2(fd[1], STDOUT_FILENO);
			// se parseaza una din comenzi si se retine valoarea intoarsa
			ret = parse_command(cmd1, level+1, father);
		} 
		else {	// daca este procesul parinte
			// se inchide celalalt file-descriptor
			close(fd[1]);
			// duplicam file-descriptorul, redirectand intrarea standard
			dup2(fd[0], STDIN_FILENO);
			// se parseaza cealalta comanda si se retine valoarea intoarsa
			ret = parse_command(cmd2, level+1, father);
		}
		// se asteapta terminarea executiei procesului
		waitpid(pid1, &statusPtr1, 0);
		// se inchid file-descriptorii
		close(fd[0]);
		close(fd[1]);

		exit(0);
	}
	// se asteapta terminarea executiei procesului
	waitpid(pid, &statusPtr, 0);

	if(ret)
		return true; /* TODO replace with actual exit status */
	return false;
}

/**
 * Parse and execute a command.
 */
int parse_command(command_t *c, int level, command_t *father)
{
	int ret, ret1;
	bool retb;

	/* TODO sanity checks */
	if(c == NULL) {
		fprintf(stderr, "Nu trebuie o comanda nula\n");
		return 0;
	}
	if (c->op == OP_NONE) {
		/* TODO execute a simple command */
		ret = parse_simple(c->scmd, level, father);
		return ret; /* TODO replace with actual exit code of command */
	}
	switch (c->op) {
		case OP_SEQUENTIAL: {
			/* TODO execute the commands one after the other */
			parse_command(c->cmd1, level+1, c);
			ret = parse_command(c->cmd2, level+1, c);
			break;
		}
		case OP_PARALLEL: {
			/* TODO execute the commands simultaneously */
			retb = do_in_parallel(c->cmd1, c->cmd2, level, c);
			if(retb == true)
				ret = 1;
			else
				ret = 0;
			break;
		}
		case OP_CONDITIONAL_NZERO: {
			/* TODO execute the second command only if the first one
		             * returns non zero */
			ret1 = parse_command(c->cmd1, level+1, c);
			if(ret1)
				ret = parse_command(c->cmd2, level+1, c);
			break;
		}
		case OP_CONDITIONAL_ZERO: {
			/* TODO execute the second command only if the first one
		             * returns zero */
			ret1 = parse_command(c->cmd1, level+1, c);
			if(!ret1)
				ret = parse_command(c->cmd2, level+1, c);
			break;
		}
		case OP_PIPE: {
			/* TODO redirect the output of the first command to the
			 * input of the second */
			retb = do_on_pipe(c->cmd1, c->cmd2, level, c);
			if(retb == true)
				ret = 1;
			else
				ret = 0;
			break;
		}
		default:
			assert(false);
	}

	return ret; /* TODO replace with actual exit code of command */
}

/**
 * Readline from mini-shell.
 */
char *read_line()
{
	char *instr;
	char *chunk;
	char *ret;

	int instr_length;
	int chunk_length;

	int endline = 0;

	instr = NULL;
	instr_length = 0;

	chunk = calloc(CHUNK_SIZE, sizeof(char));
	if (chunk == NULL) {
		fprintf(stderr, ERR_ALLOCATION);
		return instr;
	}

	while (!endline) {
		ret = fgets(chunk, CHUNK_SIZE, stdin);
		if (ret == NULL) {
			break;
		}

		chunk_length = strlen(chunk);
		if (chunk[chunk_length - 1] == '\n') {
			chunk[chunk_length - 1] = 0;
			endline = 1;
		}

		ret = instr;
		instr = realloc(instr, instr_length + CHUNK_SIZE + 1);
		if (instr == NULL) {
			free(ret);
			return instr;
		}
		memset(instr + instr_length, 0, CHUNK_SIZE);
		strcat(instr, chunk);
		instr_length += chunk_length;
	}

	free(chunk);

	return instr;
}

