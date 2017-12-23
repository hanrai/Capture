import QtQuick 2.0
import QtQuick.Controls 2.3
import com.sophia.SpotInfo 1.0

Item {
    x: spot.shape.x
    y: spot.shape.y - 16
    width: spot.shape.width
    height: spot.shape.height + 16
    visible: spot.actived && !machine.NoSnap
    property string name
    property SpotInfo spot: engine.getSpotInfo(name)
    property alias source: icon.source

    Image {
        id: icon
        width:16
        height:16
        x: 0
        y: 0
    }

    Button {
        id: button
        x: 0
        y: 16
        width: spot.shape.width
        height: spot.shape.height
        contentItem: Rectangle {
            anchors.fill: parent
            opacity: button.down ? 0.4 : 0.7
            color: "lightGray"
            border.color: "darkGray"
            border.width: 1
            radius: 2
        }
        background: null
    }
}
