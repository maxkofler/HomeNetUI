import QtQuick 2.12
import QtQuick.Window 2.12


Window {
    id: root
    width: 640
    height: 480
    visible: true
    title: qsTr("Hello World")

    Value{
        width: root.width / 1.2
        height: root.height / 1.2
        x: root.width / 2 - width / 2
        y: root.height / 2 - height / 2
    }
}
