# Components in Detail {#ComponentsDetailed}

## Server Component

As mentioned before, a server component is responsible for validating game moves, checking states against game rules,
and preserving game invariants.

Moreover, to ensure simplicity every server component has to be **stateless**. This means that server component instance
can be restarted at any point, or a different instance might be used (for scaling purposes).

This architecture forces the developers to store all the information about the game in a single entity, that we call
**global state**. On our platform this global state must be representable as a UTF-8 formatted string. Read more in
the [Server Component Protocol](#GameLogicProtocol).

### Server Component types

This server component can be implemented in two different ways. For both types we have
provided **[a game template](https://github.com/cavoke-project/cavoke-game-template)** that has already all json and
webserver stuff done, and only requires you to implement the three methods.

#### Local

**Local** server component is a **binary executable** in the main server's filesystem that is executed each time for
move processing. Local server component accepts commands from `stdin` and must answer to `stdout`. See details
in [Server Component Porotocol](#GameLogicProtocol).

#### Remote

**Remote** server component is a **remotely hosted webserver** that is accessed each time for move processing. Remote
server component must comply with this [OpenAPI schema](https://cavokedev.docs.apiary.io). See details
in [Server Component Porotocol](#GameLogicProtocol).

## Client Component

A client component is a [QML](https://doc.qt.io/qt-6/qtqml-index.html) application. It is **dynamically attached to the
main cavoke client process** and is able
to communicate via it: **sending moves** and **receiving updates** from the main server.

It must implement a game interface seen by the player and perform client-side
validation (if required).

This design allows the client component not to worry about any game session logics, authentication and requests
transport – all of this is handled by the main client. Read more in [Client Component Protocol](#QMLComponentProtocol).