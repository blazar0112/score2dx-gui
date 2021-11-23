import QtQuick 2.15
import QtQuick.Controls 2.15
import QtGraphicalEffects 1.0

Rectangle {
    property alias innerText: innerText

    property string activeVersion: ''
    property string version: ''

    width: 300
    height: 40

    border.color: 'black'
    color: '#34495E'

    Rectangle {
        id: padding

        width: 10
        height: parent.height
        anchors {
            left: parent.left
            top: parent.top
        }

        color: 'transparent'
    }

    Text {
        id: innerText

        width: parent.width-padding.width*2
        anchors.centerIn: parent

        color: 'white'
        font.family: 'Verdana'
        font.pixelSize: 16
        font.bold: true
        minimumPixelSize: 8
        fontSizeMode: Text.Fit
        visible: false
    }

    LinearGradient {
        anchors.fill: innerText
        source: innerText
        gradient: Gradient {
            GradientStop { position: 0; color: version===activeVersion ? '#AED6F1' : '#F9E79F' }
            GradientStop { position: 1; color: version===activeVersion ? '#13A2FF' : '#F4D03F' }
        }
    }
}
