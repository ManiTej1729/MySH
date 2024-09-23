# MySH

## How to use

Step 1: Clone the repository

Step 2: execute the command ```cd mini-project-1-ManiTej1729```

Step 3: run the command ```make```

## File Structure

    ~root: directory

        |__main

        |__spec1

            |__spec1.c
            
            |__spec1.h

        |__spec2

            |__spec2.c
            
            |__spec2.h

        |__spec3
            
            |__spec3.c
            
            |__spec3.h

        |__spec4
            
            |__spec4.c
            
            |__spec4.h

        |__spec5
            
            |__spec5.c
            
            |__spec5.h

        |__spec6
            
            |__spec6.c
            
            |__spec6.h

        |__spec7

            |__spec7.c
            
            |__spec7.h

        |__spec8
            
            |__spec8.c
            
            |__spec8.h

        |__spec9
            
            |__spec9.c
            
            |__spec9.h

        |__spec10
            
            |__spec10.c
            
            |__spec10.h

        |__spec11
            
            |__spec11.c
            
            |__spec11.h

        |__spec12
            
            |__spec12.c
            
            |__spec12.h

        |__spec13
            
            |__spec13.c
            
            |__spec13.h

        |__spec14
            
            |__spec14.c
            
            |__spec14.h

        |__spec16

            |__spec16.c
            
            |__spec16.h

        |__spec17
            
            |__spec17.c
            
            |__spec17.h
        
        |___README.md

        |___Makefile

## Description

### Specification 1 (Display Requirements)

It displays username, systemname, and current working directory

```sh
<username@systemname:cwd>
```

### Specification 2 (Input Requirements)

This will avain the functionality for using '&' and ';' whose functionality is similar to that of bash. But it can execute only the system commands and not the ones that are created by me in this project.

### Specification 3 (hop)

#### Syntax

```hop <flags> <path/name>```

- This command is like cd in bash
- It supports 4 flags: '.', '..', '~', '-'
- But my implementation is a bit different from what is mentioned in the project documentation.
- It doesn't execute the commands sequentially. Instead it checks whether the path exists or not.

```sh
<username@systemname:cwd> hop main/non_existent_directory
Error: main/non_existent_directory directory doesnot exist on your computer
<username@systemname:cwd>
```

- In my implementation, even though the main directory exists, it doesn't hop to the directory.
- for multiple arguments for hop, it only prints the final path.

Few examples:
```sh
hop main
hop ..
hop .
hop ~
hop -
hop main .. spec1 .. spec2
```

### Specification 4 (reveal)

#### Syntax

```reveal <flags> <path/name>```

- This command is like ls in bash
- It has 2 flags a & l whose functionality is similar to that of bash

Example:
```sh
<username@systemname:cwd> reveal -l .
Number of entitites: 19
-rw-r--r-- 1 manitej.sriram staff 312 Aug 31 16:13 Makefile 
-rw-r--r-- 1 manitej.sriram staff 2537 Aug 31 20:31 README.md 
-rwxr-xr-x 1 manitej.sriram staff 70776 Aug 31 20:27 a.out 
drwxr-xr-x 3 manitej.sriram staff 96 Aug 30 04:44 a.out.dSYM 
-rwxr--r-- 1 manitej.sriram staff 914 Aug 28 16:58 intro.sh 
-rw-r--r-- 1 manitej.sriram staff 530 Aug 23 08:56 intro.txt 
drwxr-xr-x 5 manitej.sriram staff 160 Aug 29 21:29 main 
-rwxr-xr-x 1 manitej.sriram staff 99 Aug 28 16:59 outro.sh 
-rw-r--r-- 1 manitej.sriram staff 316 Aug 28 17:00 outro.txt 
drwxr-xr-x 4 manitej.sriram staff 128 Aug 29 21:35 spec1 
drwxr-xr-x 4 manitej.sriram staff 128 Aug 31 01:08 spec2 
drwxr-xr-x 4 manitej.sriram staff 128 Aug 29 22:37 spec3 
drwxr-xr-x 4 manitej.sriram staff 128 Aug 31 01:10 spec4 
drwxr-xr-x 5 manitej.sriram staff 160 Aug 30 19:12 spec5 
drwxr-xr-x 4 manitej.sriram staff 128 Aug 31 01:10 spec6 
drwxr-xr-x 4 manitej.sriram staff 128 Aug 30 15:04 spec7 
drwxr-xr-x 4 manitej.sriram staff 128 Aug 31 01:10 spec8 
-rw-r--r-- 1 manitej.sriram staff 1388 Aug 31 18:25 test.c 
drwxr-xr-x 4 manitej.sriram staff 128 Aug 30 15:06 utils
<username@systemname:cwd>
```

#### My coloring scheme

1. Blue for directories
2. Green for executables
3. Magenta for normal files

### Specification 5 (log commands)

Whenever the user types something onto the console, that response will be recorded and stored in a file named history.txt. But it can only store latest 15 commands and discard the oldest

This specification has 3 commands:

#### 1. log

This will print all the commands present in the history

#### 2. log purge

This will empty the history.txt file

#### 3. log execute \<index>

You can use this command to execute last 15 commands (at maximum) without needing to actually type the command

Example:
If the state of history.txt is this:
```bash
hop ..  # Oldest
hop main
hop spec1
reveal main
reveal -l .
reveal -a -l main  # Latest
```

Then on executing the command ```log execute 3``` will execute the 3rd latest command you've executed. (Which is reveal main in this example)

### Specification 6 (System commands)

### Specification 7 (proclore)

#### Syntax

```proclore <pid>```

This will give us some metadata about the process with the help of PID.

It includes

1. Process ID
2. Process group ID
3. Process status
4. Virtual memory usage
5. Executable's path

But, I'm having a problem with MACOS because it doesn't have filesystem like linux does. Although I've wrote a code which might work for linux platforms.

### Specification 8 (seek)

#### Syntax

```seek <flags> <target_prefix> <location>```

### My coloring scheme

1. Blue for directories
2. Green for executables
3. Magenta for normal files

You can use this command to seek a file in the \<location> specified directory with prefix as <target_prefix>

It comes with 3 flags d, e and f.

Example:

```sh
<username@systemname:cwd> seek -f spec3 .
file: ./spec3/spec3.c
file: ./spec3/spec3.h
<username@systemname:cwd> seek -d spec3
directory: ./spec3
inside directory ./spec3
Hopped to directory: ~/spec3
<username@systemname:~/spec3> seek -f outro.sh
file: ./outro.sh
|----------\  \         / |------------
|           \  \       /  |
|           |   \     /   |
|           /    \   /    |
|__________/      \_/     |____________
|          \       |      |
|           \      |      |
|           |      |      |
|           /      |      |
|__________/       |      |------------
<username@systemname:~/spec3>
```

### Specification 9 (.myshrc)

This file is similar to the .bashrc (or .zshrc) file in bash(or zsh). This contains few aliases and functions which you can safely use. Also fell free to create your own functions or aliases

```sh
alias reveall=reveal -l
alias reveala=reveal -a

home=hop ~

acts=activities

func hop_seek()
{
    hop "$1" # Hop into this directory
    seek "$1" # search for files/directories with the same name as the directory you just hopped into.
}

func le()
{
    log execute "$1" # execute this command
}
```

### Specification 10 (I/O Redirection)

Redirection of input or output is done via these 3 symbols:

```bash
> - # redirects the output and writes the output to the specified file
>> - # redirects the output and appends the output to the specified file
< - # takes the input from the specified file and gives it to the specified process
```

This type of functionality is similar to that of bash or zsh.

### Specification 11 (Pipes)

Again, we use `|` as pipe between processes whose behaviour and syntax are similar to that of bash or zsh.

### Specification 12 (Redirection along with pipes)

Just a combination of Specification 10 and Specification 11.

### Specification 13 (Activities)

#### Syntax

```activities```

On executing this command, we will get all the background tasks currently running whose parent is **MySH**.

### Specification 14 (Signals)

#### Syntax

```ping <pid> <signal_number>```

This will send the specified signal to the specified process.

Additionally, we've added 2 more signals to the shell.

1. **ctrl-c** to send a SIGKILL signal to the currently running foreground process
2. **ctrl-d** to delete all the background processes and then eventually terminating the current shell process as well.

### Specification 16 (Neonate)

#### Syntax

```neonate -n [time_delay]```

On executing this command, we'll be getting the most recent process that has been started by that poiint of time and it will repeat this process every `time_delay` amount of seconds infinitely until a key press of *x* is given.

#### Example
```bash
<username@systemname:cwd>: neonate -n 2
6700
6703
6704
6706
# key x is pressed
<username@systemname:cwd>:
```

### Specification 17 (iMan)

#### Syntax

```iMan <prompt>```

This command is equivalent to man command in bash or zsh. This command will deliver the man pages of the desired command to you from the webserver with domain name man.he.net.

**ChatGPT Chat**: [ChatGPT](https://chatgpt.com/share/0422dc56-dce5-412c-9d9a-573641138347)

<!-- [![Review Assignment Due Date](https://classroom.github.com/assets/deadline-readme-button-22041afd0340ce965d47ae6ef1cefeee28c7c493a6346c4f15d667ab976d596c.svg)](https://classroom.github.com/a/Qiz9msrr)
# mini-project-1-template -->
