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
    property alias horizontalHeaderView: horizontalHeaderView
    property alias verticalHeaderView: verticalHeaderView
    property alias tableView: tableView
    property alias comboBoxDifficultyVersion: comboBoxDifficultyVersion
    property string tableType: 'Level'
    property string columnType: 'Clear'
    property string valueType: 'Count'

    signal optionChanged()

    ButtonGroup { id: buttonGroupTableType }
    ButtonGroup { id: buttonGroupColumnType }
    ButtonGroup { id: buttonGroupValueType }

    FontMetrics {
        id: fontMetrics
        font.family: 'Verdana'
        font.pixelSize: 16
    }

    ColumnLayout {
        width: parent.width
        height: parent.height

        GridLayout {
            columns: 5
            rows: 3

            // row 0:

            Text {
                Layout.row: 0
                Layout.column: 0
                text: 'Table'
                font: fontMetrics.font
            }

            RadioButton {
                Layout.row: 0
                Layout.column: 1
                checked: true
                text: 'Level'
                font: fontMetrics.font
                ButtonGroup.group: buttonGroupTableType

                onCheckedChanged: {
                    if (checked)
                    {
                        tableType = text
                        optionChanged()
                    }
                }
            }

            RadioButton {
                Layout.row: 0
                Layout.column: 2
                text: 'All Difficulty'
                font: fontMetrics.font
                ButtonGroup.group: buttonGroupTableType

                onCheckedChanged: {
                    if (checked)
                    {
                        tableType = 'AllDifficulty'
                        optionChanged()
                    }
                }
            }

            RadioButton {
                Layout.row: 0
                Layout.column: 3
                text: 'Difficulty by Version'
                font: fontMetrics.font
                ButtonGroup.group: buttonGroupTableType

                onCheckedChanged: {
                    if (checked)
                    {
                        tableType = 'VersionDifficulty'
                        optionChanged()
                    }
                }
            }

            StyledComboBox {
                id: comboBoxDifficultyVersion

                Layout.row: 0
                Layout.column: 4
                Layout.preferredWidth: 100
                Layout.preferredHeight: 50

                onActivated: {
                    optionChanged()
                }
            }

            // row 1:

            Text {
                Layout.row: 1
                Layout.column: 0
                text: 'Column'
                font: fontMetrics.font
            }

            RadioButton {
                Layout.row: 1
                Layout.column: 1
                checked: true
                text: 'Clear'
                font: fontMetrics.font
                ButtonGroup.group: buttonGroupColumnType

                onCheckedChanged: {
                    if (checked)
                    {
                        columnType = text
                        optionChanged()
                    }
                }
            }

            RadioButton {
                Layout.row: 1
                Layout.column: 2
                text: 'DJ Level'
                font: fontMetrics.font
                ButtonGroup.group: buttonGroupColumnType

                onCheckedChanged: {
                    if (checked)
                    {
                        columnType = 'DjLevel'
                        optionChanged()
                    }
                }
            }

            RadioButton {
                Layout.row: 1
                Layout.column: 3
                text: 'Score Level'
                font: fontMetrics.font
                ButtonGroup.group: buttonGroupColumnType

                onCheckedChanged: {
                    if (checked)
                    {
                        columnType = 'ScoreLevel'
                        optionChanged()
                    }
                }
            }

            // row 2:

            Text {
                Layout.row: 2
                Layout.column: 0
                text: 'Value'
                font: fontMetrics.font
            }

            RadioButton {
                Layout.row: 2
                Layout.column: 1
                checked: true
                text: 'Count'
                font: fontMetrics.font
                ButtonGroup.group: buttonGroupValueType

                onCheckedChanged: {
                    if (checked)
                    {
                        valueType = text
                        optionChanged()
                    }
                }
            }

            RadioButton {
                Layout.row: 2
                Layout.column: 2
                text: 'Percentage'
                font: fontMetrics.font
                ButtonGroup.group: buttonGroupValueType

                onCheckedChanged: {
                    if (checked)
                    {
                        valueType = text
                        optionChanged()
                    }
                }
            }
        }

        GridLayout {
            rows: 2
            columns: 2
            rowSpacing: 1
            columnSpacing: 1
            Layout.fillWidth: true
            Layout.fillHeight: true

            Rectangle {
                Layout.row: 0
                Layout.column: 0
            }

            TableView {
                id: horizontalHeaderView
                Layout.row: 0
                Layout.column: 1
                Layout.fillWidth: true
                Layout.preferredHeight: 30

                delegate: Rectangle {
                    implicitWidth: 80
                    implicitHeight: 30
                    border.color: 'black'
                    color: model.background
                    Text {
                        anchors.centerIn: parent
                        text: display
                        font: fontMetrics.font
                        color: model.foreground
                    }
                }
            }

            TableView {
                id: verticalHeaderView
                Layout.row: 1
                Layout.column: 0
                Layout.preferredWidth: 80
                Layout.fillHeight: true

                delegate: Rectangle {
                    implicitWidth: 80
                    implicitHeight: 30
                    border.color: 'black'
                    color: model.background
                    Text {
                        anchors.centerIn: parent
                        text: display
                        font: fontMetrics.font
                        color: model.foreground
                    }
                }
            }

            TableView {
                id: tableView
                Layout.row: 1
                Layout.column: 1
                Layout.fillWidth: true
                Layout.fillHeight: true

                delegate: Rectangle {
                    implicitWidth: 80
                    implicitHeight: 30
                    border.color: 'black'
                    color: model.background
                    Text {
                        anchors.centerIn: parent
                        text: display
                        font: fontMetrics.font
                        color: model.foreground
                    }
                }
            }
        }
    }
}
