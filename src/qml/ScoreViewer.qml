import QtQuick 2.15
import QtCharts 2.3
import QtQuick.Controls 2.15
import QtQuick.Dialogs 1.0
import QtQuick.Layouts 1.12
import QtQuick.Window 2.0

import '../qml/ui/Score'
import '../qml/ui/Style'

ApplicationWindow
{
    id: window
    visible: true
    color: '#b7bdfb'
    width: 1200
    height: 900
    opacity: 1
    title: 'Score Viewer '+core.getScore2dxVersion()

    FontMetrics {
        id: fontMetrics
        font.family: 'Verdana'
        font.pixelSize: 16
    }

    RowLayout {
        anchors.fill: parent

        Rectangle {
            width: 1
        }

        GridLayout {
            columns: 6
            rows: 10
            Layout.maximumWidth: 300
            Layout.alignment: Qt.AlignTop

            Text {
                Layout.row: 0
                Layout.columnSpan: 2
                Layout.alignment: Qt.AlignVCenter

                text: 'IIDX ID'
                font: fontMetrics.font
            }

            StyledComboBox {
                id: comboBoxPlayer

                Layout.row: 0
                Layout.column: 2
                Layout.columnSpan: 4
                Layout.fillWidth: true
                Layout.preferredHeight: 50

                model: core.playerList

                onActivated: {
                    updatePlayer()
                }
            }

            Text {
                Layout.row: 1
                Layout.columnSpan: 2
                Layout.alignment: Qt.AlignVCenter

                text: 'Add Player'
                font: fontMetrics.font
            }

            TextField {
                id: textFieldAddPlayer
                Layout.row: 1
                Layout.column: 2
                Layout.columnSpan: 4
                Layout.fillWidth: true
                Layout.preferredHeight: 50

                placeholderText: 'Example: 5483-7391'
                font: fontMetrics.font
                selectByMouse: true
                horizontalAlignment: TextInput.AlignRight

                onAccepted: {
                    var succeeded = core.addPlayer(text)
                    if (succeeded)
                    {
                        text = ''
                        generateScoreAnalysis()
                    }
                }
            }

            Button {
                id: buttonLoadDirectory

                Layout.row: 2
                Layout.columnSpan: 6
                Layout.fillWidth: true
                Layout.preferredHeight: 50

                text: qsTr('Load Directory')
                font: fontMetrics.font

                onClicked: {
                    fileDialog.open();
                }

                background: Rectangle {
                    radius: 10
                    color: parent.down ? '#FCF3CF'
                                       : (parent.hovered ? '#B4F8C8' : '#F1948A')
                }
            }

            GroupBox {
                id: groupBox
                Layout.row: 3
                Layout.columnSpan: 6
                Layout.rowSpan: 3
                Layout.fillWidth: true

                title: 'IST'

                label: Rectangle {
                    id: labelIst
                    width: parent.width * 0.7
                    height: textGroupBoxTitle.font.pixelSize
                    anchors.horizontalCenter: parent.horizontalCenter
                    anchors.bottom: parent.top
                    anchors.bottomMargin: -height/2-3

                    color: '#F5B041'

                    Text {
                        id: textGroupBoxTitle
                        anchors.centerIn: parent

                        text: qsTr('IST(score.iidx.app)')
                        font.family: 'Verdana'
                        font.pixelSize: 14
                    }
                }

                background: Rectangle {
                    anchors.fill: parent
                    color: 'transparent'
                    border.color: labelIst.color
                    radius: 5
                }

                GridLayout {
                    columns: 6
                    rows: 4

                    width: parent.width

                    Button {
                        id: buttonDownloadIst

                        Layout.row: 0
                        Layout.columnSpan: 6
                        Layout.fillWidth: true
                        Layout.preferredHeight: 50

                        text: qsTr('Download from IST')
                        font: fontMetrics.font

                        enabled: comboBoxPlayer.currentText!='' && !core.isDownloadingIst

                        onClicked: {
                            core.downloadIst(comboBoxPlayer.currentText,
                                             textFieldVersions.text,
                                             textFieldStyles.text,
                                             checkBoxPowerShell.checked)
                        }

                        background: Rectangle {
                            radius: 10
                            color: parent.down ? '#FCF3CF'
                                               : (parent.hovered ? '#B4F8C8' : '#F1948A')
                        }
                    }

                    CheckBox {
                        id: checkBoxPowerShell

                        Layout.row: 1
                        Layout.columnSpan: 6
                        Layout.fillWidth: true
                        Layout.preferredHeight: 20

                        indicator.width: 15
                        indicator.height: 15

                        text: 'Run in PowerShell'
                        font.family: 'Verdana'
                        font.pixelSize: 14
                    }

                    Text {
                        Layout.row: 2
                        Layout.columnSpan: 2
                        Layout.alignment: Qt.AlignVCenter

                        text: 'Versions'
                        font: fontMetrics.font
                    }

                    TextField {
                        id: textFieldVersions
                        Layout.row: 2
                        Layout.column: 2
                        Layout.columnSpan: 4
                        Layout.fillWidth: true
                        Layout.preferredHeight: 50

                        text: '27, 28'
                        placeholderText: 'Example: 24, 27, 28'
                        font: fontMetrics.font
                        selectByMouse: true
                        horizontalAlignment: TextInput.AlignRight
                    }

                    Text {
                        Layout.row: 3
                        Layout.columnSpan: 2
                        Layout.alignment: Qt.AlignVCenter

                        text: 'Styles'
                        font: fontMetrics.font
                    }

                    TextField {
                        id: textFieldStyles
                        Layout.row: 3
                        Layout.column: 2
                        Layout.columnSpan: 4
                        Layout.fillWidth: true
                        Layout.preferredHeight: 50

                        text: 'SP, DP'
                        placeholderText: 'Example: SP, DP'
                        font: fontMetrics.font
                        selectByMouse: true
                        horizontalAlignment: TextInput.AlignRight
                    }
                }
            }

            Text {
                Layout.row: 6
                Layout.columnSpan: 2
                Layout.alignment: Qt.AlignVCenter

                text: 'Play Style'
                font: fontMetrics.font
            }

            StyledComboBox {
                id: comboBoxPlayStyle

                Layout.row: 6
                Layout.column: 2
                Layout.columnSpan: 4
                Layout.fillWidth: true
                Layout.preferredHeight: 50

                model: core.playStyleList
                initialText: 'SinglePlay'

                onActivated: {
                    updatePlayer()
                }
            }

            Text {
                Layout.row: 7
                Layout.columnSpan: 2
                Layout.alignment: Qt.AlignVCenter

                text: 'Difficulty'
                font: fontMetrics.font
            }

            StyledComboBox {
                id: comboBoxDifficulty

                Layout.row: 7
                Layout.column: 2
                Layout.columnSpan: 4
                Layout.fillWidth: true
                Layout.preferredHeight: 50

                model: difficultyListModel
                initialText: 'Another'
                comboBox.textRole: 'display'

                onActivated: {
                    updateMusicScore()
                }
            }

            Text {
                Layout.row: 8
                Layout.columnSpan: 2
                Layout.alignment: Qt.AlignVCenter

                text: 'Active Version'
                font: fontMetrics.font
            }

            StyledComboBox {
                id: comboBoxActiveVersion

                Layout.row: 8
                Layout.column: 2
                Layout.columnSpan: 4
                Layout.fillWidth: true
                Layout.preferredHeight: 50

                model: statisticsManager.activeVersionList

                onActivated: {
                    console.log('comboBoxActiveVersion actived')
                    generateScoreAnalysis()
                }
            }
        }

        ColumnLayout {
            Layout.fillWidth: true

            TabBar {
                id: tabBar
                Layout.fillWidth: true
                Layout.preferredHeight: 50

                background: Rectangle {
                    color: 'transparent'
                }

                TabButton {
                    width: 150
                    height: parent.height
                    anchors.verticalCenter: parent.verticalCenter
                    text: qsTr('Graph')
                    font: fontMetrics.font

                    background: Rectangle {
                        color: tabBar.currentIndex==0 ? '#F1C40F' : '#616A6B'
                        radius: 5
                    }
                }

                TabButton {
                    width: 150
                    height: parent.height
                    anchors.verticalCenter: parent.verticalCenter
                    text: qsTr('Statistics')
                    font: fontMetrics.font

                    background: Rectangle {
                        color: tabBar.currentIndex==1 ? '#F1C40F' : '#616A6B'
                        radius: 5
                    }
                }

                TabButton {
                    width: 150
                    height: parent.height
                    anchors.verticalCenter: parent.verticalCenter
                    text: qsTr('Recommend')
                    font: fontMetrics.font

                    enabled: false

                    background: Rectangle {
                        color: tabBar.currentIndex==2 ? '#F1C40F' : '#616A6B'
                        radius: 5
                    }
                }
            }

            StackLayout {
                Layout.fillWidth: true
                Layout.fillHeight: true
                Layout.alignment: Qt.AlignBottom

                currentIndex: tabBar.currentIndex

                GridLayout {
                    columns: 6
                    rows: 3

                    MusicListView {
                        id: musicListView

                        Layout.column: 0
                        Layout.rowSpan: 3
                        Layout.preferredWidth: 300
                        Layout.fillHeight: true

                        listView.model: musicListModel

                        Component.onCompleted: {
                            musicListView.listView.sections = core.versionNameList
                            musicListView.listView.enableAllSections(false)
                        }

                        onMusicIdChanged: {
                            updateMusicScore()
                        }
                    }

                    Rectangle {
                        Layout.row: 0
                        Layout.column: 1
                        Layout.preferredWidth: 10
                    }

                    Text {
                        Layout.row: 0
                        Layout.column: 2
                        Layout.columnSpan: 2
                        Layout.alignment: Qt.AlignVCenter

                        text: 'Timeline begin'
                        font: fontMetrics.font
                    }

                    StyledComboBox {
                        id: comboBoxTimeline

                        Layout.row: 0
                        Layout.column: 4
                        Layout.columnSpan: 1
                        Layout.preferredWidth: 200
                        Layout.preferredHeight: 50
                        initialText: 'copula'

                        model: graphManager.timelineBeginVersionList

                        onActivated: {
                            //console.log('comboBoxTimeline onActivated', currentText)
                            graphManager.updateTimelineBeginVersion(currentText);
                            triggerScoreChartViewUpdate()
                        }
                    }

                    Rectangle {
                        Layout.row: 0
                        Layout.column: 5
                        Layout.fillWidth: true
                    }

                    ScoreChartView {
                        id: scoreChartView

                        Layout.row: 1
                        Layout.column: 1
                        Layout.rowSpan: 2
                        Layout.columnSpan: 6
                        Layout.fillWidth: true
                        Layout.fillHeight: true

                        repeaterScoreAnalysis.model: graphAnalysisListModel
                        repeaterScoreLevel.model: scoreLevelListModel

                        Component.onCompleted: {
                            graphManager.setup(
                                scoreChartView.legend,
                                scoreChartView.lineSeriesScore,
                                scoreChartView.dateTimeAxis,
                                scoreChartView.valueAxisScore,
                                scoreChartView.categoryAxisVersion,
                                scoreChartView.scatterSeriesScoreLevel,
                                scoreChartView.valueAxisScoreLevel
                            )
                        }
                    }
                }

                StatsView {
                    id: statsView
                    tableView.model: statsTableModel
                    horizontalHeaderView.model: statsHorizontalHeaderModel
                    verticalHeaderView.model: statsVerticalHeaderModel
                    comboBoxDifficultyVersion.model: statisticsManager.difficultyVersionList

                    onOptionChanged: {
                        updateStatsTable()
                    }
                }

                Rectangle {
                    color: 'plum'
                }
            }
        }
    }

    FileDialog {
        id: fileDialog
        title: 'Select directory'
        folder: 'file:///E:/project_document/score2dx'
        selectFolder: true
        onAccepted: {
            core.loadDirectory(fileDialog.fileUrl)
            updatePlayer()
        }
    }

    function updatePlayer()
    {
        updateMusicScore()
        generateScoreAnalysis()
    }

    function updateMusicScore()
    {
        graphManager.updatePlayerScore(
            comboBoxPlayer.currentText,
            comboBoxPlayStyle.currentText,
            musicListView.musicId,
            comboBoxDifficulty.currentText
        )

        triggerScoreChartViewUpdate()
    }

    function triggerScoreChartViewUpdate()
    {
        //'' workaround to trigger width update to adjust position.
        scoreChartView.width += 1
        scoreChartView.width -= 1
    }

    function generateScoreAnalysis()
    {
        console.log('generateScoreAnalysis')
        core.setActiveVersion(comboBoxPlayer.currentText, comboBoxActiveVersion.currentText)
        statisticsManager.updateDifficultyVersionList()
        updateStatsTable()
    }

    function updateStatsTable()
    {
        console.log('updateStatsTable')
        statisticsManager.updateStatsTable(
            comboBoxPlayer.currentText,
            comboBoxPlayStyle.currentText,
            statsView.tableType,
            statsView.comboBoxDifficultyVersion.currentText,
            statsView.columnType,
            statsView.valueType
        )
    }
}
