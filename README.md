#  Mini Shell – Linux Internals Project

## 📌 Project Overview

The Mini Shell is a custom Linux command-line interpreter developed in C. It mimics core functionalities of a Unix shell by allowing users to execute system commands, manage processes, and use built-in utilities.

The project demonstrates Linux internals concepts such as process creation, environment variable handling, and job control mechanisms.

---

## ⚙️ System Working Principle

1. User enters a command in the shell prompt.
2. Input is parsed and analyzed.
3. Built-in commands are executed internally.
4. External commands are executed using fork and exec system calls.
5. Parent process monitors child execution.

---

## 🚀 Features

* Execute Linux system commands
* Built-in command support
* Environment variable handling
* Exit status tracking
* Shell process identification
* Job control (foreground/background)
* Continuous interactive prompt

---

## 🛠️ Built-in Commands Supported

### Directory Commands

* `pwd` – Display present working directory
* `cd <dir>` – Change current directory

---

### Shell Control

* `exit` – Terminate the shell

---

### Environment & Process Info

* `echo $?` – Display last child exit status
* `echo $$` – Display shell process ID
* `echo $SHELL` – Display shell environment path

Example:

```
MiniShell$ echo $$
The mini-shell process id is 2451

MiniShell$ echo $?
child exit status = 0

MiniShell$ echo $SHELL
/bin/bash
```

---

### Job Control Commands

* `jobs` – List background jobs
* `fg` – Bring job to foreground
* `bg` – Resume job in background

---

## 🖥️ Sample Execution

```
MiniShell$ ls
main.c  shell.c  Makefile

MiniShell$ pwd
/home/user/mini-shell

MiniShell$ echo $$
The mini-shell process id is 2451
```


## 🧰 System Calls Used

* `fork()` – Process creation
* `execvp()` – Command execution
* `wait()` – Process synchronization
* `getenv()` – Environment variables
* `chdir()` – Directory change
* `getcwd()` – Current directory

---

## 💻 Technologies Used

* C Programming
* Linux System Calls
* Process Management
* Job Control
* Makefile Build System

---

## 📂 Project Structure

```
Mini_Shell/
│
├── main.c
├── shell.c
├── shell.h
├── Makefile
├── README.md
```

---

## ▶️ Build Instructions

```
make
./minishell
```

## 📚 Learning Outcomes

* Linux process lifecycle
* System call usage
* Job control implementation
* Environment variable handling
* Shell architecture design

---

## 👨‍💻 Author

**Roshan Jameer**
GitHub: https://github.com/Roshan12072000
