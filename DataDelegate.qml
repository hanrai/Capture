import QtQuick 2.7
import QtQuick.Controls 2.2

Pane {
    id: panel
    property string titleIcon
    property alias name: button.name
    property int spotX
    property int spotY
    property int spotWidth
    property int spotHeight
    property string colorName
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
            titleIcon: panel.titleIcon
            titleText: button.name
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
                columns: 6
                spacing: 4
                LabelText {text: "X"}
                LabelText {text: ":"}
                DataText {txt: spotX}
                LabelText {text: "Y"}
                LabelText {text: ":"}
                DataText {txt: spotY}
                LabelText {text: "W"}
                LabelText {text: ":"}
                DataText {txt: spotWidth}
                LabelText {text: "H"}
                LabelText {text: ":"}
                DataText {txt: spotHeight}
            }
        }
        HSeparator {}
        Pane {
            anchors.horizontalCenter: parent.horizontalCenter
            topPadding: 6
            bottomPadding: 8
            background: null
            Row {
                width: parent.width
                anchors.horizontalCenter: parent.horizontalCenter
                anchors.verticalCenter: parent.verticalCenter
                spacing: 8
                Rectangle {
                    width: 12
                    height: 12
                    radius: 6
                    color: colorName
                    border.color: "black"
                }
            }
        }
    }

    states: [
        State {
            name: "ACTIVED"
            extend: "ENABLED"
            when: button.name === engine.action
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
