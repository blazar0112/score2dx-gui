import QtQuick 2.15
import QtQuick.Controls 2.15
import QtGraphicalEffects 1.0

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

        color: innerText.text==='AAA' ? 'gold'
               : innerText.text==='AA' ? 'lightsteelblue'
               : innerText.text==='A' ? 'mediumspringgreen'
               : 'orchid'

        font.family: 'Verdana'
        font.pixelSize: 14
        font.bold: true
        style: Text.Outline
        horizontalAlignment: Text.AlignHCenter
    }
}
