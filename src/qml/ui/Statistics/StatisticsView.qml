import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Layouts 1.12
import QtGraphicalEffects 1.0

import '../Score'
import '../Statistics'
import '../Style'

Item {
    property alias horizontalHeaderView: horizontalHeaderView
    property alias verticalHeaderView: verticalHeaderView
    property alias tableView: tableView
    property alias chartListHeader: chartListHeader
    property alias chartList: chartList
    property alias chartListFilterRepeater: chartListFilterRepeater
    property alias comboBoxDifficultyVersion: comboBoxDifficultyVersion
    property string tableType: 'Level'
    property string columnType: 'Clear'
    property string valueType: 'Count'
    property string activeVersion: ''

    signal optionChanged()
    signal cellClicked(int row, int column)

    //'' Ver, C, Lv, Title, DjLevel, Score, RangeDiff, Miss, PDBS Diff, PDBS Ver, PDB Score, PDBM Diff, PDBM Ver, PDB Miss
    //'' 40, 20, 40,   240,      50,    60,        80,   50,        60,       40,        60,        50,       40,       50
    //'' better sum = 880 for same width as stats table.
    readonly property var headerWidths: [40, 20, 40, 240, 50, 60, 80, 50, 60, 40, 60, 50, 40, 50]

    ButtonGroup { id: buttonGroupTableType }
    ButtonGroup { id: buttonGroupColumnType }
    ButtonGroup { id: buttonGroupValueType }

    FontMetrics {
        id: fontMetrics
        font.family: 'Verdana'
        font.pixelSize: 16
        font.bold: true
    }

    ColumnLayout {
        width: parent.width
        height: parent.height
        anchors.horizontalCenter: parent.horizontalCenter

        spacing: 0

        GridLayout {
            Layout.preferredHeight: 90
            Layout.alignment: Qt.AlignHCenter

            columns: 5
            rows: 3

            columnSpacing: 0
            rowSpacing: 0

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
                Layout.maximumHeight: 30
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
                Layout.maximumHeight: 30
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
                Layout.maximumHeight: 30
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
                Layout.preferredHeight: 30

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
                Layout.maximumHeight: 30
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
                Layout.maximumHeight: 30
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
                Layout.maximumHeight: 30
                text: 'Score Level Category'
                font: fontMetrics.font
                ButtonGroup.group: buttonGroupColumnType

                onCheckedChanged: {
                    if (checked)
                    {
                        columnType = 'ScoreLevelCategory'
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
                Layout.maximumHeight: 30
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
                Layout.maximumHeight: 30
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

        Rectangle {
            Layout.fillWidth: true
            height: 5
            color: 'transparent'
        }

        Rectangle {
            id: rectTableSection
            property bool expanded: true

            implicitWidth: horizontalHeaderView.contentWidth+verticalHeaderView.contentWidth
            implicitHeight: gridLayoutTable.implicitHeight ? 30 : 0
            Layout.alignment: Qt.AlignHCenter
            visible: tableView.model.rowItemCount!==0

            gradient: Gradient {
                orientation: Gradient.Horizontal
                GradientStop { position: 0.0; color: 'white' }
                GradientStop { position: 0.125; color: '#D7BDE2' }
                GradientStop { position: 1.0; color: '#512E5F' }
            }

            Image {
                id: image
                width: 20
                height: 20
                x: verticalHeaderView.x+verticalHeaderView.width/2-width/2
                anchors.verticalCenter: parent.verticalCenter
                source: rectTableSection.expanded ? 'qrc:/qml/image/sidebar_expanded.png' : 'qrc:/qml/image/sidebar_collapsed.png'
            }

            Text {
                anchors.centerIn: parent
                text: 'Statistics Table'
                font: fontMetrics.font
                color: 'black'
            }

            MouseArea {
                anchors.fill: parent
                onClicked: {
                    rectTableSection.expanded = !rectTableSection.expanded
                }
            }
        }

        GridLayout {
            id: gridLayoutTable

            implicitWidth: horizontalHeaderView.contentWidth+verticalHeaderView.contentWidth
            implicitHeight: 0
            Layout.fillWidth: true
            Layout.minimumWidth: 10
            Layout.alignment: Qt.AlignHCenter

            visible: rectTableSection.expanded

            rows: 2
            columns: 2
            rowSpacing: 0
            columnSpacing: 0

            Rectangle {
                implicitWidth: verticalHeaderView.contentWidth
                implicitHeight: horizontalHeaderView.contentHeight

                gradient: Gradient {
                    orientation: Gradient.Horizontal
                    GradientStop { position: 0.0; color: 'white' }
                    GradientStop { position: 1; color: '#D7BDE2' }
                }
            }

            TableView {
                id: horizontalHeaderView
                Layout.row: 0
                Layout.column: 1
                implicitWidth: contentWidth
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
                implicitWidth: contentWidth
                implicitHeight: contentHeight

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
                implicitWidth: contentWidth
                implicitHeight: contentHeight

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

                    MouseArea {
                        anchors.fill: parent
                        onClicked: {
                            cellClicked(model.row, model.column)
                        }
                    }
                }
            }
        }

        Rectangle {
            Layout.fillWidth: true
            height: 5
            color: 'transparent'
        }

        Rectangle {
            id: rectChartListSection
            property bool expanded: true

            implicitWidth: chartListHeader.implicitWidth
            implicitHeight: chartList.model.rowItemCount===0 ? 0 : 40
            Layout.alignment: Qt.AlignHCenter
            visible: chartList.model.rowItemCount!==0

            gradient: Gradient {
                orientation: Gradient.Horizontal
                GradientStop { position: 0.0; color: 'white' }
                GradientStop { position: 0.125; color: '#D7BDE2' }
                GradientStop { position: 1.0; color: '#512E5F' }
            }

            ColumnLayout {
                anchors.fill: parent

                Text {
                    Layout.fillWidth: true

                    text: 'Chart List'
                    font: fontMetrics.font
                    color: 'black'
                    horizontalAlignment: Text.AlignHCenter
                    verticalAlignment: Text.AlignVCenter
                }

                Row {
                    Layout.alignment: Qt.AlignHCenter

                    Repeater {
                        id: chartListFilterRepeater

                        Rectangle {
                            implicitWidth: filterText.width+10
                            height: filterText.height
                            border.color: 'black'
                            color: '#58D68D'
                            radius: 5

                            Text {
                                id: filterText
                                anchors.centerIn: parent

                                text: modelData
                                font.family: 'Verdana'
                                font.pixelSize: 12
                                color: 'black'

                                horizontalAlignment: Text.AlignHCenter
                                verticalAlignment: Text.AlignVCenter
                            }
                        }
                    }
                }
            }

            MouseArea {
                anchors.fill: parent
                onClicked: {
                    rectChartListSection.expanded = !rectChartListSection.expanded
                }
            }
        }

        ListView {
            id: chartListHeader

            width: contentItem.childrenRect.width
            height: contentItem.childrenRect.height
            implicitWidth: width
            implicitHeight: height
            Layout.alignment: Qt.AlignHCenter
            visible: chartList.model.rowItemCount!==0

            delegate: Row {
                StatisticsChartListHeader {
                    width: headerWidths[0]
                    innerText.text: model.version
                }

                StatisticsChartListHeader {
                    width: headerWidths[1]
                    innerText.text: model.clear
                }

                StatisticsChartListHeader {
                    width: headerWidths[2]
                    innerText.text: model.level
                }

                StatisticsChartListHeader {
                    width: headerWidths[3]
                    innerText.text: model.title
                }

                StatisticsChartListHeader {
                    width: headerWidths[4]
                    innerText.text: model.djLevel
                    innerText.font.pixelSize: 14
                }

                StatisticsChartListHeader {
                    width: headerWidths[5]
                    innerText.text: model.score
                }

                StatisticsChartListHeader {
                    width: headerWidths[6]
                    innerText.text: model.scoreLevelDiff
                }

                StatisticsChartListHeader {
                    width: headerWidths[7]
                    innerText.text: model.miss
                }

                StatisticsChartListHeader {
                    width: headerWidths[8]
                    innerText.text: model.careerDiffableBestScoreDiff
                    innerText.font.pixelSize: 12
                }

                StatisticsChartListHeader {
                    width: headerWidths[9]
                    innerText.text: model.careerDiffableBestScoreVersion
                    innerText.font.pixelSize: 12
                }

                StatisticsChartListHeader {
                    width: headerWidths[10]
                    innerText.text: model.careerDiffableBestScore
                    innerText.font.pixelSize: 14
                }

                StatisticsChartListHeader {
                    width: headerWidths[11]
                    innerText.text: model.careerDiffableBestMissDiff
                    innerText.font.pixelSize: 12
                }

                StatisticsChartListHeader {
                    width: headerWidths[12]
                    innerText.text: model.careerDiffableBestMissVersion
                    innerText.font.pixelSize: 12
                }

                StatisticsChartListHeader {
                    width: headerWidths[13]
                    innerText.text: model.careerDiffableBestMiss
                    innerText.font.pixelSize: 14
                }
            }
        }

        ListView {
            id: chartList

            readonly property int rowHeight: 40

            width: contentItem.childrenRect.width
            implicitWidth: width

            Layout.fillHeight: true
            Layout.alignment: Qt.AlignHCenter

            clip: true
            cacheBuffer: 40*rowHeight

            ScrollBar.vertical: ScrollBar {
                active: true
                width: 20
            }

            delegate: Row {

                Rectangle {
                    width: headerWidths[0]
                    height: chartList.rowHeight
                    border.color: 'black'
                    color: '#34495E'
                    Text {
                        anchors.centerIn: parent
                        text: model.version
                        font: fontMetrics.font
                        color: 'white'
                    }
                }

                ClearLampRectangle {
                    width: headerWidths[1]
                    height: chartList.rowHeight
                    clear: model.clear
                    difficulty: model.difficulty
                }

                Rectangle {
                    width: headerWidths[2]
                    height: chartList.rowHeight
                    border.color: 'black'
                    color: '#34495E'

                    Text {
                        anchors.centerIn: parent
                        text: model.level
                        font.family: 'Verdana'
                        font.pixelSize: 20
                        font.bold: true
                        style: Text.Outline
                        color:  model.difficulty==='L' ? '#f500ff'
                                : model.difficulty==='A' ? 'red'
                                : model.difficulty==='H' ? '#ffb746'
                                : model.difficulty==='N' ? '#86cfff'
                                : 'white'
                    }
                }

                TitleRectangle {
                    width: headerWidths[3]
                    height: chartList.rowHeight

                    innerText.text: model.title
                }

                DjLevelRectangle {
                    width: headerWidths[4]
                    height: chartList.rowHeight
                    innerText.text: model.djLevel
                    innerText.font.pixelSize: 16
                }

                Rectangle {
                    width: headerWidths[5]
                    height: chartList.rowHeight
                    border.color: 'black'
                    color: '#34495E'

                    Text {
                        anchors.centerIn: parent
                        text: model.score
                        font: fontMetrics.font
                        color: 'white'
                    }
                }

                ScoreLevelCategoryRectangle {
                    width: headerWidths[6]
                    height: chartList.rowHeight
                    innerText.text: model.scoreLevelDiff
                }

                Rectangle {
                    width: headerWidths[7]
                    height: chartList.rowHeight
                    border.color: 'black'
                    color: '#34495E'

                    Text {
                        anchors.centerIn: parent
                        text: model.miss
                        font.family: 'Verdana'
                        font.pixelSize: model.miss==='N/A' ? 12 : 16
                        font.bold: true
                        color: model.miss==='N/A' ? 'gray' : 'white'
                    }
                }

                Rectangle {
                    width: headerWidths[8]
                    height: chartList.rowHeight
                    border.color: 'black'
                    color: '#34495E'

                    Text {
                        anchors.centerIn: parent
                        text: model.careerDiffableBestScoreDiff
                        font: fontMetrics.font
                        color: model.careerDiffableBestScoreDiff==='PB' ? 'cyan'
                               : model.careerDiffableBestScoreDiff==='NP' ? 'yellow'
                               : model.careerDiffableBestScoreDiff.startsWith('+') ? '#58D68D'
                               : model.careerDiffableBestScoreDiff==='0' ? 'white'
                               : 'red'
                    }
                }

                Rectangle {
                    width: headerWidths[9]
                    height: chartList.rowHeight
                    border.color: 'black'
                    color: '#34495E'

                    Text {
                        anchors.centerIn: parent
                        text: model.careerDiffableBestScoreVersion
                        font.family: 'Verdana'
                        font.pixelSize: model.careerDiffableBestScoreVersion==='N/A' ? 12 : 16
                        font.bold: true
                        color: model.careerDiffableBestScoreVersion==='N/A' ? 'gray' : 'white'
                    }
                }

                Rectangle {
                    width: headerWidths[10]
                    height: chartList.rowHeight
                    border.color: 'black'
                    color: '#34495E'

                    Text {
                        anchors.centerIn: parent
                        text: model.careerDiffableBestScore
                        font.family: 'Verdana'
                        font.pixelSize: model.careerDiffableBestScore==='N/A' ? 12 : 16
                        font.bold: true
                        color: model.careerDiffableBestScore==='N/A' ? 'gray' : 'white'
                    }
                }

                Rectangle {
                    width: headerWidths[11]
                    height: chartList.rowHeight
                    border.color: 'black'
                    color: '#34495E'

                    Text {
                        anchors.centerIn: parent
                        text: model.careerDiffableBestMissDiff
                        font.family: 'Verdana'
                        font.pixelSize: model.careerDiffableBestMissDiff==='N/A' ? 12 : 16
                        font.bold: true
                        color: model.careerDiffableBestMissDiff==='PB' ? 'cyan'
                               : model.careerDiffableBestMissDiff==='N/A' ? 'gray'
                               : model.careerDiffableBestMissDiff.startsWith('+') ? 'red'
                               : model.careerDiffableBestMissDiff==='0' ? 'white'
                               : '#58D68D'
                    }
                }

                Rectangle {
                    width: headerWidths[12]
                    height: chartList.rowHeight
                    border.color: 'black'
                    color: '#34495E'

                    Text {
                        anchors.centerIn: parent
                        text: model.careerDiffableBestMissVersion
                        font.family: 'Verdana'
                        font.pixelSize: model.careerDiffableBestMissVersion==='N/A' ? 12 : 16
                        font.bold: true
                        color: model.careerDiffableBestMissVersion==='N/A' ? 'gray' : 'white'
                    }
                }

                Rectangle {
                    width: headerWidths[13]
                    height: chartList.rowHeight
                    border.color: 'black'
                    color: '#34495E'

                    Text {
                        anchors.centerIn: parent
                        text: model.careerDiffableBestMiss
                        font.family: 'Verdana'
                        font.pixelSize: model.careerDiffableBestMiss==='N/A' ? 12 : 16
                        font.bold: true
                        color: model.careerDiffableBestMiss==='N/A' ? 'gray' : 'white'
                    }
                }
            }
        }
    }
}
