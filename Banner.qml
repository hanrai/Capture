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
            enabled: true
            opacity: 0.8
            name: "SnapShot"

            onClicked: {
                lastVisibility = window.visibility
                window.visibility = ApplicationWindow.Minimized
                engine.capture()
            }
        }

        VSeparator {}

        MousePositionSelector {
            enabled: machine.Ready
            name: "鼠标位置"
        }

        VSeparator {}

        DataDelegate {
            titleIcon: "qrc:/img/tag.png"
            name: "合约"
        }

        VSeparator {}

        DataDelegate {
            titleIcon: "qrc:/img/calendar.png"
            name: "日期"
        }

        VSeparator {}

        DataDelegate {
            titleIcon: "qrc:/img/clock.png"
            name: "时间"
        }

        VSeparator {}

        DataDelegate {
            titleIcon: "qrc:/img/dollar.png"
            name: "点位"
        }

        VSeparator {}
    }

}
