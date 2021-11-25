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

        color: innerText.text==='MAX' ? 'cyan'
               : innerText.text.startsWith('MAX+') ? 'cyan'
               : innerText.text.startsWith('MAX-') ? 'gold'
               : innerText.text.startsWith('AAA+') ? 'gold'
               : innerText.text.startsWith('AAA-') ? 'lightsteelblue'
               : innerText.text.startsWith('AA+') ? 'lightsteelblue'
               : innerText.text.startsWith('AA-') ? 'mediumspringgreen'
               : innerText.text.startsWith('A+') ? 'mediumspringgreen'
               : 'orchid'

        font.family: 'Verdana'
        font.pixelSize: 14
        font.bold: true
        style: Text.Outline
        horizontalAlignment: Text.AlignHCenter
    }
}
