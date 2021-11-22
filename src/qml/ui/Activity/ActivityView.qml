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

    //'' Row: | Time(JST) | Ver | Title | PreviousPlayCount | -> | PlayCount |
    readonly property var headerWidths: [60, 40, 340, 60, 40, 60]

    //'' Header:    | Chart | Previous Record | -> | New Record |
    //'' AuxHeader: | Lv | C | Clear | Score | Miss | -> | C | Clear | Score | Miss |
    //'' Row:       | Lv | ClearLamp | ClearText | Score | Miss | -> | ClearLamp | ClearText | Score | Miss |
    readonly property var chartHeaderWidths: [60, 20, 110, 60, 60, 40, 20, 110, 60, 60]
    property string activeVersion: ''

    implicitWidth: 600

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
            height: activityListView.rowHeight
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
            Layout.alignment: Qt.AlignHCenter

            visible: activityListView.width!==0

            Rectangle {
                width: headerWidths[0]
                height: activityListView.rowHeight
                border.color: 'black'
                color: 'dodgerblue'
                Text {
                    width: parent.width
                    anchors.centerIn: parent
                    text: 'Time (JST)'
                    font.family: 'Verdana'
                    font.pixelSize: 16
                    font.bold: true
                    color: 'white'
                    wrapMode: Text.WrapAtWordBoundaryOrAnywhere
                    horizontalAlignment: Text.AlignHCenter
                }
            }

            Rectangle {
                width: headerWidths[1]
                height: activityListView.rowHeight
                border.color: 'black'
                color: 'dodgerblue'

                Text {
                    anchors.centerIn: parent
                    text: 'Ver'
                    font: fontMetrics.font
                    color: 'white'
                }
            }

            Rectangle {
                width: headerWidths[2]
                height: activityListView.rowHeight
                border.color: 'black'
                color: 'dodgerblue'

                Text {
                    anchors.centerIn: parent
                    text: 'Title'
                    font: fontMetrics.font
                    color: 'white'
                }
            }

            Rectangle {
                width: headerWidths[3]+headerWidths[4]+headerWidths[5]
                height: activityListView.rowHeight
                border.color: 'black'
                color: 'dodgerblue'

                Text {
                    anchors.centerIn: parent
                    text: 'Play Count'
                    font: fontMetrics.font
                    color: 'white'
                }
            }
        }

        ListView {
            id: activityListView

            readonly property int rowHeight: 40

            width: contentItem.childrenRect.width
            implicitWidth: width

            Layout.fillHeight: true
            Layout.alignment: Qt.AlignHCenter

            clip: true
            cacheBuffer: 40*rowHeight

            ScrollBar.vertical: ScrollBar {
                active: true
                width: 15
            }

            delegate: Item {
                implicitWidth: root.implicitWidth
                implicitHeight: activityListView.rowHeight
                                +chartActivityHeader.implicitHeight
                                +chartActivityAuxHeader.implicitHeight
                                +chartActivityListView.implicitHeight
                                +separateLine.height

                ColumnLayout {
                    anchors.fill: parent
                    spacing: 0

                    Row {
                        id: musicActivityRow

                        Rectangle {
                            id: activityRectangle
                            width: headerWidths[0]
                            height: activityListView.rowHeight
                            border.color: 'black'
                            color: '#34495E'
                            Text {
                                anchors.centerIn: parent
                                text: model.time
                                font: fontMetrics.font
                                color: 'white'
                            }
                        }

                        Rectangle {
                            width: headerWidths[1]
                            height: activityListView.rowHeight
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
                            width: headerWidths[2]
                            height: activityListView.rowHeight
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
                            width: headerWidths[3]
                            height: activityListView.rowHeight
                            border.color: 'black'
                            color: '#34495E'

                            Text {
                                anchors.centerIn: parent
                                text: model.previousPlayCount
                                font: fontMetrics.font
                                color: 'white'
                            }
                        }

                        Rectangle {
                            width: headerWidths[4]
                            height: activityListView.rowHeight
                            border.color: 'black'
                            color: '#34495E'

                            Text {
                                anchors.centerIn: parent
                                text: '->'
                                font: fontMetrics.font
                                color: 'white'
                            }
                        }

                        Rectangle {
                            width: headerWidths[5]
                            height: activityListView.rowHeight
                            border.color: 'black'
                            color: '#34495E'

                            Text {
                                anchors.centerIn: parent
                                text: model.playCount
                                font: fontMetrics.font
                                color: 'white'
                            }
                        }
                    }

                    Row {
                        id: chartActivityHeader

                        ChartActivityHeaderRectangle {
                            width: chartHeaderWidths[0]
                            height: chartActivityListView.rowHeight
                            color: activityRectangle.color
                            innerText.text: '#'+(index+1)
                        }

                        ChartActivityHeaderRectangle {
                            width: chartHeaderWidths[1]+chartHeaderWidths[2]+chartHeaderWidths[3]+chartHeaderWidths[4]
                            height: chartActivityListView.rowHeight
                            innerText.text: 'Previous Record'
                        }

                        ChartActivityHeaderRectangle {
                            width: chartHeaderWidths[5]
                            height: chartActivityListView.rowHeight
                            innerText.text: '->'
                        }

                        ChartActivityHeaderRectangle {
                            width: chartHeaderWidths[6]+chartHeaderWidths[7]+chartHeaderWidths[8]+chartHeaderWidths[9]
                            height: chartActivityListView.rowHeight
                            innerText.text: 'New Record'
                        }
                    }

                    Row {
                        id: chartActivityAuxHeader

                        ChartActivityHeaderRectangle {
                            width: chartHeaderWidths[0]
                            height: chartActivityListView.rowHeight
                            innerText.text: 'Lv'
                        }

                        ChartActivityHeaderRectangle {
                            width: chartHeaderWidths[1]
                            height: chartActivityListView.rowHeight
                            innerText.text: 'C'
                        }

                        ChartActivityHeaderRectangle {
                            width: chartHeaderWidths[2]
                            height: chartActivityListView.rowHeight
                            innerText.text: 'Clear'
                        }

                        ChartActivityHeaderRectangle {
                            width: chartHeaderWidths[3]
                            height: chartActivityListView.rowHeight
                            innerText.text: 'Score'
                        }

                        ChartActivityHeaderRectangle {
                            width: chartHeaderWidths[4]
                            height: chartActivityListView.rowHeight
                            innerText.text: 'Miss'
                        }

                        ChartActivityHeaderRectangle {
                            width: chartHeaderWidths[5]
                            height: chartActivityListView.rowHeight
                            innerText.text: '->'
                        }

                        ChartActivityHeaderRectangle {
                            width: chartHeaderWidths[6]
                            height: chartActivityListView.rowHeight
                            innerText.text: 'C'
                        }

                        ChartActivityHeaderRectangle {
                            width: chartHeaderWidths[7]
                            height: chartActivityListView.rowHeight
                            innerText.text: 'Clear'
                        }

                        ChartActivityHeaderRectangle {
                            width: chartHeaderWidths[8]
                            height: chartActivityListView.rowHeight
                            innerText.text: 'Score'
                        }

                        ChartActivityHeaderRectangle {
                            width: chartHeaderWidths[9]
                            height: chartActivityListView.rowHeight
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
