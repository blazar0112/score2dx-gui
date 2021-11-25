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

        color: text==='NO PLAY' ? 'darkgray'
               : text==='FAILED' ? 'firebrick'
               : text==='ASSIST' ? 'plum'
               : text==='EASY' ? 'palegreen'
               : text==='CLEAR' ? 'deepskyblue'
               : text==='HARD' ? 'red'
               : text==='EX HARD' ? 'gold'
               : text==='FC' ? 'cyan'
               : 'white'

        font.family: 'Verdana'
        font.pixelSize: 14
        font.bold: true
        style: Text.Outline
        horizontalAlignment: Text.AlignHCenter
    }
}
