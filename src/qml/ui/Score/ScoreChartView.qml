import QtQuick 2.15
import QtQuick.Controls 2.15
import QtCharts 2.3

ChartView
{
    id: chartView

    //'' To use C++ API to turn off marker.
    property alias legend: chartView.legend

    property alias lineSeriesScore: lineSeriesScore
    property alias dateTimeAxis: dateTimeAxis
    property alias valueAxisScore: valueAxisScore

    property alias categoryAxisVersion: categoryAxisVersion

    property alias scatterSeriesScoreLevel: scatterSeriesScoreLevel
    property alias valueAxisScoreLevel: valueAxisScoreLevel

    //'' model must match lineSeriesScore, to generate custom score analysis ui.
    property alias repeaterScoreAnalysis: repeaterScoreAnalysis

    //'' model must match scatterSeriesScoreLevel, to generate custom score level axis labels.
    property alias repeaterScoreLevel: repeaterScoreLevel

    theme: ChartView.ChartThemeDark
    antialiasing: true
    implicitWidth: 300
    implicitHeight: 300

    //'' Main series that have point of (DateTime, ExScore).
    LineSeries {
        id: lineSeriesScore
        name: 'EX Score'
        color: 'red'

        axisX: DateTimeAxis {
            id: dateTimeAxis
        }

        axisYRight: ValueAxis {
            id: valueAxisScore
        }
    }

    //'' Aux series just to add exta axis, axis category and grid line for visual.
    LineSeries {
        id: lineSeriesVisual

        //'' C++ controlled {version date time range}.
        axisXTop: CategoryAxis {
            id: categoryAxisVersion
        }

        //'' it's not actually accurate, but since it's just an indicator line, not care here.
        //'' ScoreLevel series below take care of exact key score.
        axisY: CategoryAxis {
            min: 0
            max: 1
            lineVisible: false
            gridLineColor: 'yellow'

            CategoryRange {
                endValue: 6/9
            }
            CategoryRange {
                label: 'A'
                endValue: 7/9
            }
            CategoryRange {
                label: 'AA'
                endValue: 8/9
            }
            CategoryRange {
                label: 'AAA'
                endValue: 8.5/9
            }
            CategoryRange {
                label: 'MAX-'
                endValue: 1
            }
        }
    }

    //'' Aux series to use hidden points to position manual rectangle correctly,
    //'' to add 'tick' on right y axis.
    //'' Use C++ to add points, which x = 1 and y is the desire position.
    ScatterSeries {
        id: scatterSeriesScoreLevel

        axisX: ValueAxis {
            min: 0
            max: 1
            gridVisible: false
            labelsVisible: false
            lineVisible: false
        }

        //'' C++ controlled min max value of key scores.
        axisY: ValueAxis {
            id: valueAxisScoreLevel
        }
    }

    Repeater {
        id: repeaterScoreAnalysis
        model: null

        Rectangle {
            color: 'transparent'
            border.color: 'red'

            property real parentWidth: chartView.width
            property real parentHeight: chartView.height

            onParentWidthChanged: chartView.adjustScoreAnalysisPosition(this, index)
            onParentHeightChanged: chartView.adjustScoreAnalysisPosition(this, index)

            Rectangle {
                id: pointIndicator
                width: 5
                height: 5

                anchors {
                    bottom: parent.bottom
                }
                anchors.horizontalCenter: parent.horizontalCenter

                color: lineSeriesScore.color
                radius: 10
            }

            Rectangle {
                id: placeHolder
                width: 5
                height: lineSeriesScore.pointLabelsFont.pixelSize+20

                anchors {
                    bottom: parent.bottom
                }
                anchors.horizontalCenter: parent.horizontalCenter

                visible: false
            }

            Rectangle {
                id: clearIndicator
                width: 5
                height: 5

                anchors {
                    top: rowClear.bottom
                }
                anchors.horizontalCenter: parent.horizontalCenter

                color: textClear.color
                radius: 10

                visible: rowClear.visible
            }

            Row {
                id: rowClear

                anchors {
                    bottom: placeHolder.top
                }
                anchors.horizontalCenter: parent.horizontalCenter

                spacing: 5
                visible: model.clearNewRecord

                Rectangle {
                    id: rectClear

                    width: 10
                    height: 15

                    border.color: 'gray'
                    color: '#1C2833'

                    states: [
                        State {
                            name: 'Fail'
                            when: textClear.text=='FAILED'
                            PropertyChanges { target: failAnimation; running: true }
                        },
                        State {
                            name: 'Assist'
                            when: textClear.text=='ASSIST'
                            PropertyChanges { target: rectClear; color: '#BB8FCE' }
                        },
                        State {
                            name: 'Easy'
                            when: textClear.text=='EASY'
                            PropertyChanges { target: rectClear; color: '#58D68D' }
                        },
                        State {
                            name: 'Clear'
                            when: textClear.text=='CLEAR'
                            PropertyChanges { target: rectClear; color: '#3498DB' }
                        },
                        State {
                            name: 'Hard'
                            when: textClear.text=='HARD'
                            PropertyChanges { target: rectClear; color: 'white' }

                        },
                        State {
                            name: 'EX'
                            when: textClear.text=='EX HARD'
                            PropertyChanges { target: exAnimation; running: true }
                        },
                        State {
                            name: 'FC'
                            when: textClear.text=='FULLCOMBO'
                            PropertyChanges { target: fcAnimation; running: true }
                        }
                    ]

                    SequentialAnimation on color {
                        id: fcAnimation
                        running: false
                        loops: Animation.Infinite
                        ColorAnimation { from: 'white'; to: 'cyan'; duration: 75 }
                        ColorAnimation { from: 'cyan'; to: 'white'; duration: 75 }
                        ColorAnimation { from: 'white'; to: 'yellow'; duration: 75 }
                        //ColorAnimation { from: 'cyan'; to: 'yellow'; duration: 50 }
                        //ColorAnimation { from: 'yellow'; to: 'cyan'; duration: 50 }
                        //ColorAnimation { from: 'cyan'; to: 'white'; duration: 100 }
                    }

                    SequentialAnimation on color {
                        id: exAnimation
                        running: false
                        loops: Animation.Infinite
                        ColorAnimation { from: 'red'; to: 'yellow'; duration: 75 }
                        ColorAnimation { from: 'yellow'; to: 'white'; duration: 75 }
                        ColorAnimation { from: 'white'; to: 'red'; duration: 75 }
                    }

                    SequentialAnimation on color {
                        id: failAnimation
                        running: false
                        loops: Animation.Infinite
                        ColorAnimation { from: '#1C2833'; to: '#A93226'; duration: 75 }
                        ColorAnimation { from: '#A93226'; to: '#1C2833'; duration: 75 }
                    }
                }

                Text {
                    id: textClear

                    anchors.verticalCenter: parent.verticalCenter

                    font.pixelSize: 12
                    text: model.clearRecord
                    renderType: Text.NativeRendering

                    color: '#FAD7A0'
                }
            }

            Rectangle {
                id: scoreLevelIndicator
                width: 5
                height: 5

                anchors {
                    bottom: rowClear.top
                }
                anchors.horizontalCenter: parent.horizontalCenter

                color: textDjLevel.color
                radius: 10

                visible: rowScoreLevel.visible
            }

            Row {
                id: rowScoreLevel

                anchors {
                    bottom: scoreLevelIndicator.top
                }
                anchors.horizontalCenter: parent.horizontalCenter

                spacing: 5
                visible: model.scoreNewRecord

                Text {
                    id: textDjLevel

                    anchors.verticalCenter: parent.verticalCenter

                    font.pixelSize: 12
                    text: model.djLevelRecord+' ('+model.scoreLevelRangeDiff+')'
                    renderType: Text.NativeRendering

                    color: '#ABEBC6'
                }
            }
        }
    }

    Repeater {
        id: repeaterScoreLevel
        model: null

        Rectangle {
            width: textKeyScore.implicitWidth+5
            height: textKeyScore.implicitHeight+5
            color: 'transparent'

            property real parentWidth: chartView.width
            property real parentHeight: chartView.height

            onParentWidthChanged: chartView.adjustScoreLevelPosition(this, index)
            onParentHeightChanged: chartView.adjustScoreLevelPosition(this, index)

            Text {
                id: textKeyScore
                anchors.centerIn: parent
                font.pixelSize: 12
                //font.bold: true
                //antialiasing: true
                text: model.y
                renderType: Text.NativeRendering

                color: 'white'
            }
        }
    }

    Component.onCompleted: {
        /*
        scatterSeriesScoreLevel.clear()
        for (var i=0; i<scoreLevelModel.count; ++i)
        {
            scatterSeriesScoreLevel.append(scoreLevelModel.get(i).x, scoreLevelModel.get(i).y)
        }
        */
    }

    function adjustScoreAnalysisPosition(item, index)
    {
        let point = Qt.point(lineSeriesScore.at(index).x, lineSeriesScore.at(index).y)
        let position = chartView.mapToPosition(point, lineSeriesScore)
        item.x = position.x - item.width/2
        item.y = position.y - item.height/2 + 2
        item.visible = true
        if (point.x<categoryAxisVersion.min)
        {
            item.visible = false
        }
    }

    function adjustScoreLevelPosition(item, index)
    {
        //let point = Qt.point(repeaterScoreLevel.model.get(index).x, repeaterScoreLevel.model.get(index).y)
        let point = Qt.point(scatterSeriesScoreLevel.at(index).x, scatterSeriesScoreLevel.at(index).y)
        let position = chartView.mapToPosition(point, scatterSeriesScoreLevel)
        item.x = position.x + 5
        item.y = position.y - item.height/2
        item.visible = true
        if (point.x<categoryAxisVersion.min)
        {
            item.visible = false
        }
    }
}
