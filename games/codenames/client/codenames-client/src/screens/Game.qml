import QtQuick 2.0
import QtQuick.Controls 2.15
import QtQuick.Layouts 1.15


import "../components"
import "../interactions/interactions.js" as Interact

Rectangle {
    // BEGIN cavoke section
    Connections {
        target: cavoke

        function onReceiveUpdate(jsonUpdate) {
            console.log("Received: " + jsonUpdate);
            Interact.processUpdate(jsonUpdate);
        }
    }
    // END cavoke section

    property var idMap: ({ board:board, hintControls:hintControls, currentHint:currentHint });

    function findItemById(id) {
          return idMap[id];
        }



    Rectangle {
        color: "#f0ffff";
        anchors.fill: parent;
        Grid {
           id: board
           anchors.centerIn: parent
           spacing: 5;
        }
    }

    RowLayout {
                id: currentHintBlock;

                anchors.top: parent.top;
                anchors.horizontalCenter: parent.horizontalCenter;
                anchors.topMargin: 50;
                Text {
                    id: currentHintLabel;
                    color: "black";
                    text: "Current hint: "
                    anchors.left: parent.left;
                }
                Text {
                    id: currentHint;
                    color: "black";
                    anchors.left: currentHintLabel.right;
                }

            }

    RowLayout {
        id: hintControls;

        anchors.bottom: parent.bottom;
        anchors.horizontalCenter: parent.horizontalCenter;
        anchors.bottomMargin: 75;

        Rectangle {
            color: "#faebd7";
            width: 300;
            height: 30;
            TextInput {
                anchors.fill: parent;
                id: hintInput;
                visible: true;
            }
        }
        Button {
            id: hintSubmit;
            text: "Submit";
            onClicked: {
                let hint = hintInput.text
                hintInput.text = "";
                Interact.makeHint(hint);
            }
            width: 100;
            height: 30;

        }

    }

//    Component.onCompleted: drawBoard(5, 5, [], [])
}
