void execute_pipeline(t_minishell *shell, t_cmd *cmd)
{
	int prev_pipe_read;

	prev_pipe_read = -1;
	while (cmd)
	{
		// 1. If cmd->next, call pipe()

		// 2. Fork()

		// 3. In Child:
		//    a. Wire pipes (using prev_pipe_read and current pipe)
		//    b. Process Redirections (Loop through cmd->redirs)
		//    c. Check for Builtins
		//    d. Execve(cmd->args)

		// 4. In Parent:
		//    a. Close unused pipe ends
		//    b. Save current pipe read end for next command

		cmd = cmd->next;
	}
	// 5. Wait for all children
}
