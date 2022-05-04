import QtQuick 2.0
import QtQuick.Controls 2.15
import QtQuick.Layouts 1.15


import "../components"
import "../interactions/interactions.js" as Interact
import "../style/colors.js" as Colors

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
    Item {
        id: gameBoard;
        visible: true;
        anchors.fill: parent;

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
                        color: Colors.BLUE_AGENT;
                        text: "Blue cards left: "
                        anchors.left: parent.left;
                    }
                    Text {
                        id: blueCardsLeft;
                        color: Colors.BLUE_AGENT;
                        anchors.left: blueCardsLeftLabel.right;
                    }
                }

                RowLayout {
                    anchors.topMargin: 5;
                    anchors.left: parent.left;

                    Text {
                        id: redCardsLeftLabel;
                        color: Colors.RED_AGENT;
                        text: "Red cards left: "
                        anchors.left: parent.left;
                    }
                    Text {
                        id: redCardsLeft;
                        color: Colors.RED_AGENT;
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
            Text {
                id: currentAttemptsLabel;
                color: "black";
                text: "attempts left: "
                anchors.left: currentHint.right;
                anchors.leftMargin: 50;
            }
            Text {
                id: currentAttempts;
                color: "black";
                anchors.left: currentAttemptsLabel.right;
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
                color: "white";
                border.width: 1;
                border.color: "black";

                width: 600;
                height: 30;
                TextInput {
                    anchors.fill: parent;
                    anchors.leftMargin: 3;
                    id: hintInput;
                    visible: true;
                    verticalAlignment: Qt.AlignVCenter;
                    font.capitalization: Font.AllLowercase
                    font.pointSize: 15;
                    maximumLength: 37;
                }
            }

            Rectangle {
                color: "white";
                border.width: 1;
                border.color: "black";

                width: 75;
                height: 30;
                anchors.leftMargin: 5
                TextInput {
                    anchors.fill: parent;
                    anchors.leftMargin: 3;
                    id: attemptsInput;
                    visible: true;
                    verticalAlignment: Qt.AlignVCenter;
                    font.pointSize: 15;
                    validator: IntValidator {bottom: 1; top: 200}
                }
            }

            Button {
                id: hintSubmit;
                text: "Submit";
                onClicked: {
                    let hint = hintInput.text
                    let attempts = attemptsInput.text;
                    hintInput.text = "";
                    attemptsInput.text = "";
                    Interact.makeHint(hint, parseInt(attempts));
                }
                width: 100;
                height: 30;
                anchors.leftMargin: 5
            }
        }

        RowLayout {
            id: skipControls;

            anchors.bottom: parent.bottom;
            anchors.horizontalCenter: parent.horizontalCenter;
            anchors.bottomMargin: 25;
            Button {
                id: skipButton;
                text: "Skip";
                onClicked: {
                    Interact.skip();
                }
                width: 100;
                height: 30;

            }
        }
    }
    Item {
        id: resultsScreen;
        visible: false;
        anchors.centerIn: parent;
        anchors.fill: parent;
        Text {
            id: blueWins;
            visible: false;
            text: "Blue team wins!";
            color: Colors.BLUE_AGENT;
            font.pointSize: 64;

            anchors.centerIn: parent;
        }

        Text {
            id: redWins;
            visible: false;
            text: "Red team wins!";
            color: Colors.RED_AGENT;
            font.pointSize: 64;

            anchors.centerIn: parent;
        }
    }

//    Component.onCompleted: drawBoard(5, 5, [], [])
}
