import QtQuick 2.15
import QtQuick.Controls 2.15

Rectangle {
    property alias innerText: innerText

    width: 60
    height: 30

    border.color: 'black'
    color: 'darkslategray'

    Text {
        id: innerText

        width: parent.width
        anchors.centerIn: parent

        color: text==='N/A' ? 'lightpink' : 'white'

        font.family: 'Verdana'
        font.pixelSize: text==='N/A' ? 12 : 14
        font.bold: true
        style: Text.Outline
        horizontalAlignment: Text.AlignHCenter
    }
}
