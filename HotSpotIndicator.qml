import QtQuick 2.0
import QtQuick.Controls 2.3

Button {
    text: hotSpot.name
    x: hotSpot.spotX
    y: hotSpot.spotY
    width: hotSpot.spotWidth
    height: hotSpot.spotHeight
    visible: hotSpot.actived
}
