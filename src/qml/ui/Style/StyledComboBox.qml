import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Controls.Styles 1.4

Row {
    //'' must set property, alias for convenience.
    property alias model: comboBox.model
    property alias currentText: comboBox.currentText
    property alias comboBox: comboBox
    //'' user can set preferred initial text item.
    property string initialText: ''

    signal activated(int index)

    ComboBox {
        id: comboBox

        implicitWidth: parent.width
        implicitHeight: parent.height
        anchors.verticalCenter: parent.verticalCenter

        font.pointSize: 12
        font.family: 'Verdana'

        background: Rectangle {
            color: '#B2D2A4'
            radius: 5
        }

        delegate: ItemDelegate {
            width: comboBox.width
            text: comboBox.textRole ? (Array.isArray(comboBox.model) ? modelData[comboBox.textRole] : model[comboBox.textRole]) : modelData
            font.family: comboBox.font.family
            font.pointSize: comboBox.font.pointSize

            //highlighted: comboBox.highlightedIndex === comboBox.index
            hoverEnabled: comboBox.hoverEnabled

            background: Rectangle {
                color: parent.hovered ? '#32CD30' : '#B2D2A4'
            }
        }

        popup: Popup {
            y: comboBox.height - 1
            width: comboBox.width
            implicitHeight: Math.min(300, contentItem.implicitHeight)
            padding: 1

            contentItem: ListView {
                clip: true
                implicitHeight: contentHeight
                model: comboBox.popup.visible ? comboBox.delegateModel : null
                currentIndex: comboBox.highlightedIndex

                ScrollIndicator.vertical: ScrollIndicator { }
            }

            background: Rectangle {
                border.color: "#21be2b"
                radius: 2
            }
        }

        onActivated: {
            parent.activated(index)
        }

        Component.onCompleted: {
            if (model)
            {
                //console.log('try find initial', initialText)
                var i = find(initialText)
                if (i!==-1)
                {
                    currentIndex = i
                }
            }
            //console.log('StyledComboBox onCompleted set to', currentText)
            activated(currentIndex)
        }
    }
}


