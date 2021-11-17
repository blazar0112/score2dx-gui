import QtQuick 2.15
import QtQuick.Controls 2.15

Rectangle {
    property alias text: innerText.text
    property alias font: innerText.font
    width: 60
    height: 50
    border.color: 'black'
    color: 'dodgerblue'
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
