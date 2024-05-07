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

#### Running the project 🏃‍♂️

##### Compiling the code 🛠️
```bash
# To work on client
$ make client

$ ./client/client execute <estimated time> -u <command>
$ ./client/client status

# To work on server
$ make orchestrator

$ ./server/server <output directory> <max tasks> <scheduling policy>
```

##### Cleaning object files, executable and logs 🧹
```bash
$ make clean
```

## Developed by 🧑‍💻:

- [Afonso Pedreira](https://github.com/afooonso)
- [Dário Guimarães](https://github.com/darguima)
- [Fábio Magalhães](https://github.com/Mackgame4)
