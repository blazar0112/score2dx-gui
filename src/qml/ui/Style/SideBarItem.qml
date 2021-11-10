import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Controls.Styles 1.4
import QtQuick.Layouts 1.12

Rectangle {
    id: root
    property alias gridLayout: gridLayout
    default property alias content: gridLayout.children
    property alias title: titleText.text

    implicitHeight: barSection.expanded ? barSection.height+item.implicitHeight : barSection.height

    color: 'transparent'

    Rectangle {
        id: barSection
        width: parent.width
        height: 30
        property bool expanded: true

        gradient: Gradient {
            orientation: Gradient.Horizontal
            GradientStop { position: 0.0; color: "white" }
            GradientStop { position: 1.0; color: "#512E5F" }
        }

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

                font.family: 'Verdana'
                font.pixelSize: 20
                color: 'black'
            }
        }

        MouseArea {
            anchors.fill: parent
            onClicked: {
                barSection.expanded = !barSection.expanded
            }
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


