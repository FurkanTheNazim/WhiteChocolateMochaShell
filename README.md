*This project has been created as part of the 42 curriculum by mahmmous, kedemiro.*

# 🐚 WhiteChocolateMochaShell — Minishell

A fully functional UNIX shell written in C, replicating core behaviors of **bash**. This project explores process creation, file descriptor management, signal handling, and command-line parsing — all while adhering to the strict 42 Norm coding standard.

---

## Description

**WhiteChocolateMochaShell (WCMS)** is a simplified yet robust recreation of a UNIX shell, built from scratch in C. The goal of the project is to deeply understand how a shell works under the hood: from reading user input with `readline`, tokenizing and parsing it into an Abstract Syntax Tree (AST), expanding environment variables, to forking child processes and managing pipes and redirections.

### Architecture Overview

The shell follows a classic **pipeline architecture**:

```
Input → Lexer → Validator → Heredoc → Expander → Parser → AST Builder → Executor
```

| Module               | Responsibility                                                       |
| -------------------- | -------------------------------------------------------------------- |
| **Lexer**            | Tokenizes raw input into a linked list of tokens (words, operators)  |
| **Validator**        | Checks for syntax errors before further processing                   |
| **Heredoc**          | Handles `<<` delimiter-based input with variable expansion           |
| **Expander**         | Resolves `$VAR`, `$?`, and handles quote removal                     |
| **Parser**           | Converts token list into a linked list of commands with redirections |
| **AST Builder**      | Constructs a binary tree of pipe and command nodes                   |
| **Executor**         | Walks the AST, sets up pipes/redirections, and forks processes       |
| **Garbage Collector** | Custom memory manager with checkpoint/rollback for leak-free operation |

---

## Features

- **Interactive prompt** with `readline` and command history
- **Pipes** (`|`) — chain multiple commands together
- **Redirections** — input (`<`), output (`>`), append (`>>`), and heredoc (`<<`)
- **Environment variable expansion** — `$VAR`, `$?` (last exit status)
- **Quote handling** — single (`'`) and double (`"`) quotes with proper semantics
- **Signal handling** — `Ctrl+C`, `Ctrl+D`, `Ctrl+\` behave like bash
- **Built-in commands**:
  - `echo` (with `-n` flag)
  - `cd` (with relative, absolute, and home paths)
  - `pwd`
  - `export`
  - `unset`
  - `env`
  - `exit`
- **Variable assignments** — `VAR=value` syntax
- **Custom garbage collector** — checkpoint/rollback memory management to prevent leaks

---

## Instructions

### Prerequisites

- A Linux or macOS system
- `cc` (GCC or Clang)
- `make`
- `readline` library installed

On Debian/Ubuntu, install `readline` with:

```bash
sudo apt-get install libreadline-dev
```

On macOS (with Homebrew):

```bash
brew install readline
```

### Compilation

Clone the repository and build the project:

```bash
git clone https://github.com/FurkanTheNazim/WhiteChocolateMochaShell.git
cd WhiteChocolateMochaShell
make
```

The `Makefile` supports the following targets:

| Target    | Description                            |
| --------- | -------------------------------------- |
| `make`    | Build the `minishell` binary           |
| `make clean` | Remove object files                 |
| `make fclean` | Remove object files and the binary |
| `make re` | Full rebuild (clean + build)           |

### Execution

```bash
./minishell
```

The shell accepts no arguments — simply run the binary and start typing commands.

### Usage Examples

```bash
# Simple command
ls -la

# Pipes
cat Makefile | grep SRC | wc -l

# Redirections
echo "hello world" > output.txt
cat < output.txt

# Heredoc
cat << EOF
This is a heredoc
with multiple lines
EOF

# Environment variables
export MY_VAR="42 Istanbul"
echo $MY_VAR

# Variable assignment
GREETING=hello
echo $GREETING

# Exit status
ls nonexistent_file
echo $?

# Built-in commands
cd /tmp && pwd
env | grep PATH
exit 42
```

---

## Resources

### References & Documentation

- [GNU Bash Reference Manual](https://www.gnu.org/software/bash/manual/bash.html) — the authoritative reference for bash behavior and POSIX shell semantics
- [The Open Group Base Specifications — Shell Command Language](https://pubs.opengroup.org/onlinepubs/9699919799/utilities/V3_chap02.html) — POSIX specification for shell syntax and execution
- [*Writing Your Own Shell* — Stephen Brennan](https://brennan.io/2015/01/16/write-a-shell-in-c/) — introductory tutorial on building a shell in C
- [Minishell — 42 Docs](https://harm-smits.github.io/42docs/projects/minishell) — community-maintained project guide
- [`readline(3)` man page](https://man7.org/linux/man-pages/man3/readline.3.html) — documentation for the GNU Readline library
- [Pipes, Forks, & Dups — explained](https://www.rozmichelle.com/pipes-forks-dups/) — visual guide to pipes and process management in C

### AI Usage Disclosure

AI-assisted tools (GitHub Copilot, ChatGPT, and Claude) were used during the development of this project for the following purposes:

- **Debugging assistance** — diagnosing signal handling edge cases and heredoc behavior
- **Code review** — identifying memory leaks and Norminette compliance issues
- **Documentation** — generating and refining this README and in-code comments
- **Conceptual explanations** — understanding AST construction patterns and POSIX shell semantics

All code was written, reviewed, and understood by the authors. AI was used as a supplementary tool, not as a primary code generator.

---

## Project Structure

```
WhiteChocolateMochaShell/
├── Makefile
├── README.md
├── includes/
│   ├── WCMS.h                 # Main header — core shell struct
│   ├── lexer_parser.h         # Token, command, and AST types
│   ├── executor.h             # Executor function prototypes
│   ├── builtin.h              # Built-in command declarations
│   ├── env.h                  # Environment variable management
│   ├── expander.h             # Variable expansion logic
│   ├── gc.h                   # Garbage collector interface
│   ├── heredoc.h              # Heredoc handling
│   ├── sh_signals.h           # Signal handler setup
│   ├── errors.h               # Error reporting utilities
│   └── libft/                 # Custom C standard library
└── src/
    ├── main.c                 # Entry point and main loop
    ├── main_inputs.c          # Input reading with readline
    ├── lexer/                 # Tokenizer (4 files)
    ├── parser/                # Parser and AST builder (5 files)
    ├── expander/              # Variable and quote expansion (4 files)
    ├── executor/              # Process execution and piping (5 files)
    ├── builtins/              # Built-in command implementations (14 files)
    ├── heredoc/               # Heredoc processing (3 files)
    ├── enviroment/            # Environment variable storage (2 files)
    ├── signals/               # Signal handler configuration (1 file)
    ├── garbage_collector/     # Custom memory management (2 files)
    └── errors/                # Error message formatting (1 file)
```

---

## Authors

- **mahmmous** — [42 Istanbul](https://42istanbul.com.tr/)
- **kedemiro** — [42 Istanbul](https://42istanbul.com.tr/)
