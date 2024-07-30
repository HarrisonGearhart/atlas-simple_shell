# Simple Shell

## Description
Simple Shell is a custom Unix shell implementaion. It aims to replicate the functionality of a traditional Unix shell, allowing users to execute commands and manage processes.

## Table of Contents
1. [Installation](#installation)
2. [Usage](#usage)
3. [Features](#features)
4. [Flowchart](#flowchart)
5. [Authors](#authors)
6. [Acknowledgments](#acknowledgments)

## Installation
To install and run Simple Shell, follow these steps:

1. **Clone the repository:**
```sh
git clone https://github.com/HarrisonGearhart/simple_shell.git
```
2. **Navigate to the project directory:**
```sh
cd simple_shell
```
3. **Compile the shell:**
```sh
gcc -Wall -Werror -Wextra -pedantic -std=gnu89 *.c -o hsh
```

## Usage
To use Simple Shell:

1. **Run the shell:**
```sh
./hsh
```
2. **Use commands as you would in a traditional Unix shell:**
```sh
($) /bin/ls
```

## Features
- Display a prompt and wait for user input.<br>
- Execute commands entered by the user.<br>
- Handle command lines with arguments.<br>
- Implement built-in commands like 'exit' and 'env'.<br>
- Use the PATH environment variable to locate executables.<br>
- Error handling and end-of-file (Ctrl+D) condition.

## Flowchart
```mermaid
flowchart TB
    A[Read Line] --> B[Parse Line]
    B --> C[Execute]
    C --> A
```
```mermaid
flowchart TB
	A[Read Line] -- Fail --> B(exit w/ error message)
	A -- Success --> C(return read line)
	C-->D[Parse Line]
```
```mermaid
flowchart TB
    A[Parse Line]
    B(Check memeroy allocation)
    C(print error message)
    D(tokenize using delimiters)
    E(iterate thru tokens)
    F(Continue until NULL)
    G(If buf size if exceeded)
    H(reallocate memory)
    I(exit/memory allocation error)
    J(return array)
    K[Execute]
    A-->B--error-->C
    B--success-->D-->E-->F-->J-->K
    E-->G-->H-->F
    H--no token?-->I
```
```mermaid
flowchart TB
A[Execute]
B(return 1)
C(Check for exit command)
D(return 0)
E(Check for ENV)
F(Print current ENV variables, return 1)
G(Check if command is in current directory)
H(return 1 & execute)
I(find in system path)
J(print error)
K[Call fork to create new process]
A--empty-->B-->C
A--not empty-->C
C--Yes-->D
C--None-->E-->F-->G--exists-->H-->K
G--doesn't-->I--exists-->H
I--doesn't-->J
```
```mermaid
flowchart TB
A[New Process]
B(return -1 & print error)
C(return 0 & execute child process)
D(Return PID of child process)
E[parent process]
F(Child Process terminates)
G(Wifexited returns true)
H(Wifsignal returns true)
I{Infinite loop until a condition is met}
A--error-->B
A--success-->C-->D-->E-->F--Normal termination-->G-->I
F--Termination by signal-->H-->I
```

## Authors
- **Ari Williams** - [AriRainbow](https://github.com/AriRainbow)<br>
- **Harrison Gearhart** - [HarrisonGearhart](https://github.com/HarrisonGearhart)

## Acknowledgments
- Inspired by the Unix shell.<br>
- Thanks to the open-source community for their valuable resources.
