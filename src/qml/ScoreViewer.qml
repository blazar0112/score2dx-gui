import QtQuick 2.15
import QtCharts 2.3
import QtQuick.Controls 2.15
import QtQuick.Dialogs 1.0
import QtQuick.Layouts 1.12
import QtQuick.Window 2.0

import Score2dx.Gui 1.0

import '../qml/ui/Activity'
import '../qml/ui/Calendar'
import '../qml/ui/Score'
import '../qml/ui/Statistics'
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

                CollapsibleGridLayout {
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
                                updateActiveVersion()
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

                    enabled: !Core.isDownloadingIst && !Core.isDownloadingMe

                    onClicked: {
                        fileDialog.open();
                    }

                    background: Rectangle {
                        radius: 10
                        color: parent.down ? '#FCF3CF'
                                           : (parent.hovered ? '#B4F8C8' : '#F1948A')
                    }
                }

                CollapsibleGridLayout {
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
                            updateActiveVersion()
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

                CollapsibleGridLayout {
                    Layout.fillWidth: true

                    title: 'ME (iidx.me)'

                    gridLayout.columns: 2
                    gridLayout.rows: 4

                    Button {
                        id: buttonDownloadMe
                        Layout.fillWidth: true
                        Layout.preferredHeight: 60
                        Layout.columnSpan: 2

                        text: 'Download from ME'
                        font.family: 'Verdana'
                        font.pixelSize: 20

                        enabled: currentMeUser.text!='' && !Core.isDownloadingIst && !Core.isDownloadingMe

                        background: Rectangle {
                            radius: 10
                            color: parent.down ? '#FCF3CF'
                                               : (parent.hovered ? '#B4F8C8' : '#F1948A')
                        }

                        onClicked: {
                            progressBarMe.value = 0
                            progressBarMeTimer.running = true
                            Core.downloadMe(currentMeUser.text)
                        }

                        ProgressBar {
                            id: progressBarMe

                            anchors.bottom: parent.bottom
                            width: parent.width

                            visible: Core.isDownloadingMe

                            from: 0
                            to: 3800
                            value: 0
                        }

                        Timer {
                            id: progressBarMeTimer
                            interval: 100
                            repeat: true
                            running: false
                            onTriggered: progressBarMe.value < progressBarMe.to ? progressBarMe.value += 1.0 : progressBarMe.value = progressBarMe.to
                        }
                    }

                    SideBarText {
                        text: 'Enter User'
                    }

                    TextField {
                        id: textFieldMeUser

                        Layout.fillWidth: true
                        Layout.preferredHeight: 30
                        Layout.alignment: Qt.AlignVCenter | Qt.AlignRight

                        placeholderText: 'e.g. blazar'
                        font: fontMetrics.font
                        selectByMouse: true
                        horizontalAlignment: TextInput.AlignRight

                        enabled: !Core.isDownloadingMe

                        onAccepted: {
                            let iidxId = Core.findMeUserIidxId(text)
                            currentMeUser.text = ''
                            currentMeIidxId.text = ''
                            if (iidxId!=='')
                            {
                                currentMeUser.text = text
                                currentMeIidxId.text = iidxId
                                text = ''
                            }
                        }
                    }

                    SideBarText {
                        text: 'ME User'
                    }

                    SideBarText {
                        id: currentMeUser
                        text: ''
                        color: 'wheat'
                    }

                    SideBarText {
                        text: 'IIDX ID'
                    }

                    SideBarText {
                        id: currentMeIidxId
                        text: ''
                        color: 'wheat'
                    }
                }

                CollapsibleGridLayout {
                    Layout.fillWidth: true

                    title: 'IST (score.iidx.app)'

                    gridLayout.columns: 2
                    gridLayout.rows: 5

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

                        enabled: comboBoxPlayer.currentText!=''
                                 && Core.isChromeDriverReady
                                 && !Core.isDownloadingIst && !Core.isDownloadingMe

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
                        text: 'Activity'
                        font: fontMetrics.font

                        background: Rectangle {
                            color: tabBar.currentIndex==2 ? tabBar.activeTabColor : tabBar.inactiveTabColor
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
                            color: tabBar.currentIndex==3 ? tabBar.activeTabColor : tabBar.inactiveTabColor
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
                            color: tabBar.currentIndex==4 ? tabBar.activeTabColor : tabBar.inactiveTabColor
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

                    StatisticsView {
                        id: statsView

                        Layout.fillWidth: true
                        Layout.fillHeight: true
                        Layout.alignment: Qt.AlignHCenter

                        comboBoxDifficultyVersion.model: StatisticsManager.difficultyVersionList
                        horizontalHeaderView.model: StatsHorizontalHeaderModel
                        verticalHeaderView.model: StatsVerticalHeaderModel
                        tableView.model: StatsTableModel
                        chartListHeader.model: StatsChartListHeaderModel
                        chartList.model: StatsChartListModel
                        chartListFilterRepeater.model: StatisticsManager.chartListFilterList

                        activeVersion: comboBoxActiveVersion.currentText

                        onOptionChanged: {
                            updateStatsTable()
                        }
                        onCellClicked: {
                            updateStatsChartList(row, column)
                        }
                    }

                    ColumnLayout {
                        Layout.fillWidth: true
                        Layout.fillHeight: true
                        spacing: 0

                        CollapsibleGridLayout {
                            width: 280
                            height: 30
                            Layout.alignment: Qt.AlignHCenter

                            title: 'Calendar'

                            gridLayout.columns: 1
                            gridLayout.rows: 1

                            Calendar {
                                id: calendar
                                Layout.fillWidth: true

                                customColorFunction: function(isoDate) {
                                    let dateType = ActivityManager.findActivityDateType(
                                        comboBoxPlayer.currentText,
                                        comboBoxPlayStyle.currentText,
                                        isoDate
                                    )
                                    if (dateType==='VersionBegin') { return 'lime' }
                                    if (dateType==='VersionEnd') { return 'red' }
                                    if (dateType==='HasActivity') { return 'yellow' }
                                    return ''
                                }

                                onDateClicked: {
                                    ActivityManager.updateActivity(
                                        comboBoxPlayer.currentText,
                                        comboBoxPlayStyle.currentText,
                                        date
                                    )
                                }
                            }
                        }

                        Rectangle {
                            Layout.fillWidth: true
                            height: 10
                            color: 'transparent'
                        }

                        SectionRectangle {
                            id: activitySection

                            implicitWidth: activityView.activityListView.implicitWidth
                                           ? activityView.activityListView.implicitWidth
                                           : activityView.implicitWidth
                            height: 40
                            Layout.alignment: Qt.AlignHCenter

                            innerText.font.pixelSize: 16
                            innerText.text: '['+calendar.selectedIsoDate+'] '
                                            +ActivityManager.activityPlayStyle
                                            +' Activity: '+ActivityListModel.rowItemCount
                                            +' PlayCount: '+ActivityListModel.getTotalIncreasedPlayCount()
                                            +'\n'+ActivityManager.getVersionDateTimeRange(calendar.selectedIsoDate)
                        }

                        ActivityView {
                            id: activityView
                            Layout.fillWidth: true
                            Layout.fillHeight: true

                            activityListView.model: ActivityListModel
                            activeVersion: comboBoxActiveVersion.currentText
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

        Rectangle {
            id: rectChromeStatus
            anchors.left: parent.left
            anchors.bottom: rectDbStatus.top

            width: textChromeStatus.contentWidth
            height: textChromeStatus.contentHeight

            color: '#A0000000'
            radius: 3

            Text {
                id: textChromeStatus
                anchors.fill: parent

                horizontalAlignment: Text.AlignHCenter
                verticalAlignment: Text.AlignVCenter

                font.family: 'Verdana'
                font.pixelSize: 12
                font.bold: true

                text: Core.getChromeStatus()
                color: Core.isChromeDriverReady ? 'MediumSeaGreen' : 'LightCoral'
            }
        }

        Rectangle {
            id: rectDbStatus
            anchors.left: parent.left
            anchors.bottom: parent.bottom

            width: textDbFilename.contentWidth
            height: textDbFilename.contentHeight

            color: '#A0000000'
            radius: 3

            Text {
                id: textDbFilename
                anchors.fill: parent

                horizontalAlignment: Text.AlignHCenter
                verticalAlignment: Text.AlignVCenter

                font.family: 'Verdana'
                font.pixelSize: 12
                font.bold: true

                text: 'DB: '+Core.getDbFilename()
                color: 'MediumSeaGreen'
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
        //folder: 'file:///E:/project_document/score2dx'
        selectFolder: true
        onAccepted: {
            Core.loadDirectory(fileDialog.fileUrl)
            updatePlayer()
        }
    }

    function updatePlayer()
    {
        console.log('updatePlayer')
        updateMusicScore()
        updateActiveVersion()
    }

    function updateMusicScore()
    {
        GraphManager.updatePlayerScore(
            comboBoxPlayer.currentText,
            comboBoxPlayStyle.currentText,
            musicListView.musicId,
            comboBoxDifficulty.currentText,
            comboBoxActiveVersion.currentText
        )

        triggerScoreChartViewUpdate()
    }

    function triggerScoreChartViewUpdate()
    {
        //'' workaround to trigger width update to adjust position.
        scoreChartView.width += 1
        scoreChartView.width -= 1
    }

    function updateActiveVersion()
    {
        let versionBeginDate = Core.setActiveVersion(comboBoxPlayer.currentText, comboBoxActiveVersion.currentText)
        StatisticsManager.updateDifficultyVersionList()
        updateStatsTable()
        calendar.updateCalender(versionBeginDate)
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

    function updateStatsChartList(row, column)
    {
        StatisticsManager.updateChartList(
            comboBoxPlayer.currentText,
            comboBoxPlayStyle.currentText,
            statsView.tableType,
            statsView.comboBoxDifficultyVersion.currentText,
            statsView.columnType,
            comboBoxActiveVersion.currentText,
            row,
            column
        )
    }
}
