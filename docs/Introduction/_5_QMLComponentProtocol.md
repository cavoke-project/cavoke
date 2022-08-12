# Client Component Protocol {#QMLComponentProtocol}

Client component interaction with the main cavoke application is extremely straightforward.

Let's look into a sample `app.qml` based on the one
from [tictactoe](https://github.com/cavoke-project/tictactoe/blob/master/client/app.qml) game:

~~~{.qml}
import QtQuick 2.0

Rectangle {
    // BEGIN cavoke section
    Connections {
        target: cavoke

        function onReceiveUpdate(update) {
            console.log("Received: " + update);
        }
    }
    // END cavoke section
    onClicked: {
        cavoke.getMoveFromQml("rectangle-clicked");
    }
}
~~~

First of all, add a `Connections` object to your root object (in this instance `Rectangle`). Please set your target
as `cavoke` and create a function `onReceiveUpdate(str)` for receiving signals from the cavoke main application.

Now you can receive and send moves! To receive updates just implement the `onReceiveUpdate` slot (in this example it
just prints it to console). And to send moves just use `cavoke.getMoveFromQml(str)` function.