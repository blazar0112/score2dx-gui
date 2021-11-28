import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Controls.Styles 1.4
import QtQuick.Layouts 1.12

import '../Style'

Rectangle {
    id: root
    property alias gridLayout: gridLayout
    default property alias content: gridLayout.children
    property alias title: titleText.text

    implicitHeight: barSection.expanded ? barSection.height+item.implicitHeight : barSection.height

    color: 'transparent'

    SectionRectangle {
        id: barSection

        property bool expanded: true

        width: parent.width
        height: 30
        innerText.text: ''

        Row {
            Rectangle {
                width: 5
                height: 1
                color: 'transparent'
            }

            Image {
                id: image
                width: 20
                height: 20
                anchors.verticalCenter: parent.verticalCenter
                source: barSection.expanded ? 'qrc:/qml/image/sidebar_expanded.png' : 'qrc:/qml/image/sidebar_collapsed.png'
            }

            Rectangle {
                width: 5
                height: 1
                color: 'transparent'
            }

            Text {
                id: titleText
            }
        }

        MouseArea {
            anchors.fill: parent
            onClicked: {
                barSection.expanded = !barSection.expanded
            }
        }

        Component.onCompleted: {
            titleText.font.family = barSection.innerText.font.family
            titleText.font.pixelSize = barSection.innerText.font.pixelSize
            titleText.color = barSection.innerText.color
        }
    }

    Rectangle {
        id: item

        width: parent.width
        implicitHeight: gridLayout.implicitHeight

        anchors {
            top: barSection.bottom
            left: parent.left
        }

        color: 'transparent'
        visible: barSection.expanded

        GridLayout {
            id: gridLayout
            anchors.fill: parent

            columnSpacing: 0
            rowSpacing: 0
        }
    }
}


