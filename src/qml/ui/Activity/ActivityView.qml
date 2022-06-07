import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Layouts 1.12
import QtGraphicalEffects 1.0

import '../Activity'
import '../Score'
import '../Style'

Item {
    id: root
    property alias activityListView: activityListView

    readonly property string rightArrow: '\u279C'

    //'' Header:    | #Act  | Time(JST) | Ver | Title | PreviousPlayCount | -> | PlayCount |
    //'' AuxHeader: | Lv    | Previous Record | -> | New Record | Statistics |
    readonly property var headerWidths: [40, 60, 40, 500, 100, 40, 100]
    readonly property var auxHeaderWidths: [40, 350, 20, 350, 120]

    //'' Row:       | Lv    |( ClearLamp | Clear | Score | DJ Level | SLv Diff | Miss )| () =  previous record
    //''                | -> | (new record same columns as previous record) |
    //''                | PDBS Diff | PDBM Diff |
    readonly property var chartHeaderWidths: [
        40, 20, 80, 60, 50, 90, 50,
        20, 20, 80, 60, 50, 90, 50,
        60, 60
    ]
    property string activeVersion: ''

    implicitWidth: 880

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

        Row {
            id: activityListHeader

            readonly property int headerHeight: 40

            Layout.alignment: Qt.AlignHCenter

            visible: activityListView.width!==0

            ActivityHeaderRectangle {
                width: headerWidths[0]
                height: parent.headerHeight
                innerText.text: '#Act'
            }

            ActivityHeaderRectangle {
                width: headerWidths[1]
                height: parent.headerHeight
                innerText.text: 'Time (JST)'
            }

            ActivityHeaderRectangle {
                width: headerWidths[2]
                height: parent.headerHeight
                innerText.text: 'Ver'
            }

            ActivityHeaderRectangle {
                width: headerWidths[3]
                height: parent.headerHeight
                innerText.text: 'Title'
            }

            ActivityHeaderRectangle {
                width: headerWidths[4]
                height: parent.headerHeight
                innerText.text: 'Previous PlayCount'
            }

            ActivityHeaderRectangle {
                width: headerWidths[5]
                height: parent.headerHeight
                innerText.text: rightArrow
            }

            ActivityHeaderRectangle {
                width: headerWidths[6]
                height: parent.headerHeight
                innerText.text: 'PlayCount'
            }
        }

        Row {
            id: activityListAuxHeader

            readonly property int headerHeight: 20

            Layout.alignment: Qt.AlignHCenter

            visible: activityListView.width!==0

            ActivityHeaderRectangle {
                width: auxHeaderWidths[0]
                height: parent.headerHeight
                innerText.text: 'Lv'
            }

            ActivityHeaderRectangle {
                width: auxHeaderWidths[1]
                height: parent.headerHeight
                innerText.text: 'Previous Record'
            }

            ActivityHeaderRectangle {
                width: auxHeaderWidths[2]
                height: parent.headerHeight
                innerText.text: rightArrow
            }

            ActivityHeaderRectangle {
                width: auxHeaderWidths[3]
                height: parent.headerHeight
                innerText.text: 'New Record'
            }

            ActivityHeaderRectangle {
                width: auxHeaderWidths[4]
                height: parent.headerHeight
                innerText.text: 'Stats Diff'
            }
        }

        ListView {
            id: activityListView

            width: contentItem.childrenRect.width
            implicitWidth: width

            Layout.fillHeight: true
            Layout.alignment: Qt.AlignHCenter

            clip: true
            cacheBuffer: 1000

            ScrollBar.vertical: ScrollBar {
                active: true
                width: 15
            }

            delegate: Item {
                implicitWidth: root.implicitWidth
                implicitHeight: musicActivityRow.rowHeight
                                +chartActivityHeader.rowHeight
                                +chartActivityListView.implicitHeight
                                +separateLine.height

                ColumnLayout {
                    anchors.fill: parent
                    spacing: 0

                    Row {
                        id: musicActivityRow

                        readonly property int rowHeight: 40

                        ActivityRectangle {
                            width: headerWidths[0]
                            height: parent.rowHeight
                            innerText.text: '#'+index
                        }

                        ActivityRectangle {
                            width: headerWidths[1]
                            height: parent.rowHeight
                            innerText.text: model.time
                        }

                        ActivityRectangle {
                            width: headerWidths[2]
                            height: parent.rowHeight
                            innerText.text: model.version
                        }

                        TitleRectangle {
                            width: headerWidths[3]
                            height: parent.rowHeight

                            version: model.version
                            activeVersion: root.activeVersion

                            innerText.text: model.title
                        }

                        ActivityRectangle {
                            width: headerWidths[4]
                            height: parent.rowHeight
                            innerText.text: model.previousPlayCount
                        }

                        ActivityRectangle {
                            width: headerWidths[5]
                            height: parent.rowHeight
                            innerText.text: rightArrow
                        }

                        ActivityRectangle {
                            width: headerWidths[6]
                            height: parent.rowHeight
                            innerText.text: model.playCount
                        }
                    }

                    Row {
                        id: chartActivityHeader

                        readonly property int rowHeight: 20

                        ChartActivityHeaderRectangle {
                            width: chartHeaderWidths[0]
                            height: parent.rowHeight
                        }

                        ChartActivityHeaderRectangle {
                            width: chartHeaderWidths[1]
                            height: parent.rowHeight
                            innerText.text: 'CL'
                        }

                        ChartActivityHeaderRectangle {
                            width: chartHeaderWidths[2]
                            height: parent.rowHeight
                            innerText.text: 'Clear'
                        }

                        ChartActivityHeaderRectangle {
                            width: chartHeaderWidths[3]
                            height: parent.rowHeight
                            innerText.text: 'Score'
                        }

                        ChartActivityHeaderRectangle {
                            width: chartHeaderWidths[4]
                            height: parent.rowHeight
                            innerText.text: 'DJ Lv'
                        }

                        ChartActivityHeaderRectangle {
                            width: chartHeaderWidths[5]
                            height: parent.rowHeight
                            innerText.text: 'SL Diff'
                        }

                        ChartActivityHeaderRectangle {
                            width: chartHeaderWidths[6]
                            height: parent.rowHeight
                            innerText.text: 'Miss'
                        }

                        ChartActivityHeaderRectangle {
                            width: chartHeaderWidths[7]
                            height: parent.rowHeight
                        }

                        ChartActivityHeaderRectangle {
                            width: chartHeaderWidths[8]
                            height: parent.rowHeight
                            innerText.text: 'CL'
                        }

                        ChartActivityHeaderRectangle {
                            width: chartHeaderWidths[9]
                            height: parent.rowHeight
                            innerText.text: 'Clear'
                        }

                        ChartActivityHeaderRectangle {
                            width: chartHeaderWidths[10]
                            height: parent.rowHeight
                            innerText.text: 'Score'
                        }

                        ChartActivityHeaderRectangle {
                            width: chartHeaderWidths[11]
                            height: parent.rowHeight
                            innerText.text: 'DJ Lv'
                        }

                        ChartActivityHeaderRectangle {
                            width: chartHeaderWidths[12]
                            height: parent.rowHeight
                            innerText.text: 'SL Diff'
                        }

                        ChartActivityHeaderRectangle {
                            width: chartHeaderWidths[13]
                            height: parent.rowHeight
                            innerText.text: 'Miss'
                        }

                        ChartActivityHeaderRectangle {
                            width: chartHeaderWidths[14]
                            height: parent.rowHeight
                            innerText.text: 'PDBS'
                            color: 'orangered'
                        }

                        ChartActivityHeaderRectangle {
                            width: chartHeaderWidths[15]
                            height: parent.rowHeight
                            innerText.text: 'PDBM'
                            color: 'orangered'
                        }
                    }

                    ListView {
                        id: chartActivityListView

                        readonly property int rowHeight: 30

                        implicitHeight: model.rowCount()*rowHeight
                        model: activityListView.model.getChartActivityListModel(index)

                        delegate: Row {
                            ChartActivityRectangle {
                                id: chartActivityRectangle
                                width: chartHeaderWidths[0]
                                height: chartActivityListView.rowHeight
                                innerText {
                                    text: level
                                    style: Text.Outline
                                    color:  model.difficulty==='L' ? '#f500ff'
                                            : model.difficulty==='A' ? 'red'
                                            : model.difficulty==='H' ? '#ffb746'
                                            : model.difficulty==='N' ? '#86cfff'
                                            : 'white'
                                }
                            }

                            ClearLampRectangle {
                                width: chartHeaderWidths[1]
                                height: chartActivityListView.rowHeight
                                color: chartActivityRectangle.color
                                clear: previousClear
                                difficulty: model.difficulty
                            }

                            ClearRectangle {
                                width: chartHeaderWidths[2]
                                height: chartActivityListView.rowHeight
                                innerText.text : previousClear
                                color: chartActivityRectangle.color
                            }

                            ChartActivityRectangle {
                                width: chartHeaderWidths[3]
                                height: chartActivityListView.rowHeight
                                innerText.text : previousScore
                            }

                            DjLevelRectangle {
                                width: chartHeaderWidths[4]
                                height: chartActivityListView.rowHeight
                                innerText.text : previousDjLevel
                                color: chartActivityRectangle.color
                            }

                            ScoreLevelCategoryRectangle {
                                width: chartHeaderWidths[5]
                                height: chartActivityListView.rowHeight
                                innerText.text : previousScoreLevelDiff
                                color: chartActivityRectangle.color
                            }

                            MissRectangle {
                                width: chartHeaderWidths[6]
                                height: chartActivityListView.rowHeight
                                innerText.text : previousMiss
                                color: chartActivityRectangle.color
                            }

                            ChartActivityRectangle {
                                width: chartHeaderWidths[7]
                                height: chartActivityListView.rowHeight
                                innerText.text : rightArrow
                            }

                            ClearLampRectangle {
                                width: chartHeaderWidths[8]
                                height: chartActivityListView.rowHeight
                                color: chartActivityRectangle.color
                                clear: newRecordClear
                                difficulty: model.difficulty
                            }

                            ClearRectangle {
                                width: chartHeaderWidths[9]
                                height: chartActivityListView.rowHeight
                                innerText.text : newRecordClear
                                color: chartActivityRectangle.color
                            }

                            ChartActivityRectangle {
                                width: chartHeaderWidths[10]
                                height: chartActivityListView.rowHeight
                                innerText.text : newRecordScore
                            }

                            DjLevelRectangle {
                                width: chartHeaderWidths[11]
                                height: chartActivityListView.rowHeight
                                innerText.text : newRecordDjLevel
                                color: chartActivityRectangle.color
                            }

                            ScoreLevelCategoryRectangle {
                                width: chartHeaderWidths[12]
                                height: chartActivityListView.rowHeight
                                innerText.text : newRecordScoreLevelDiff
                                color: chartActivityRectangle.color
                            }

                            MissRectangle {
                                width: chartHeaderWidths[13]
                                height: chartActivityListView.rowHeight
                                innerText.text : newRecordMiss
                                color: chartActivityRectangle.color
                            }

                            CareerBestScoreDiffRectangle {
                                width: chartHeaderWidths[14]
                                height: chartActivityListView.rowHeight
                                innerText.text : careerDiffableBestScoreDiff
                                color: chartActivityRectangle.color
                            }

                            CareerBestMissDiffRectangle {
                                width: chartHeaderWidths[15]
                                height: chartActivityListView.rowHeight
                                innerText.text : careerDiffableBestMissDiff
                                color: chartActivityRectangle.color
                            }
                        }
                    }

                    Rectangle {
                        id: separateLine
                        Layout.fillWidth: true
                        height: 2
                        color: 'dodgerblue'
                    }
                }
            }
        }
    }
}
