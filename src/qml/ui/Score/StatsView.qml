import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Layouts 1.12
import QtGraphicalEffects 1.0

import '../Style'
import '.'

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
    property alias musicListHeader: musicListHeader
    property alias musicList: musicList
    property alias musicListFilterRepeater: musicListFilterRepeater
    property alias comboBoxDifficultyVersion: comboBoxDifficultyVersion
    property string tableType: 'Level'
    property string columnType: 'Clear'
    property string valueType: 'Count'
    property string activeVersion: ''

    signal optionChanged()
    signal cellClicked(int row, int column)

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
            id: rectMusicListSection
            property bool expanded: true

            implicitWidth: musicListHeader.implicitWidth
            implicitHeight: musicList.model.rowItemCount===0 ? 0 : 40
            Layout.alignment: Qt.AlignHCenter
            visible: musicList.model.rowItemCount!==0

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

                    text: 'Music List'
                    font: fontMetrics.font
                    color: 'black'
                    horizontalAlignment: Text.AlignHCenter
                    verticalAlignment: Text.AlignVCenter
                }

                Row {
                    Layout.alignment: Qt.AlignHCenter

                    Repeater {
                        id: musicListFilterRepeater
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
                    rectMusicListSection.expanded = !rectMusicListSection.expanded
                }
            }
        }

        ListView {
            id: musicListHeader

            width: contentItem.childrenRect.width
            height: contentItem.childrenRect.height
            implicitWidth: width
            implicitHeight: height
            Layout.alignment: Qt.AlignHCenter
            visible: musicList.model.rowItemCount!==0

            delegate: Row {
                StatsMusicHeader {
                    width: 60
                    text: model.version
                }

                StatsMusicHeader {
                    width: 20
                    text: model.clear
                }

                StatsMusicHeader {
                    width: 60
                    text: model.level
                }

                StatsMusicHeader {
                    width: 300
                    text: model.title
                }

                StatsMusicHeader {
                    width: 80
                    text: model.djLevel
                }

                StatsMusicHeader {
                    width: 100
                    text: model.score
                }

                StatsMusicHeader {
                    width: 100
                    text: model.bestScoreDiff
                }

                StatsMusicHeader {
                    width: 60
                    text: model.careerBestVersion
                }

                StatsMusicHeader {
                    width: 100
                    text: model.careerBestScore
                }
            }
        }

        ListView {
            id: musicList

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
                    width: 60
                    height: musicList.rowHeight
                    border.color: 'black'
                    color: '#34495E'
                    Text {
                        anchors.centerIn: parent
                        text: model.version
                        font: fontMetrics.font
                        color: 'white'
                    }
                }

                Rectangle {
                    id: rectClear
                    width: 20
                    height: musicList.rowHeight
                    border.color: 'black'
                    color: 'white'

                    states: [
                        State {
                            name: 'No Play'
                            when: model.clear==='NO PLAY'
                            PropertyChanges { target: rectClear; color: '#17202A' }
                        },
                        State {
                            name: 'Fail'
                            when: model.clear==='FAILED'
                            PropertyChanges { target: failAnimation; running: true }
                        },
                        State {
                            name: 'Assist'
                            when: model.clear==='ASSIST'
                            PropertyChanges { target: rectClear; color: '#BB8FCE' }
                        },
                        State {
                            name: 'Easy'
                            when: model.clear==='EASY'
                            PropertyChanges { target: rectClear; color: '#58D68D' }
                        },
                        State {
                            name: 'Clear'
                            when: model.clear==='CLEAR'
                            PropertyChanges {
                                target: rectClear
                                color: model.difficulty==='L' ? '#f500ff'
                                       : model.difficulty==='A' ? 'red'
                                       : model.difficulty==='H' ? '#ffb746'
                                       : '#3498DB'
                            }
                        },
                        State {
                            name: 'Hard'
                            when: model.clear==='HARD'
                            PropertyChanges { target: rectClear; color: 'white' }

                        },
                        State {
                            name: 'EX'
                            when: model.clear==='EX HARD'
                            PropertyChanges { target: exAnimation; running: true }
                        },
                        State {
                            name: 'FC'
                            when: model.clear==='FC'
                            PropertyChanges { target: fcAnimation; running: true }
                        }
                    ]

                    SequentialAnimation on color {
                        id: fcAnimation
                        running: false
                        loops: Animation.Infinite
                        ColorAnimation { from: 'white'; to: 'cyan'; duration: 150 }
                        ColorAnimation { from: 'cyan'; to: 'white'; duration: 150 }
                        ColorAnimation { from: 'white'; to: 'yellow'; duration: 150 }
                        //ColorAnimation { from: 'cyan'; to: 'yellow'; duration: 50 }
                        //ColorAnimation { from: 'yellow'; to: 'cyan'; duration: 50 }
                        //ColorAnimation { from: 'cyan'; to: 'white'; duration: 100 }
                    }

                    SequentialAnimation on color {
                        id: exAnimation
                        running: false
                        loops: Animation.Infinite
                        ColorAnimation { from: 'red'; to: 'yellow'; duration: 150 }
                        ColorAnimation { from: 'yellow'; to: 'white'; duration: 150 }
                        ColorAnimation { from: 'white'; to: 'red'; duration: 150 }
                    }

                    SequentialAnimation on color {
                        id: failAnimation
                        running: false
                        loops: Animation.Infinite
                        ColorAnimation { from: '#1C2833'; to: '#A93226'; duration: 150 }
                        ColorAnimation { from: '#A93226'; to: '#1C2833'; duration: 150 }
                    }
                }

                Rectangle {
                    width: 60
                    height: musicList.rowHeight
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

                Rectangle {
                    width: 300
                    height: musicList.rowHeight
                    implicitWidth: width
                    implicitHeight: height
                    border.color: 'black'
                    color: '#34495E'

                    Rectangle {
                        id: rectPadding
                        width: 10
                        height: parent.height
                        anchors {
                            left: parent.left
                            top: parent.top
                        }
                        color: 'transparent'
                    }

                    Text {
                        id: titleText
                        width: parent.width-rectPadding.width*2
                        anchors.centerIn: parent

                        text: model.title
                        color: 'white'
                        font: fontMetrics.font
                        minimumPixelSize: 8
                        fontSizeMode: Text.Fit
                        //wrapMode: Text.WrapAnywhere
                        //renderType: Text.NativeRendering
                        visible: false
                    }

                    LinearGradient {
                        anchors.fill: titleText
                        source: titleText
                        gradient: Gradient {
                            GradientStop { position: 0; color: model.version===activeVersion ? '#AED6F1' : '#F9E79F' }
                            GradientStop { position: 1; color: model.version===activeVersion ? '#13A2FF' : '#F4D03F' }
                        }
                    }
                }

                Rectangle {
                    width: 80
                    height: musicList.rowHeight
                    border.color: 'black'
                    color: '#34495E'

                    Text {
                        anchors.centerIn: parent
                        text: model.djLevel
                        font: fontMetrics.font
                        color: 'white'
                    }
                }

                Rectangle {
                    width: 100
                    height: musicList.rowHeight
                    border.color: 'black'
                    color: '#34495E'
                    Text {
                        anchors.centerIn: parent
                        text: model.score
                        font: fontMetrics.font
                        color: 'white'
                    }
                }

                Rectangle {
                    width: 100
                    height: musicList.rowHeight
                    border.color: 'black'
                    color: '#34495E'
                    Text {
                        anchors.centerIn: parent
                        text: model.bestScoreDiff
                        font: fontMetrics.font
                        color: model.bestScoreDiff==='PB' ? '#2ECC71'
                               : model.bestScoreDiff==='NP' ? 'yellow'
                               : model.bestScoreDiff==='N/A' ? 'white'
                               : 'red'
                    }
                }

                Rectangle {
                    width: 60
                    height: musicList.rowHeight
                    border.color: 'black'
                    color: '#34495E'
                    Text {
                        anchors.centerIn: parent
                        text: model.careerBestVersion
                        font: fontMetrics.font
                        color: 'white'
                    }
                }

                Rectangle {
                    width: 100
                    height: musicList.rowHeight
                    border.color: 'black'
                    color: '#34495E'
                    Text {
                        anchors.centerIn: parent
                        text: model.careerBestScore
                        font: fontMetrics.font
                        color: 'white'
                    }
                }
            }
        }
    }
}
