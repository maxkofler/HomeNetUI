import QtQuick 2.0

Item {
    Rectangle{
        id: mainRect
        anchors.fill: parent
        color: "#2000ff95"
        radius: 5
        Text {
            anchors.fill: mainRect
            id: txtValue
            text: "0.00"
            horizontalAlignment: Text.AlignHCenter
            verticalAlignment: Text.AlignVCenter
            fontSizeMode: Text.Fit
        }
    }
}
