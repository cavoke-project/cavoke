# Cavoke Server {#CavokeServer}

## About the server

**Cavoke server** (aka. main cavoke server, main server) is the server-application that is responsible for connecting
players with their games and persisting the game session information.

## How to Use

### Docker

The simplest way to start up the server is inside a **docker container**.

#### Docker Compose

For local development or small-scale use you can start it using **docker-compose**.

```console
cavoke@ubuntu:~$ docker-compose up -d
cavoke@ubuntu:~$ curl -s localhost:8080/games/list
[{"display_name":"Tic-tac-toe","id":"tictactoe", ... }]
```

#### Manual docker installation

If you don't want to use docker-compose, for instance you want to run it inside a kubernetes, you can use a pre-built [docker image](https://ghcr.io/cavoke-project/cavoke-server). You must attach an
initialized (with [schema.sql](./db/schema.sql)) **database** (Postgres14 recommended, other database might work, but
have not been tested) and a **mounted directory with games**. Both these parameters are most easily specified using
the [config file]. <!-- TODO -->

Example with *local* games (see [local vs remote games]):

```console
cavoke@ubuntu:~$ find . -type f
./local_games/tictactoe/client.zip
./local_games/tictactoe/logic
./local_games/tictactoe/config.json
cavoke@ubuntu:~$ chmod +x ./local_games/tictactoe/logic  # Required for executing 
cavoke@ubuntu:~$ docker run -v `pwd`/local_games:/mnt/games -p 8080:8080 -d ghcr.io/cavoke-project/cavoke-server -g /mnt/games
cavoke@ubuntu:~$ curl -s localhost:8080/games/list
[{"display_name":"Tic-tac-toe","id":"tictactoe", ... }]
```

A detailed description of command line options is available by
running `docker run -t ghcr.io/cavoke-project/cavoke-server --help`

### Build Server

#### On a local machine

This project uses CMake for building.

Most external libraries are added as submodules into [third_party](../third_party) folder for easier configuration.
Unfortunately, their dependencies are not particularly available and must be downloaded separately.

##### Ubuntu

All required packages are available using `apt`:

```console
cavoke@ubuntu:~$ apt install libboost-all-dev \
    openssl libssl-dev libjsoncpp-dev uuid-dev zlib1g-dev libc-ares-dev \
    postgresql-server-dev-all
```

Now you can build the server.

```console
cavoke@ubuntu:~$ cmake . -B build-server -DBUILD_ALL=OFF -DBUILD_SERVER=ON
```

> :information_source: If using libraries from submodules, you might need to execute
> `git submodule update --init --recursive` to download them.

##### Windows

It is recommended to use [Msys2](https://www.msys2.org/). Download the equivalent msys packages via `packman`.

##### Using external libraries

Alternatively, you can link the following libraries if they are already installed on your machine:

- [drogon](https://github.com/drogonframework/drogon) 1.7.3 or newer
- [nlohmann/json](https://github.com/nlohmann/json) 3.9.0 or newer
- [boost](https://www.boost.org/) 1.71 or newer

To tell CMake to use external libraries, use a corresponding flag. For instance, `-DUSE_EXTERNAL_DROGON=ON`.

A complete CMake command might look like this:

```console
cavoke@ubuntu:~$ cmake . -B build-server -DUSE_EXTERNAL_DROGON=ON -DUSE_EXTERNAL_NLOHMANN=ON
```

More details in the [CMakeLists.txt](./CMakeLists.txt)

#### Dev-container

For a simplified setup Cavoke also provides a [dev-container](../.devcontainer/Dockerfile) with all dependencies
installed.

#### Gitpod

Alternatively, you can develop remotely using a dev-container. For this you can use Gitpod. It starts up a remote VS
Code Instance inside a dev-container.

[![Open in Gitpod](https://gitpod.io/button/open-in-gitpod.svg)](https://gitpod.io/#https://github.com/cavoke-project/cavoke)

### SQL

Cavoke Server requires a working SQL database to store and update information about the games.

We use [Postgres 14](https://www.postgresql.org/). Any other database have not been tested, and are most likely not to
work.

To initialize your postgres database for cavoke, please use [schema.sql](db/schema.sql). Example using `psql`:

```bash
PGPASSWORD="postgres_password"
psql -h postgres -d cavoke -U postgres_user -f server/db/schema.sql
```

You should provide valid database credentials in [drogon configuration file]. <!-- TODO -->

To pass the configuration file to the server, please use `-c` option. For example:

```console
cavoke@ubuntu:~$ cavoke_server -c my_config.json
Booting server... at /home/cavoke/build/server/cavoke_server
Server configuration loaded from: 'my_config.json'
Initialize models...
Initialize controllers...
Database connection: host=127.0.0.1 port=5432 dbname=cavoke user=postgres_user <pass hidden>
Listening at 0.0.0.0:8080... 
```

## Development

### Drogon ORM

For interacting with the database from
C++ [Drogon ORM](https://github.com/drogonframework/drogon/wiki/ENG-08-3-DataBase-ORM) is used. You can find the
**autogenerated** [models](https://github.com/drogonframework/drogon/wiki/ENG-08-3-DataBase-ORM#model) in
the [sql-models](./sql-models) directory.

To (re)generate ORM sql-models (from a running database instance), please use [`drogon_ctl`](https://github.com/drogonframework/drogon/wiki/ENG-11-drogon_ctl-Command). It is installed automatically together with Drogon.

> :warning: Please do not manually edit c++ files in the [sql-models](./sql-models) directory.

Example:
```console
cavoke@ubuntu:~$ drogon_ctl create model sql-models
```

> :information_source: You will have to provide your database credentials to connect to the database.
