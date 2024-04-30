# Trivial-Orchestrator OS

## About the Project

This project was developed for the subject Operating systems from University of Minho - Software Engineering degree.

#### Grade ⭐️ ⚠️⚠️ Complete this after the project is done ⚠️⚠️/20

### Demo 📽️

⚠️⚠️ Complete this after the project is done ⚠️⚠️
<!-- ![Demo Image](./demo.png) -->

### The goal ⛳️

In some words, the goal of this project was to create two C programs. The first is a server that orchestrate tasks that were requested from the second one - the client.

The server have the task of asynchronously execute all the tasks, being always available to receive new tasks, and managing all them in the best way.

If you want, you can read the [project statement](enunciado.pdf) (in Portuguese).

### About the Code 🧑‍💻

⚠️⚠️ Complete this after the project is done ⚠️⚠️

## Getting Started 🚀

#### Cloning the repository

```bash
$ git clone https://github.com/Trivial-Orchestrator-OS
$ git clone git@github.com:Darguima/Trivial-Orchestrator-OS.git
```

### Installing dependencies 📦

```bash
# Debian / Ubuntu / Mint
$ sudo apt-get install valgrind

# Arch
$ sudo pacman -S valgrind
```

#### Running the project 🏃‍♂️

Since this project is split into two programs, you need choose which one you want to work.

```bash
# To work on client
$ cd Trivial-Orchestrator-OS/client

# To work on server
$ cd Trivial-Orchestrator-OS/server
```

##### Compiling the code 🛠️
```bash
$ make

# With debug flags
$ DEBUG=1 make

# Executing program
$ ./client <...args>
$ ./server <...args>
```

##### Developing

This function `clean`, `format` and `compile` in debug mode and `execute` the project

```bash
$ make dev <...args>
```

##### Running memory leaks checks

Will run Valgrind to detect memory leaks.
This also compiles the code, and pass default params to the executable.

```bash
$ make check-memory
```

##### Cleaning object files and executable
```bash
$ make clean
```

## Developed by 🧑‍💻:

- [Afonso Pedreira](https://github.com/afooonso)
- [Dário Guimarães](https://github.com/darguima)
- [Fábio Magalhães](https://github.com/Mackgame4)
