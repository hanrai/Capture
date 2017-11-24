import QtQuick 2.0

Image {
    source: "qrc:/img/cursor.png"
    width:16
    height:16
    x: mousePosition.spotX - 5
    y: mousePosition.spotY - 5
    visible: (mousePosition.spotX > 0 && mousePosition.spotY > 0) && !machine.NoSnap
}
