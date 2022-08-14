# Client Server Communication {#ClientServerProtocol}

## Main ideas

There are three types of communication between the client and the server.

1. Sending a move for game session
2. Getting the latest state for game session
3. Everything else (new game room, sessions, statistics, etc.)

We have chosen to use simple **http**-protocol for all of these scenarios. For type 2 requests, simple polling is used.
In the future, types 1 and 2 might be replaced with Websockets or HTTP/2.

## OpenAPI docs

We use an **OpenAPI schema** (formerly Swagger) to formalize our http communication.
Detailed documentation is available on apiary:

[![openapi](https://img.shields.io/badge/Cavoke-passing-success?logo=swagger&logoColor=white&style=for-the-badge)](https://cavoke.docs.apiary.io)
