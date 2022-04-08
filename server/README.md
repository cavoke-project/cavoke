# Серверный компонент Cavoke

## О сервере

TODO

## Как запустить

### Docker

Проще всего сервер запустить внутри docker контейнера. Для этого можно воспользоваться актуальным
image [`ghcr.io/cavoke-project/cavoke-server`](ghcr.io/cavoke-project/cavoke-server).

#### Пример с mount игр

```bash
find . -type f
# ./local_games/tictactoe/client.zip
# ./local_games/tictactoe/logic
# ./local_games/tictactoe/config.json

docker run -v `pwd`/local_games:/mnt/games -p 4200:8080 -d ghcr.io/cavoke-project/cavoke-server -g /mnt/games

curl -s localhost:4200/games/list
# [{"display_name":"Tic-tac-toe","id":"tictactoe", ... }]
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
