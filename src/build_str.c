 #include "minishell.h"

/* 

I. Parsing: From Tokens to Linked List
Transform tokens into a list of command nodes (`t_cmd`)
* Create Node: Start a new `t_cmd` at the beginning and every time a `T_PIPE` is found.
* Classify:
   * `T_WORD` → Add to `char **args`.
   * Redirection → Create a `t_redir` node with the type and filename.
* Finish: When a `T_PIPE` or the end of tokens is reached, NULL-terminate the `args` array.


II. Expansion (Environment Variables)
Search for the `$` symbol before Word Splitting.
* Where: Check all `args` and `t_redir->file` names.
* Quote Rules:
   * Single (`' '`): Do nothing (no expansion).
   * Double (`" "`): Expand the variable (e.g., `"$USER"` → `"juan"`).
   * No Quotes: Expand the variable.
* Special Case: Replace `$?` with the exit status of the last command.


III. Word Splitting
Break down expanded variables that were not inside double quotes.
* Why: If `VAR="ls -l"`, then `$VAR` must become two separate arguments (`ls` and `-l`).
* The Rule:
   * Inside `""`: Keep as one single string (no splitting).
   * Outside `""`: Split by spaces into separate arguments.


*/