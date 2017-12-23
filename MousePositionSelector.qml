import QtQuick 2.7
import QtQuick.Controls 2.2
import QtScxml 5.8
import com.sophia.SpotInfo 1.0

Pane {
    id: panel
    property string icon
    property string name
    property SpotInfo info: engine.getSpotInfo(name)
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
            titleText: name
        }
        HSeparator {}
        Row {
            padding: 8
            spacing: 8

            StateButton {
                id: button
                radius: 20
                name: panel.name
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
                DataText {txt: info.shape.x}
                LabelText {text: "Y"}
                LabelText {text: ":"}
                DataText {txt: info.shape.y}
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
                DataText {txt: info.shape.width}
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
