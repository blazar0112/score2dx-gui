import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Layouts 1.12
import QtGraphicalEffects 1.0

import '../Style'

Item {
    id: root
    property alias activityList: activityList
    readonly property var headerWidths: [60, 380, 60, 40, 60]
    readonly property var chartHeaderWidths: [40, 40, 120, 60, 60, 40, 120, 60, 60]
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

        Row {
            id: header
            Layout.alignment: Qt.AlignHCenter

            Rectangle {
                width: headerWidths[0]
                height: activityList.rowHeight
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
                height: activityList.rowHeight
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
                width: headerWidths[2]+headerWidths[3]+headerWidths[4]
                height: activityList.rowHeight
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
            id: activityList

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

            delegate: Item {
                implicitWidth: root.implicitWidth
                implicitHeight: activityList.rowHeight+placeholder.height+musicActivityListView.height

                ColumnLayout {
                    anchors.fill: parent
                    spacing: 0

                    Row {
                        Rectangle {
                            width: headerWidths[0]
                            height: activityList.rowHeight
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
                            height: activityList.rowHeight
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
                            width: headerWidths[2]
                            height: activityList.rowHeight
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
                            width: headerWidths[3]
                            height: activityList.rowHeight
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
                            width: headerWidths[4]
                            height: activityList.rowHeight
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

                    ListView {
                        id: musicActivityListView
                        implicitHeight: model.rowCount()*activityList.rowHeight
                        model: activityList.model.getMusicActivityListModel(index)

                        delegate: Row {

                            Rectangle {
                                width: chartHeaderWidths[0]
                                height: activityList.rowHeight
                                border.color: 'black'
                                color: '#34495E'
                                Text {
                                    anchors.centerIn: parent
                                    text: styleDifficulty
                                    font: fontMetrics.font
                                    color: 'white'
                                }
                            }

                            Rectangle {
                                width: chartHeaderWidths[1]
                                height: activityList.rowHeight
                                border.color: 'black'
                                color: '#34495E'
                                Text {
                                    anchors.centerIn: parent
                                    text: level
                                    font: fontMetrics.font
                                    color: 'white'
                                }
                            }

                            Rectangle {
                                width: chartHeaderWidths[2]
                                height: activityList.rowHeight
                                border.color: 'black'
                                color: '#34495E'
                                Text {
                                    anchors.centerIn: parent
                                    text: previousClear
                                    font: fontMetrics.font
                                    color: 'white'
                                }
                            }

                            Rectangle {
                                width: chartHeaderWidths[3]
                                height: activityList.rowHeight
                                border.color: 'black'
                                color: '#34495E'
                                Text {
                                    anchors.centerIn: parent
                                    text: previousScore
                                    font: fontMetrics.font
                                    color: 'white'
                                }
                            }

                            Rectangle {
                                width: chartHeaderWidths[4]
                                height: activityList.rowHeight
                                border.color: 'black'
                                color: '#34495E'
                                Text {
                                    anchors.centerIn: parent
                                    text: previousMiss
                                    font: fontMetrics.font
                                    color: 'white'
                                }
                            }

                            Rectangle {
                                width: chartHeaderWidths[5]
                                height: activityList.rowHeight
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
                                width: chartHeaderWidths[6]
                                height: activityList.rowHeight
                                border.color: 'black'
                                color: '#34495E'
                                Text {
                                    anchors.centerIn: parent
                                    text: newRecordClear
                                    font: fontMetrics.font
                                    color: 'white'
                                }
                            }

                            Rectangle {
                                width: chartHeaderWidths[7]
                                height: activityList.rowHeight
                                border.color: 'black'
                                color: '#34495E'
                                Text {
                                    anchors.centerIn: parent
                                    text: newRecordScore
                                    font: fontMetrics.font
                                    color: 'white'
                                }
                            }

                            Rectangle {
                                width: chartHeaderWidths[8]
                                height: activityList.rowHeight
                                border.color: 'black'
                                color: '#34495E'
                                Text {
                                    anchors.centerIn: parent
                                    text: newRecordMiss
                                    font: fontMetrics.font
                                    color: 'white'
                                }
                            }
                        }
                    }

                    Rectangle {
                        id: placeholder
                        Layout.fillWidth: true
                        height: 3
                        color: 'yellow'
                    }
                }
            }
        }
    }
}
