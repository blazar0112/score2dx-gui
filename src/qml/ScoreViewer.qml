import QtQuick 2.15
import QtCharts 2.3
import QtQuick.Controls 2.15
import QtQuick.Dialogs 1.0
import QtQuick.Layouts 1.12
import QtQuick.Window 2.0

import Score2dx.Gui 1.0

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
    title: comboBoxPlayer.currentText
           ? 'Score Viewer '+Core.getScore2dxVersion()+' ['+comboBoxPlayer.currentText+']'
           : 'Score Viewer '+Core.getScore2dxVersion()

    FontMetrics {
        id: fontMetrics
        font.family: 'Verdana'
        font.pixelSize: 16
    }

    Item {
        id: rootItem
        focus: true
        anchors.fill: parent

        RowLayout {
            anchors.fill: parent
            spacing: 0

            ColumnLayout {
                Layout.maximumWidth: 300
                Layout.preferredWidth: 300
                Layout.fillWidth: true
                Layout.fillHeight: true
                Layout.alignment: Qt.AlignTop
                spacing: 0

                SideBarItem {
                    Layout.fillWidth: true
                    title: 'Player'

                    gridLayout.columns: 2
                    gridLayout.rows: 2

                    SideBarText {
                        text: 'IIDX ID'
                    }

                    StyledComboBox {
                        id: comboBoxPlayer

                        Layout.fillWidth: true
                        Layout.preferredHeight: 30
                        Layout.alignment: Qt.AlignVCenter | Qt.AlignRight

                        model: Core.playerList

                        onActivated: {
                            updatePlayer()
                        }
                    }

                    SideBarText {
                        text: 'Add Player'
                    }

                    TextField {
                        id: textFieldAddPlayer

                        Layout.fillWidth: true
                        Layout.preferredHeight: 30
                        Layout.alignment: Qt.AlignVCenter | Qt.AlignRight

                        placeholderText: 'e.g. 5483-7391'
                        font: fontMetrics.font
                        selectByMouse: true
                        horizontalAlignment: TextInput.AlignRight

                        onAccepted: {
                            var succeeded = Core.addPlayer(text)
                            if (succeeded)
                            {
                                text = ''
                                generateScoreAnalysis()
                            }
                        }
                    }
                }

                Button {
                    id: buttonLoadDirectory

                    Layout.fillWidth: true
                    Layout.preferredHeight: 60

                    text: 'Load Directory'
                    font.family: 'Verdana'
                    font.pixelSize: 20
                    font.bold: true

                    onClicked: {
                        fileDialog.open();
                    }

                    background: Rectangle {
                        radius: 10
                        color: parent.down ? '#FCF3CF'
                                           : (parent.hovered ? '#B4F8C8' : '#F1948A')
                    }
                }

                SideBarItem {
                    Layout.fillWidth: true
                    title: 'View Setting'

                    gridLayout.columns: 2
                    gridLayout.rows: 3

                    SideBarText {
                        text: 'Active Version'
                    }

                    StyledComboBox {
                        id: comboBoxActiveVersion

                        Layout.fillWidth: true
                        Layout.preferredHeight: 30

                        model: StatisticsManager.activeVersionList

                        onActivated: {
                            generateScoreAnalysis()
                        }
                    }

                    SideBarText {
                        text: 'Play Style'
                    }

                    StyledComboBox {
                        id: comboBoxPlayStyle

                        Layout.fillWidth: true
                        Layout.preferredHeight: 30

                        model: Core.playStyleList
                        initialText: 'SinglePlay'

                        onActivated: {
                            updatePlayer()
                        }
                    }

                    SideBarText {
                        text: 'Difficulty'
                    }

                    StyledComboBox {
                        id: comboBoxDifficulty

                        Layout.fillWidth: true
                        Layout.preferredHeight: 30

                        model: Core.difficultyList
                        initialText: 'Another'

                        onActivated: {
                            updateMusicScore()
                        }
                    }
                }

                SideBarItem {
                    Layout.fillWidth: true

                    title: 'IST'

                    gridLayout.columns: 2
                    gridLayout.rows: 4

                    Button {
                        id: buttonDownloadIst

                        Layout.row: 0
                        Layout.columnSpan: 2
                        Layout.fillWidth: true
                        Layout.preferredHeight: 60
                        Layout.alignment: Qt.AlignVCenter | Qt.AlignHCenter

                        text: 'Download from IST'
                        font.family: 'Verdana'
                        font.pixelSize: 20

                        enabled: comboBoxPlayer.currentText!='' && !Core.isDownloadingIst

                        onClicked: {
                            Core.downloadIst(comboBoxPlayer.currentText,
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

                    SideBarText {
                        text: 'PowerShell'
                    }

                    CheckBox {
                        id: checkBoxPowerShell

                        Layout.fillWidth: true
                        Layout.preferredHeight: 30

                        indicator.width: 30
                        indicator.height: 30
                        indicator.anchors.right: right

                        background: Rectangle {
                            anchors.fill: parent
                            color: '#2ECC71'
                        }
                    }

                    SideBarText {
                        text: 'Versions'
                    }

                    TextField {
                        id: textFieldVersions
                        Layout.fillWidth: true
                        Layout.preferredHeight: 30

                        text: '28, 29'
                        placeholderText: 'Example: 24, 28, 29'
                        font: fontMetrics.font
                        selectByMouse: true
                        horizontalAlignment: TextInput.AlignRight
                    }

                    SideBarText {
                        text: 'Styles'
                    }

                    TextField {
                        id: textFieldStyles
                        Layout.fillWidth: true
                        Layout.preferredHeight: 30

                        text: 'SP, DP'
                        placeholderText: 'Example: SP, DP'
                        font: fontMetrics.font
                        selectByMouse: true
                        horizontalAlignment: TextInput.AlignRight
                    }
                }
            }

            Rectangle {
                width: 1
                Layout.fillHeight: true
                color: 'gray'
            }

            ColumnLayout {
                Layout.fillWidth: true

                TabBar {
                    id: tabBar
                    Layout.fillWidth: true
                    Layout.preferredHeight: 50

                    property string activeTabColor: 'cyan'
                    property string inactiveTabColor: 'darkslateblue'

                    background: Rectangle {
                        color: 'mediumpurple'
                    }

                    TabButton {
                        width: 150
                        height: parent.height
                        anchors.verticalCenter: parent.verticalCenter
                        text: qsTr('Graph')
                        font: fontMetrics.font

                        background: Rectangle {
                            color: tabBar.currentIndex==0 ? tabBar.activeTabColor : tabBar.inactiveTabColor
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
                            color: tabBar.currentIndex==1 ? tabBar.activeTabColor : tabBar.inactiveTabColor
                            radius: 5
                        }
                    }

                    TabButton {
                        width: 150
                        height: parent.height
                        anchors.verticalCenter: parent.verticalCenter
                        text: 'Browse'
                        font: fontMetrics.font

                        enabled: false

                        background: Rectangle {
                            color: tabBar.currentIndex==2 ? tabBar.activeTabColor : tabBar.inactiveTabColor
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
                            color: tabBar.currentIndex==3 ? tabBar.activeTabColor : tabBar.inactiveTabColor
                            radius: 5
                        }
                    }
                }

                StackLayout {
                    Layout.fillWidth: true
                    Layout.fillHeight: true

                    currentIndex: tabBar.currentIndex

                    RowLayout {
                        Layout.fillWidth: true
                        Layout.fillHeight: true

                        Rectangle {
                            width: 5
                            height: 1
                            color: 'transparent'
                        }

                        MusicListView {
                            id: musicListView

                            Layout.preferredWidth: 300
                            Layout.fillHeight: true

                            listView.model: MusicListModel

                            Component.onCompleted: {
                                musicListView.listView.sections = Core.versionNameList
                                musicListView.listView.enableAllSections(false)
                            }

                            onMusicIdChanged: {
                                updateMusicScore()
                            }
                        }

                        Rectangle {
                            width: 5
                            height: 1
                            color: 'transparent'
                        }

                        ColumnLayout {

                            RowLayout {

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

                                    model: GraphManager.timelineBeginVersionList

                                    onActivated: {
                                        //console.log('comboBoxTimeline onActivated', currentText)
                                        GraphManager.updateTimelineBeginVersion(currentText);
                                        triggerScoreChartViewUpdate()
                                    }
                                }

                                Rectangle {
                                    Layout.row: 0
                                    Layout.column: 5
                                    Layout.fillWidth: true
                                }
                            }

                            ScoreChartView {
                                id: scoreChartView

                                Layout.row: 1
                                Layout.column: 1
                                Layout.rowSpan: 2
                                Layout.columnSpan: 6
                                Layout.fillWidth: true
                                Layout.fillHeight: true

                                repeaterScoreAnalysis.model: GraphAnalysisListModel
                                repeaterScoreLevel.model: ScoreLevelListModel

                                Component.onCompleted: {
                                    GraphManager.setup(
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
                    }

                    StatsView {
                        id: statsView

                        Layout.fillWidth: true
                        Layout.fillHeight: true
                        Layout.alignment: Qt.AlignHCenter

                        tableView.model: StatsTableModel
                        horizontalHeaderView.model: StatsHorizontalHeaderModel
                        verticalHeaderView.model: StatsVerticalHeaderModel
                        comboBoxDifficultyVersion.model: StatisticsManager.difficultyVersionList

                        onOptionChanged: {
                            updateStatsTable()
                        }
                    }

                    Rectangle {
                        color: 'plum'
                    }

                    Rectangle {
                        color: 'plum'
                    }
                }
            }
        }

        Keys.onPressed: {
            if (event.key === Qt.Key_S)
            {
                comboBoxPlayStyle.comboBox.currentIndex = 0
                updatePlayer()
            }
            if (event.key === Qt.Key_D)
            {
                comboBoxPlayStyle.comboBox.currentIndex = 1
                updatePlayer()
            }
        }
    }

    FileDialog {
        id: fileDialog
        title: 'Select directory'
        folder: 'file:///E:/project_document/score2dx'
        selectFolder: true
        onAccepted: {
            Core.loadDirectory(fileDialog.fileUrl)
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
        GraphManager.updatePlayerScore(
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
        Core.setActiveVersion(comboBoxPlayer.currentText, comboBoxActiveVersion.currentText)
        StatisticsManager.updateDifficultyVersionList()
        updateStatsTable()
    }

    function updateStatsTable()
    {
        StatisticsManager.updateStatsTable(
            comboBoxPlayer.currentText,
            comboBoxPlayStyle.currentText,
            statsView.tableType,
            statsView.comboBoxDifficultyVersion.currentText,
            statsView.columnType,
            statsView.valueType
        )
    }
}
