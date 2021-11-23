import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Layouts 1.12
import QtGraphicalEffects 1.0

import '../Activity'
import '../Score'
import '../Style'

Item {
    id: root
    property alias activityList: activityListView
    property alias activitySectionText: activitySectionText

    readonly property string rightArrow: '➜'

    //'' Header:    | #Act  | Time(JST) | Ver | Title | PreviousPlayCount | -> | PlayCount |
    //'' AuxHeader: | Lv    | Previous Record | -> | New Record |
    readonly property var headerWidths: [40, 60, 40, 500, 100, 40, 100]
    readonly property var auxHeaderWidths: [40, 400, 40, 400]

    //'' Header:    | Lv    | C | Clear | Score | Miss | -> | C | Clear | Score | Miss |
    //'' Row:       | Lv    | ClearLamp | ClearText | Score | Miss | -> | ClearLamp | ClearText | Score | Miss |
    readonly property var chartHeaderWidths: [40, 30, 250, 60, 60, 40, 30, 250, 60, 60]
    property string activeVersion: ''

    implicitWidth: 880

    FontMetrics {
        id: fontMetrics
        font.family: 'Verdana'
        font.pixelSize: 16
        font.bold: true
    }

    Gradient {
        id: gradientAAA
        GradientStop { position: 0.0; color: 'white' }
        GradientStop { position: 1.0; color: 'gold' }
    }

    Gradient {
        id: gradientAA
        GradientStop { position: 0.0; color: 'white' }
        GradientStop { position: 1.0; color: 'silver' }
    }

    Gradient {
        id: gradientA
        GradientStop { position: 0.0; color: 'white' }
        GradientStop { position: 1.0; color: '#2ECC71' }
    }

    Gradient {
        id: gradientAMinus
        GradientStop { position: 0.0; color: 'white' }
        GradientStop { position: 1.0; color: '#8E44AD' }
    }

    ColumnLayout {
        width: parent.width
        height: parent.height
        anchors.horizontalCenter: parent.horizontalCenter

        spacing: 0

        Rectangle {
            id: activitySection
            implicitWidth: activityListView.implicitWidth ? activityListView.implicitWidth : root.implicitWidth
            Layout.alignment: Qt.AlignHCenter
            height: 30
            border.color: 'black'

            gradient: Gradient {
                orientation: Gradient.Horizontal
                GradientStop { position: 0.0; color: 'white' }
                GradientStop { position: 1.0; color: '#512E5F' }
            }

            Text {
                id: activitySectionText
                anchors.centerIn: parent
                font: fontMetrics.font
            }
        }

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
                            activeVersion: activeVersion

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
                            innerText.text: 'Lv'
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
                            innerText.text: 'Miss'
                        }

                        ChartActivityHeaderRectangle {
                            width: chartHeaderWidths[5]
                            height: parent.rowHeight
                            innerText.text: rightArrow
                        }

                        ChartActivityHeaderRectangle {
                            width: chartHeaderWidths[6]
                            height: parent.rowHeight
                            innerText.text: 'CL'
                        }

                        ChartActivityHeaderRectangle {
                            width: chartHeaderWidths[7]
                            height: parent.rowHeight
                            innerText.text: 'Clear'
                        }

                        ChartActivityHeaderRectangle {
                            width: chartHeaderWidths[8]
                            height: parent.rowHeight
                            innerText.text: 'Score'
                        }

                        ChartActivityHeaderRectangle {
                            width: chartHeaderWidths[9]
                            height: parent.rowHeight
                            innerText.text: 'Miss'
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

                            ChartActivityRectangle {
                                width: chartHeaderWidths[2]
                                height: chartActivityListView.rowHeight
                                innerText.text : previousClear
                            }

                            ChartActivityRectangle {
                                width: chartHeaderWidths[3]
                                height: chartActivityListView.rowHeight
                                innerText.text : previousScore
                            }

                            ChartActivityRectangle {
                                width: chartHeaderWidths[4]
                                height: chartActivityListView.rowHeight
                                innerText.text : previousMiss
                            }

                            ChartActivityRectangle {
                                width: chartHeaderWidths[5]
                                height: chartActivityListView.rowHeight
                                innerText.text : '->'
                            }

                            ClearLampRectangle {
                                width: chartHeaderWidths[6]
                                height: chartActivityListView.rowHeight
                                color: chartActivityRectangle.color
                                clear: newRecordClear
                                difficulty: model.difficulty
                            }

                            ChartActivityRectangle {
                                width: chartHeaderWidths[7]
                                height: chartActivityListView.rowHeight
                                innerText.text : newRecordClear
                            }

                            ChartActivityRectangle {
                                width: chartHeaderWidths[8]
                                height: chartActivityListView.rowHeight
                                innerText.text : newRecordScore
                            }

                            ChartActivityRectangle {
                                width: chartHeaderWidths[9]
                                height: chartActivityListView.rowHeight
                                innerText.text : newRecordMiss
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
