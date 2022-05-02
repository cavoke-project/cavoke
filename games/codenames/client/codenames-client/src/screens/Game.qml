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

    Rectangle {
        color: "#f0ffff";
        anchors.fill: parent;
        Grid {
           id: board
           anchors.centerIn: parent
           spacing: 5;
        }
    }

    ColumnLayout {
            id: cardsLeftBlock;

            anchors.top: parent.top;
            anchors.left: parent.left;
            anchors.topMargin: 25;
            anchors.leftMargin: 25;

            RowLayout {
                anchors.left: parent.left;
                Text {
                    id: blueCardsLeftLabel;
                    color: "blue";
                    text: "Blue cards left: "
                    anchors.left: parent.left;
                }
                Text {
                    id: blueCardsLeft;
                    color: "blue";
                    anchors.left: blueCardsLeftLabel.right;
                }
            }

            RowLayout {
                anchors.topMargin: 5;
                anchors.left: parent.left;

                Text {
                    id: redCardsLeftLabel;
                    color: "red";
                    text: "Red cards left: "
                    anchors.left: parent.left;
                }
                Text {
                    id: redCardsLeft;
                    color: "red";
                    anchors.left: redCardsLeftLabel.right;
                }
            }

        }

    RowLayout {
        id: currentHintBlock;

        anchors.top: parent.top;
        anchors.horizontalCenter: parent.horizontalCenter;
        anchors.topMargin: 25;
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

    Text {
        id: yourTurnLabel;
        color: "black";
        text: "Your turn!";
        anchors.right: yourRoleBlock.right;
        anchors.top: yourRoleBlock.bottom;
        anchors.topMargin: 5;

        visible: false;
    }

    RowLayout {
        id: yourRoleBlock;

        anchors.top: parent.top;
        anchors.right: parent.right;
        anchors.topMargin: 25;
        anchors.rightMargin: 25;
        Text {
            id: yourRoleLabel;
            color: "black";
            text: "Your role is: "
            anchors.right: yourRole.left;
        }
        Text {
            id: yourRole;
            color: "black";
            anchors.right: parent.right;
        }

    }

    RowLayout {
        id: hintControls;

        anchors.bottom: parent.bottom;
        anchors.horizontalCenter: parent.horizontalCenter;
        anchors.bottomMargin: 25;

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
