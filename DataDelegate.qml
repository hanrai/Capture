import QtQuick 2.7
import QtQuick.Controls 2.2

Pane {
    id: panel
    property string titleIcon
    property string titleText
    padding: 0
    anchors.verticalCenter: parent.verticalCenter
    background: null
    opacity: 0.1
    enabled: false

    Column {
        CellTitle {
            titleIcon: panel.titleIcon
            titleText: panel.titleText
        }
        HSeparator {}
        Row {
            padding: 8
            spacing: 8

            StateButton {
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
                DataText {txt: "8888"}
                LabelText {text: "Y"}
                LabelText {text: ":"}
                DataText {txt: "8888"}
                LabelText {text: "W"}
                LabelText {text: ":"}
                DataText {txt: "8888"}
                LabelText {text: "H"}
                LabelText {text: ":"}
                DataText {txt: "8888"}
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
                    color: "red"
                    border.color: "black"
                }
            }
        }
    }

    states: [
        State {
            name: "ENABLED"
            when: panel.enabled
            PropertyChanges { target: panel; opacity: 0.8 }
        }
    ]

    transitions: Transition {
        from: ""; to: "ENABLED"; reversible: true
        NumberAnimation { properties: "opacity"; duration: 100}
    }
}
