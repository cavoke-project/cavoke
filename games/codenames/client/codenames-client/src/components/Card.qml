import QtQuick 2.0

import "../interactions/interactions.js" as Interact

Item {
    id: card;

    property string word: ""
    property int state: 0
    property int player: 0
    property int card_index: 0
    width: 180
    height: 100

    function getColor(state) {
        if (state === 0) {
            return "blue";
        } else if (state === 1) {
            return "red";
        } else if (state === 2) {
            return "black";
        } else if (state === 3) {
            return "lime";
        } else {
            return "cyan";
        }
    }

    function getTextColor(state) {
        if (state === 0) {
            return "white";
        } else if (state === 1) {
            return "white";
        } else if (state === 2) {
            return "white";
        } else if (state === 3) {
            return "black";
        } else {
            return "black";
        }
    }

    function getText(state, player, word) {
        if (state === 4 || player === 0 || player === 1) {
            return word;
        } else {
            return "";
        }
    }

    Rectangle {
        id: card_content
        anchors.fill: parent
//        radius: 10

        color: getColor(card.state);

        Text {
            id: card_hint;

            anchors.centerIn: parent;
            text: getText(card.state, card.player, card.word);
            color: getTextColor(card.state);
        }

        MouseArea {
            anchors.fill: parent
            onClicked: Interact.openCard(parent.parent.card_index);
        }
    }
}
