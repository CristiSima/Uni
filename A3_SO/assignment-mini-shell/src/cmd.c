// SPDX-License-Identifier: BSD-3-Clause

#include <sys/types.h>
#include <sys/stat.h>
#include <sys/wait.h>

#include <linux/limits.h>

#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

#include "cmd.h"
#include "utils.h"

#define READ		0
#define WRITE		1



/**
 * Internal change-directory command.
 */
static bool shell_cd(char *dir)
{
	if (dir == NULL)
		return 1;

	int ret = chdir(dir);

	free(dir);
	if (ret == 0)
		return 0;

	printf("no such file or directory\n");

	return 1;
}

/**
 * Parse a simple command (internal, environment variable assignment,
 * external command).
 */
static int parse_simple(simple_command_t *s, int level, command_t *father)
{
	int ret;

	/* Sanity checks. */
	if (s == NULL)
		return 0;

	char *command = get_word(s->verb);

	/* If builtin command, execute the command. */
	// TODO: redirect for internal commands
	if (!strcmp(command, "cd")) {
		if (s->out) {
			char *out_file = get_word(s->out);
			int is_append = (s->io_flags & IO_OUT_APPEND) ? O_APPEND : O_TRUNC;
			int new_fd = open(out_file, O_WRONLY|O_CREAT|is_append, 0664);

			free(out_file);
			close(new_fd);
		}
		ret = shell_cd(get_word(s->params));
		free(command);
		return ret;
	}
	if (!strcmp(command, "exit") || !strcmp(command, "quit")) {
		free(command);
		return SHELL_EXIT;
	}

	/* If variable assignment, execute the assignment and return
	 * the exit status.
	 */
	if (strchr(command, '=')) {
		char *new_val = strchr(command, '=');

		// split on the first '='
		new_val[0] = 0;
		new_val++;

		setenv(command, new_val, 1);
		return 0;
	}

	pid_t forkr = fork();

	if (forkr == 0) {
		int argc;
		char **argv = get_argv(s, &argc);

		if (s->in) {
			char *in_file = get_word(s->in);
			int new_fd = open(in_file, O_RDONLY);

			free(in_file);
			dup2(new_fd, 0);
			close(new_fd);
		}

		if (s->out) {
			char *out_file = get_word(s->out);
			int is_append = (s->io_flags & IO_OUT_APPEND) ? O_APPEND : O_TRUNC;
			int new_fd = open(out_file, O_WRONLY|O_CREAT|is_append, 0664);

			free(out_file);
			dup2(new_fd, 1);
			close(new_fd);
		}

		if (s->err && s->out == s->err) {
			dup2(1, 2);
		} else if (s->err) {
			char *err_file = get_word(s->err);
			int is_append = (s->io_flags & IO_ERR_APPEND) ? O_APPEND : O_TRUNC;
			int new_fd = open(err_file, O_WRONLY|O_CREAT|is_append, 0664);

			free(err_file);
			dup2(new_fd, 2);
			close(new_fd);
		}

		execvp(command, argv);
		printf("Execution failed for '%s'\n", command);
		exit(127);
	}

	free(command);

	int status;

	waitpid(forkr, &status, 0);

	return status;
}

/**
 * Process two commands in parallel, by creating two children.
 */
static bool run_in_parallel(command_t *cmd1, command_t *cmd2, int level,
		command_t *father)
{
	pid_t forkr = fork();

	if (forkr == 0) {
		int ret = parse_command(cmd1, level+1, father);

		if (ret != 0 && (ret & 0xFF) == 0)
			ret = 1;
		exit(ret);
	}

	int cmd1_ret;
	int cmd2_ret = parse_command(cmd2, level+1, father);

	waitpid(forkr, &cmd1_ret, 0);

	return cmd1_ret | cmd2_ret;
}

/**
 * Run commands by creating an anonymous pipe (cmd1 | cmd2).
 */
static bool run_on_pipe(command_t *cmd1, command_t *cmd2, int level,
		command_t *father)
{
	int pipefd[2];

	pipe(pipefd);
	pid_t forkr1 = fork();

	if (forkr1 == 0) {
		close(pipefd[READ]);
		dup2(pipefd[WRITE], WRITE);
		close(pipefd[WRITE]);
		int ret = parse_command(cmd1, level+1, father);

		if (ret != 0 && (ret & 0xFF) == 0)
			ret = 1;

		exit(ret);
	}
	close(pipefd[WRITE]);

	pid_t forkr2 = fork();

	if (forkr2 == 0) {
		dup2(pipefd[READ], READ);
		close(pipefd[READ]);
		int ret = parse_command(cmd2, level+1, father);

		if (ret != 0 && (ret & 0xFF) == 0)
			ret = 1;

		exit(ret);
	}
	close(pipefd[READ]);

	int cmd1_ret, cmd2_ret;

	waitpid(forkr2, &cmd2_ret, 0);
	waitpid(forkr1, &cmd1_ret, 0);

	return cmd2_ret;
}

/**
 * Parse and execute a command.
 */
int parse_command(command_t *c, int level, command_t *father)
{
	int ret;
	/* sanity checks */
	if (c == NULL)
		return 0;

	if (c->op == OP_NONE)
		return parse_simple(c->scmd, level, father);

	switch (c->op) {
	case OP_SEQUENTIAL:
		parse_command(c->cmd1, level+1, c);
		return parse_command(c->cmd2, level+1, c);

	case OP_PARALLEL:
		return run_in_parallel(c->cmd1, c->cmd2, level, c);

	case OP_CONDITIONAL_NZERO:
		// Execute the second command only if the first one returns non zero.
		ret = parse_command(c->cmd1, level+1, c);
		if (ret != 0)
			return parse_command(c->cmd2, level+1, c);

		return ret;

	case OP_CONDITIONAL_ZERO:
		// Execute the second command only if the first one returns zero.
		ret = parse_command(c->cmd1, level+1, c);
		if (ret == 0)
			return parse_command(c->cmd2, level+1, c);

		return ret;

	case OP_PIPE:
		return run_on_pipe(c->cmd1, c->cmd2, level, c);

	default:
		return SHELL_EXIT;
	}
}
