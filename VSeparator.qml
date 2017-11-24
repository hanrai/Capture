import QtQuick 2.0

Rectangle {
    property int margin: 0
    width: 1
    border.color: "darkGray"
    height: parent.height - margin * 2
    anchors.verticalCenter: parent.verticalCenter
}
