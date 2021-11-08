import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Layouts 1.12

import '../Style'

//! @brief StatsView
//'' Select style, active version from combobox (not in this view)
//'' display score analysis
//'' input ui:
//'' Statistics select by Level/Difficulty
//'' o Level
//'' o Difficulty groupbox { o AllVersion o Version cbox [version]
//'' create grid buttons below.
//'' inside Statistics, select column: clear type, djlevel, score level.
Item {
    property alias tableView: tableView

    ColumnLayout {
        width: parent.width
        height: parent.height

        RowLayout {
            RadioButton {
                checked: true
                text: 'Level'
            }
            RadioButton {
                text: 'All version difficulty'
            }
            RadioButton {
                text: 'Difficulty by version'
            }
            StyledComboBox {
                Layout.preferredWidth: 200
                Layout.preferredHeight: 50
            }
        }

        RowLayout {
            RadioButton {
                checked: true
                text: 'Clear'
            }
            RadioButton {
                text: 'DJ Level'
            }
            RadioButton {
                text: 'Score Level'
            }
        }

        Item {
            Layout.fillWidth: true
            Layout.fillHeight: true
            Layout.preferredHeight: 400

            TableView {
                id: tableView

                topMargin: horizontalHeader.implicitHeight
                leftMargin: verticalHeader.implicitWidth
                anchors.fill: parent

                delegate: Rectangle {
                    implicitWidth: 100
                    implicitHeight: 50
                    Text {
                        text: display
                    }
                }
            }

            HorizontalHeaderView {
                id: horizontalHeader
                syncView: tableView
                anchors.left: tableView.left
            }

            VerticalHeaderView {
                id: verticalHeader
                syncView: tableView
                anchors.top: tableView.top
            }
        }
    }
}


