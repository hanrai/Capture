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
                engine.capture()
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
            name: "Contract"
            spotX: contractSpot.spotX
            spotY: contractSpot.spotY
            spotWidth: contractSpot.spotWidth
            spotHeight: contractSpot.spotHeight
            colorName: contractSpot.colorName
            enabled: machine.Ready || (machine.HotSpotScan && hotSpot.name == name)
        }

        VSeparator {}

        DataDelegate {
            titleIcon: "qrc:/img/calendar.png"
            titleText: "日期"
            name: "Date"
            spotX: dateSpot.spotX
            spotY: dateSpot.spotY
            spotWidth: dateSpot.spotWidth
            spotHeight: dateSpot.spotHeight
            colorName: dateSpot.colorName
            enabled: machine.Ready || (machine.HotSpotScan && hotSpot.name == name)
        }

        VSeparator {}

        DataDelegate {
            titleIcon: "qrc:/img/clock.png"
            titleText: "时间"
            name: "Time"
            spotX: timeSpot.spotX
            spotY: timeSpot.spotY
            spotWidth: timeSpot.spotWidth
            spotHeight: timeSpot.spotHeight
            colorName: timeSpot.colorName
            enabled: machine.Ready || (machine.HotSpotScan && hotSpot.name == name)
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
