import QtQuick 2.7
import QtQuick.Controls 2.2
import QtScxml 5.8

Pane {
    id: panel
    property string icon
    property alias name: button.name
    padding: 0
    anchors.verticalCenter: parent.verticalCenter
    opacity: 0.1
    enabled: false

    background: Rectangle {
        id:bgc
        anchors.fill: parent
        color: "blueviolet"
        opacity: 0
    }

    Column {
        CellTitle {
            id: title
            titleIcon: "qrc:/img/cursor.png"
            titleText: "鼠标位置"
        }
        HSeparator {}
        Row {
            padding: 8
            spacing: 8

            StateButton {
                id: button
                radius: 20
                source: "qrc:/img/target.png"
                enabled: panel.enabled
            }

            VSeparator {
                height: 40
            }

            Grid {
                columns: 3
                spacing: 4
                LabelText {text: "X"}
                LabelText {text: ":"}
                DataText {txt: mousePosition.spotX}
                LabelText {text: "Y"}
                LabelText {text: ":"}
                DataText {txt: mousePosition.spotY}
            }
        }
        HSeparator {}
        Pane {
            anchors.horizontalCenter: parent.horizontalCenter
            topPadding: 0
            bottomPadding: 8
            background: null
            Row {
                width: parent.width
                anchors.horizontalCenter: parent.horizontalCenter
                spacing: 8
                Image {
                    width: 16
                    height: 16
                    source: "qrc:/img/resize.png"
                }
                DataText {txt: mousePosition.spotRange}
            }
        }
    }

    states: [
        State {
            name: "ACTIVED"
            extend: "ENABLED"
            when: button.name === engine.actionName
            PropertyChanges { target: bgc; opacity: 0.1 }
        },
        State {
            name: "ENABLED"
            when: panel.enabled
            PropertyChanges { target: panel; opacity: 0.8 }
        }

    ]

    transitions: [
        Transition {
            from: ""; to: "ENABLED"; reversible: true
            NumberAnimation { properties: "opacity"; duration: 100}
        },
        Transition {
            from: "ENABLED"; to: "ACTIVED"; reversible: true
            NumberAnimation { properties: "opacity"; duration: 100}
        }
    ]

}
