*This project has been created as part of the 42 curriculum by mariacos and andcardo.*

---

# minishell

## Description

**minishell** is a simple Unix shell written in C built as part of the 42 school curriculum. The goal is to reproduce the core behavior of **bash** giving us a deep understanding of how a shell actually works under the hood. From reading user input, to parsing commands, to executing processes.

The shell supports:

- An interactive prompt that displays while waiting for a new command
- Command history (navigate with arrow keys)
- Execution of commands using the `PATH` environment variable, or with relative/absolute paths
- Single quotes `'` : prevent all interpretation of the enclosed characters.
- Double quotes `"` : prevent interpretation except for `$` (variable expansion).
- **Redirections:**
  - `<` : redirect input.
  - `>` : redirect output.
  - `>>` : append output.
  - `<<` : heredoc (read input until a delimiter line is found).
- **Pipes** `|` : connect the output of one command to the input of the next (we used the knowledge we gained by doing the project Pipex in the Milestone 2).
- **Environment variable expansion** with `$VAR` and `$?` (last exit status).
- **Signal handling:** `Ctrl+C`, `Ctrl+D`, `Ctrl+\` behave like in bash.
- **Built-in commands** implemented from scratch:

| Command | Description |
|---------|-------------|
| `echo` | Print text (`-n` flag supported) |
| `cd` | Change directory |
| `pwd` | Print current working directory |
| `export` | Set or display environment variables |
| `unset` | Remove environment variables |
| `env` | Print all environment variables |
| `exit` | Exit the shell with an optional exit code |

### Overview of how it works

```
Input (readline)
     │
     ▼
  Lexer: splits the input into tokens (words, pipes, redirections...) and identify syntax errors
     │
     ▼
  Parser: builds a list of command structures from the tokens
     │
     ▼
  Expansion: expands $VAR, $?, and handles quotes
     │
     ▼
  Execution: runs builtins directly, forks/execs external commands, handles pipes and redirections
```

---

## Instructions

### Requirements

- GCC compiler
- GNU `make`
- `readline` library

On Debian/Ubuntu, install readline with:
```bash
sudo apt-get install libreadline-dev
```

### Compilation

This will compile the project and produce the `minishell` executable.

Other make targets:

| Target | Description |
|--------|-------------|
| `make` | Build the project |
| `make clean` | Remove object files |
| `make fclean` | Remove object files and the executable |
| `make re` | Full recompile from scratch |
| `make valgrind` | Run with valgrind (leak check, suppression for readline) |

### Running

```bash
./minishell
```

The shell does not accept arguments. Once running you will see a prompt where you can type commands just like in bash.

### Usage examples

```bash
# Basic command
$ ls -la

# Pipe
$ ls | grep .c | wc -l

# Redirections
$ echo "hello" > file.txt
$ cat < file.txt >> output.txt

# Heredoc
$ cat << EOF
> some text
> EOF

# Variable expansion
$ export NAME=world
$ echo "Hello $NAME"
Hello world

# Exit status
$ ls nonexistent
$ echo $?
2

# Built-ins
$ cd ..
$ pwd
$ export MY_VAR=42
$ unset MY_VAR
$ env
$ exit 0
```

---

## Resources

### Documentation
- [Bash Manual](https://www.gnu.org/software/bash/manual/bash.html)
- [POSIX Shell Command Language](https://pubs.opengroup.org/onlinepubs/009695399/utilities/xcu_chap02.html)
- [Bash Metacharacters](https://www.angelfire.com/mi/genastorhotz/reality/computers/linux/bashmetachars.html)

### Guides & Books
- [Crafting Interpreters Representing Code](https://craftinginterpreters.com/representing-code.html)
- [Writing Your Own Shell (Purdue)](https://www.cs.purdue.edu/homes/grr/SystemsProgrammingBook/Book/Chapter5-WritingYourOwnShell.pdf)
- [Minishell: Building a Mini Bash (Medium)](https://m4nnb3ll.medium.com/minishell-building-a-mini-bash-a-42-project-b55a10598218)

### 42 Resources
- [42docs – Minishell](https://harm-smits.github.io/42docs/projects/minishell)
- [Mastering 42 – Minishell](https://haglobah.github.io/Mastering-42/holy_graph/minishell.html)
- [Minishell Resource List (GitHub)](https://github.com/ulsgks/minishell/blob/main/docs/resources.md)

### Videos
- [Computerphile: Parsing](https://www.youtube.com/watch?v=r6vNthpQtSI)
- [Shell Implementation Walkthrough](https://www.youtube.com/watch?v=T0BO415l3N0)


### AI usage

AI was used during this project as a learning resource to better understand bash behavior, clarify concepts, and explore how things work under the hood.

---

*mariacos | andcardo | 42 Lisboa*
