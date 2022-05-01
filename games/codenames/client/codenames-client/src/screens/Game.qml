import QtQuick 2.0
import QtQuick.Controls 2.15
import QtQuick.Layouts 1.15


import "../components"
import "../interactions/interactions.js" as Interact

Item {
    // BEGIN cavoke section
    Connections {
        target: cavoke

        function onReceiveUpdate(jsonUpdate) {
            console.log("Received: " + jsonUpdate);
            Interact.processUpdate(jsonUpdate);
        }
    }
    // END cavoke section

    property var idMap: ({ board:board, hintControls:hintControls})

    function findItemById(id) {
          return idMap[id];
        }

    Rectangle {
        anchors.fill: parent;
        Grid {
           id: board
           anchors.centerIn: parent
           spacing: 5;
        }
    }

    Rectangle {
        id: hintControls;

        anchors.bottom: parent;
        anchors.horizontalCenter: parent;
        anchors.bottomMargin: 10;
        RowLayout {
            anchors.fill: parent;
            TextInput {
                id: hintInput;
                width: 300;
                height: 30;
                visible: true;
            }
            Button {
                id: hintSubmit;
                text: "Submit";
                onClicked: console.log("Submit hint");
                width: 100;
                height: 30;

            }

        }
    }

//    Component.onCompleted: drawBoard(5, 5, [], [])
}
