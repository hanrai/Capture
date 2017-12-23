import QtQuick 2.0
import com.sophia.SpotInfo 1.0

Image {
    property SpotInfo info: engine.getSpotInfo("鼠标位置")
    source: "qrc:/img/cursor.png"
    width:16
    height:16
    x: info.shape.x - 5
    y: info.shape.y - 5
    visible: (info.shape.x > 0 && info.shape.y > 0) && !machine.NoSnap
}
