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
    property alias comboBoxDifficultyVersion: comboBoxDifficultyVersion
    property string tableType: 'Level'
    property string columnType: 'Clear'
    property string valueType: 'Count'

    signal optionChanged()

    ColumnLayout {
        width: parent.width
        height: parent.height

        RowLayout {
            RadioButton {
                checked: true
                text: 'Level'

                onCheckedChanged: {
                    if (checked)
                    {
                        tableType = text
                        optionChanged()
                    }
                }
            }
            RadioButton {
                text: 'All Difficulty'

                onCheckedChanged: {
                    if (checked)
                    {
                        tableType = 'AllDifficulty'
                        optionChanged()
                    }
                }
            }
            RadioButton {
                text: 'Difficulty by Version'

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
                Layout.preferredWidth: 100
                Layout.preferredHeight: 50

                onActivated: {
                    optionChanged()
                }
            }
        }

        RowLayout {
            RadioButton {
                checked: true
                text: 'Clear'

                onCheckedChanged: {
                    if (checked)
                    {
                        columnType = text
                        optionChanged()
                    }
                }
            }
            RadioButton {
                text: 'DJ Level'

                onCheckedChanged: {
                    if (checked)
                    {
                        columnType = 'DjLevel'
                        optionChanged()
                    }
                }
            }
            RadioButton {
                text: 'Score Level'

                onCheckedChanged: {
                    if (checked)
                    {
                        columnType = 'ScoreLevel'
                        optionChanged()
                    }
                }
            }
        }

        RowLayout {
            RadioButton {
                checked: true
                text: 'Count'

                onCheckedChanged: {
                    if (checked)
                    {
                        valueType = text
                        optionChanged()
                    }
                }
            }
            RadioButton {
                text: 'Percentage'

                onCheckedChanged: {
                    if (checked)
                    {
                        valueType = text
                        optionChanged()
                    }
                }
            }
        }

        Item {
            Layout.fillWidth: true
            Layout.fillHeight: true
            Layout.preferredHeight: 400

            TableView {
                id: tableView

                topMargin: horizontalHeader.implicitHeight
                leftMargin: verticalHeader.implicitWidth+25
                anchors.fill: parent

                delegate: Rectangle {
                    implicitWidth: 70
                    implicitHeight: 30
                    Text {
                        anchors.centerIn: parent
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

                implicitWidth: 30
            }
        }
    }
}
