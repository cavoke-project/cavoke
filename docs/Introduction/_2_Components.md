# Components {#Components}

## Main Idea

The idea to achieve the described functionalities is simple. We split the game into two parts (components): the
server-side and the client-side. Then we put **our platform in between them** and provide both components with simple
yet
**powerful and comprehensible interfaces**, and let the platform in between about all authentication, persistence and
synchronization, etc.

## Our design

Our design splits the game into these two components:

1. **Server Component** (aka. **Game Logic**): This component is accessed by the [cavoke main server](#CavokeServer).
   Its purpose is to validate game moves, check states against game rules, and to preserve game invariants.
2. **Client Component** (aka. **QML Component**): A [QML](https://doc.qt.io/qt-6/qtqml-index.html) application that is
   connected to
   the [cavoke client](#CavokeClient) and is responsible for displaying the game interface, record user's moves, and to
   update the interface by incoming state updates.

In this design two pieces of infrastructure that Cavoke provides are **[Cavoke Server](#CavokeServer)**
and **[Cavoke Client](#CavokeClient)**. You can read more about them in the [Deep Dive](#DeepDive) section.

## How it works together

![Cavoke Design Diagram](cavoke-flowchart.jpg)

In this diagram we can see how cavoke works.

First, clients automatically download client components onto their machines. They are unpacked, started and attached to
the main cavoke client.

Second, when the game is started, a player can *make a move* (interact in some way with the qml interface), for which
the client
component can send a move to the server logic through its interface with the cavoke main client application. The main
application adds necessary information to the request (game session id, authentication, etc.) and sends it to the main
server. On the main server this information is checked, the corresponding game logic is accessed and a new game state is
saved.

Finally, another client can check whether the game state has been updated. It receives this new game state, and signals
the qml to update its interface using the information from the new state.