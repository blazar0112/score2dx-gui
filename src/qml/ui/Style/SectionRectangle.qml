import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Controls.Styles 1.4
import QtQuick.Layouts 1.12

Rectangle {
    id: root

    property alias innerText: innerText

    height: 30
    //border.color: 'black'

    gradient: Gradient {
        orientation: Gradient.Horizontal
        GradientStop { position: 0.0; color: 'white' }
        GradientStop { position: 1.0; color: 'darkviolet' }
    }

    Text {
        id: innerText

        anchors.centerIn: parent

        font.family: 'Verdana'
        font.pixelSize: 20
        color: 'black'
        horizontalAlignment: Text.AlignHCenter
    }
}
