import QtQuick 2.7
import QtQuick.Controls 2.2
import QtScxml 5.8

Pane {
    id: panel
    property string icon
    property string name
    property StateMachine stateMachine
    padding: 0
    anchors.verticalCenter: parent.verticalCenter
    background: null
    opacity: 0.1
    enabled: false

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
                radius: 20
                source: "qrc:/img/target.png"
                enabled: panel.enabled
                stateMachine: panel.stateMachine
                name: panel.name
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
