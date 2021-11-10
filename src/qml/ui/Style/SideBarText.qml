import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Layouts 1.12

Rectangle {
    property alias text: inner.text

    Layout.minimumWidth: 140
    Layout.fillWidth: true
    height: 30
    color: '#2ECC71'

    Text {
        id: inner
        anchors.fill: parent
        font.family: 'Verdana'
        font.pixelSize: 16
        horizontalAlignment: Text.AlignHCenter
        verticalAlignment: Text.AlignVCenter
    }
}


