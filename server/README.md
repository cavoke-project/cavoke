# Серверный компонент Cavoke

## О сервере

TODO

## Как запустить

### Docker

Проще всего сервер запустить внутри docker контейнера.

#### Docker Compose

Для локальной разработки в одну команду можно запустить сервер через docker-compose.

```console
cavoke@ubuntu:~$ docker-compose up -d
cavoke@ubuntu:~$ curl -s localhost:8080/games/list
[{"display_name":"Tic-tac-toe","id":"tictactoe", ... }]
```

Для этого можно воспользоваться актуальным
image [`ghcr.io/cavoke-project/cavoke-server`](ghcr.io/cavoke-project/cavoke-server).

#### Пример с mount игр

```console
cavoke@ubuntu:~$ find . -type f
./local_games/tictactoe/client.zip
./local_games/tictactoe/logic
./local_games/tictactoe/config.json
cavoke@ubuntu:~$ docker run -v `pwd`/local_games:/mnt/games -p 8080:8080 -d ghcr.io/cavoke-project/cavoke-server -g /mnt/games
cavoke@ubuntu:~$ curl -s localhost:8080/games/list
[{"display_name":"Tic-tac-toe","id":"tictactoe", ... }]
```

<!-- TODO: execution permissions required. Example above will only list games, etc.-->

Подробное описание всех опций запуска см. в `docker run -t ghcr.io/cavoke-project/cavoke-server --help`

### Собрать самому

#### На своей машине

Для сборки необходимо установить [boost](https://www.boost.org/) не старее 1.71, а также несколько пакетов для
серверного фреймворка.

Например, на ubuntu все зависимости можно загрузить с помощью `apt`.

```console
cavoke@ubuntu:~$ apt install libboost-all-dev \
    openssl libssl-dev libjsoncpp-dev uuid-dev zlib1g-dev libc-ares-dev \
    postgresql-server-dev-all
```

После этого можно будет собрать проект с помощью `cmake`.

```console
cavoke@ubuntu:~$ mkdir -p build-cmake && cmake . -B build-cmake
```

> :information_source: Для сборки, используя локальные библиотеки, может понадобиться
> прописать `git submodule update --init --recursive` для загрузки подмодулей.

#### Установка библиотек извне

Вообще же на сервере используются следующие библиотеки. Альтернативно можно установить каждую из них отдельно,
следуя инструкциям в их документации.

- [drogon](https://github.com/drogonframework/drogon) не старее 1.7.3
- [nlohmann/json](https://github.com/nlohmann/json) не старее 3.9.0
- [boost](https://www.boost.org/) не старее 1.71

После этого, чтобы указать установку библиотеки cmake, стоит использовать флаг. Например, `-DUSE_EXTERNAL_DROGON=ON`.

Итого команда для запуска `cmake` может выглядеть так:

```console
cavoke@ubuntu:~$ mkdir -p build-cmake && cmake . -B build-cmake -DUSE_EXTERNAL_DROGON=ON -DUSE_EXTERNAL_NLOHMANN=ON
```

Более подробно смотрите в [CMakeLists.txt](./CMakeLists.txt)

#### Gitpod

Альтернативно, можно попробовать разрабатывать в удаленной среде. Для этого подключён сервис Gitpod, с помощью которого
можно в браузере запустить VS Code, в котором уже будут добавлены все зависимости, а работать он будет в облаке.

[![Open in Gitpod](https://gitpod.io/button/open-in-gitpod.svg)](https://gitpod.io/#https://github.com/cavoke-project/cavoke)

### SQL

Для обработки большинства запросов сервера требуется база данных [Postgresql](https://www.postgresql.org/).

После того как сервер postgres запущен, создайте необходимые таблицы с помощью [скрипта sql](db/schema.sql). Например,
на linux:

```bash
PGPASSWORD="postgres_password"
psql -h postgres -d cavoke -U postgres_user -f server/db/schema.sql
```

После этого нужно обновить ваш конфигурационный файл Drogon ([шаблон конфигурационного файла](./example_config.json)),
дополнив его информацией о подключении к базе данных.

Для того чтобы передать серверу конфигурационный файл, запустите его с ключом `-c`. Например:

```console
cavoke@ubuntu:~$ cavoke_server -c my_config.json
Booting server... at /home/cavoke/build/server/cavoke_server
Server configuration loaded from: 'my_config.json'
Initialize models...
Initialize controllers...
Database connection: host=127.0.0.1 port=5432 dbname=cavoke user=postgres_user <pass hidden>
Listening at 0.0.0.0:8080... 
```

## Разработка

### Drogon ORM

Для работы с базой данных
используется [Drogon ORM](https://github.com/drogonframework/drogon/wiki/ENG-08-3-DataBase-ORM). Модели находятся в
директории [sql-models](./sql-models).

Чтобы перегенерировать ORM, стоит воспользоваться утилитой
[`drogon_ctl`](https://github.com/drogonframework/drogon/wiki/ENG-11-drogon_ctl-Command), которая
устанавливается вместе с Drogon.

```console
cavoke@ubuntu:~$ drogon_ctl create model sql-models
```