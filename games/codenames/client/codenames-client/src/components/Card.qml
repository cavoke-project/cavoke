import QtQuick 2.0

import "../interactions/interactions.js" as Interact

Item {
    property var idMap: ({ card:card, card_content:card_content })

    function findItemById(id) {
          return idMap[id];
        }

    id: card
    property string word: ""
    property int state: 0
    property int player: 0
    property int card_index: 0
    width: 180
    height: 100

    function getColor(card) {
        if (card.state === 0) {
            return "blue";
        } else if (card.state === 1) {
            return "red";
        } else if (card.state === 2) {
            return "black";
        } else if (card.state === 3) {
            return "lime";
        } else {
            return "cyan";
        }
    }

    function getTextColor(card) {
        if (card.state === 0) {
            return "white";
        } else if (card.state === 1) {
            return "white";
        } else if (card.state === 2) {
            return "white";
        } else if (card.state === 3) {
            return "black";
        } else {
            return "black";
        }
    }

    function getText(card) {
        if (card.state === 4 || card.player === 0 || card.player === 1) {
            return card.word;
        } else {
            return "";
        }
    }

    Rectangle {
        id: card_content
        anchors.fill: parent
//        radius: 10

        color: getColor(card);

        Text {
            id: card_hint

            anchors.centerIn: parent
            text: getText(card)
            color: getTextColor(card)
        }

        MouseArea {
            anchors.fill: parent
            //onClicked: Interact.openCard(card.card_index);
        }
    }
}
