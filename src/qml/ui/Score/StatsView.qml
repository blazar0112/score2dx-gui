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

        GridLayout {
            id: grid
            columns: 2
            rowSpacing: 5
            columnSpacing: 5
            anchors.margins: 5
            // example models
            property var titles: [ "title1", "title2", "title3", "title4", "title5" ]
            property var values: [ "value1", "value2", "value3", "value4", "value5" ]

            Repeater {
                model: grid.titles
                Label {
                    Layout.row: index
                    Layout.column: 0
                    Layout.fillWidth: true
                    Layout.fillHeight: true
                    text: modelData
                }
            }

            Repeater {
                model: grid.values
                TextArea {
                    Layout.row: index
                    Layout.column: 1
                    Layout.fillWidth: true
                    Layout.fillHeight: true
                    text: modelData
                }
            }
        }
    }
}


