import QtQuick 2.7
import QtQuick.Controls 2.2
import QtScxml 5.8

Frame {
    id: banner
    height: 120
    topPadding: 8
    bottomPadding: 8
    opacity: 0.8
    antialiasing: true
    property StateMachine stateMachine

    background: Rectangle {
            color: "white"
            border.color: "darkGray"
            radius: 1
        }

    Row {
        leftPadding: 8
        spacing: 20
        anchors.fill: parent

        StateButton {
            radius: 35
            source: "qrc:/img/photo-camera.svg"
            enabled: stateMachine.Ready || stateMachine.NoSnap
            stateMachine: banner.stateMachine
            name: "SnapShot"

            onClicked: {
                lastVisibility = window.visibility
                window.visibility = ApplicationWindow.Minimized
                ocr.capture()
            }
        }

        VSeparator {}

        MousePositionSelector {
            enabled: stateMachine.Ready || stateMachine.GetMousePosition
            stateMachine: banner.stateMachine
            name: "MousePosition"
        }

        VSeparator {}

        DataDelegate {
            titleIcon: "qrc:/img/tag.png"
            titleText: "合约"
            enabled: stateMachine.Ready
        }

        VSeparator {}

        DataDelegate {
            titleIcon: "qrc:/img/calendar.png"
            titleText: "日期"
            enabled: stateMachine.Ready
        }

        VSeparator {}

        DataDelegate {
            titleIcon: "qrc:/img/clock.png"
            titleText: "时间"
            enabled: stateMachine.Ready
        }

        VSeparator {}

        DataDelegate {
            titleIcon: "qrc:/img/dollar.png"
            titleText: "点位"
            enabled: stateMachine.Ready
        }

        VSeparator {}
    }

}
