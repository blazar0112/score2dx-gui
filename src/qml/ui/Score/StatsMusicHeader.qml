import QtQuick 2.15
import QtQuick.Controls 2.15

Rectangle {
    property alias text: innerText.text
    width: 60
    height: 50
    border.color: 'black'
    color: '#A569BD'
    Text {
        id: innerText
        anchors.centerIn: parent
        color: 'white'
        font.family: 'Verdana'
        font.pixelSize: 18
        font.bold: true
    }
}
