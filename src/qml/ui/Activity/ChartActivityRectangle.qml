import QtQuick 2.15
import QtQuick.Controls 2.15

Rectangle {
    property alias innerText: innerText
    width: 60
    height: 30
    border.color: 'black'
    color: 'lightslategray'
    Text {
        id: innerText

        width: parent.width
        anchors.centerIn: parent

        color: 'white'
        font.family: 'Verdana'
        font.pixelSize: 16
        font.bold: true
        wrapMode: Text.WrapAtWordBoundaryOrAnywhere
        horizontalAlignment: Text.AlignHCenter
    }
}
