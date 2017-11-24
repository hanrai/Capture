import QtQuick 2.7
import QtQuick.Controls 2.2
import QtScxml 5.8

ToolBar {
    id: banner
    opacity: 0.8
    antialiasing: true

    background: Rectangle {
            color: "white"
            border.color: "darkGray"
            radius: 1
        }

    Row {
        leftPadding: 20
        spacing: 20
        anchors.fill: parent

        StateButton {
            radius: 35
            source: "qrc:/img/photo-camera.svg"
            enabled: machine.Ready || machine.NoSnap
            name: "SnapShot"

            onClicked: {
                lastVisibility = window.visibility
                window.visibility = ApplicationWindow.Minimized
                ocr.capture()
            }
        }

        VSeparator {}

        MousePositionSelector {
            enabled: machine.Ready || machine.GetMousePosition
            name: "MousePosition"
        }

        VSeparator {}

        DataDelegate {
            titleIcon: "qrc:/img/tag.png"
            titleText: "合约"
            enabled: machine.Ready
        }

        VSeparator {}

        DataDelegate {
            titleIcon: "qrc:/img/calendar.png"
            titleText: "日期"
            enabled: machine.Ready
        }

        VSeparator {}

        DataDelegate {
            titleIcon: "qrc:/img/clock.png"
            titleText: "时间"
            enabled: machine.Ready
        }

        VSeparator {}

        DataDelegate {
            titleIcon: "qrc:/img/dollar.png"
            titleText: "点位"
            enabled: machine.Ready
        }

        VSeparator {}
    }

}
