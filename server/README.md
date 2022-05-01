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

#### Установка библиотек

Для работы сервера нужно установить несколько библиотек:

- [drogon](https://github.com/drogonframework/drogon) не старее 1.7.3
- [nlohmann/json](https://github.com/nlohmann/json) не старее 3.9.0
- [boost](https://www.boost.org/) не старее 1.71

На своей машине первые две библиотеки проще собрать самому. В nlohmann вообще отсутствуют внешние зависимости. В drogon
же их много, поэтому сборка не на linux может быть увлекательным процессом.

### SQL

Для обработки большинства запросов сервера требуется база данных [Postgresql](https://www.postgresql.org/).

После того как сервер postgres запущен, создайте необходимые таблицы с помощью [скрипта sql](db/schema.sql). Например,
на linux:

```bash
PGPASSWORD="postgres_password"
psql -h postgres -d postgres_db -U postgres_user -f server/db/schema.sql
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
Database connection: host=127.0.0.1 port=5432 dbname=postgres_db user=postgres_user <pass hidden>
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