import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Layouts 1.12

Item {
    id: calendar
    property date beginDate: (initializeBeginDate())
    property date endDate: beginDate
    readonly property var weekDays: ['Sun', 'Mon', 'Tue', 'Wed', 'Thu', 'Fri', 'Sat']

    signal dateClicked(string date)

    width: 300
    implicitHeight: columnLayout.implicitHeight

    ColumnLayout {
        id: columnLayout
        anchors.fill: parent
        spacing: 0

        Rectangle {
            id: title
            Layout.alignment: Qt.AlignHCenter

            width: 280
            height: 30

            Text {
                anchors.fill: parent
                text: getIsoDate(new Date())
            }
        }

        RowLayout {
            Layout.alignment: Qt.AlignHCenter
            spacing: 0

            Repeater {
                model: weekDays
                delegate: Rectangle {
                    implicitWidth: 40
                    implicitHeight: 40
                    border.color: 'red'
                    color: 'white'

                    Text {
                        anchors.fill: parent
                        text: weekDays[index]
                    }
                }
            }
        }

        GridLayout {
            Layout.fillWidth: true
            Layout.maximumWidth: 280
            Layout.alignment: Qt.AlignHCenter

            rows: 6
            columns: 7
            rowSpacing: 0
            columnSpacing: 0

            Repeater {
                model: 42
                delegate: Rectangle {
                    implicitWidth: 40
                    implicitHeight: 40
                    border.color: 'red'
                    color: 'white'

                    Text {
                        id: textDate
                        anchors.fill: parent
                    }

                    MouseArea {
                        anchors.fill: parent
                        onClicked: {
                            console.log('repeater ', index)
                            var date = new Date(calendar.beginDate)
                            date.setDate(date.getDate()+index)
                            var isoDate = getIsoDate(date)
                            console.log('date', isoDate, 'clicked')
                            dateClicked(isoDate)
                        }
                    }

                    Component.onCompleted: {
                        textDate.text = calendar.endDate.getDate()
                        var nextDate = new Date(calendar.endDate)
                        nextDate.setDate(nextDate.getDate()+1)
                        calendar.endDate = nextDate
                    }
                }
            }
        }

    }

    Component.onCompleted: {
        console.log('Component.onCompleted calendar.beginDate', calendar.beginDate)
        console.log('width', width)
    }

    function initializeBeginDate() {
        console.log(initializeBeginDate)
        var lefTopDay = new Date()
        lefTopDay.setDate(lefTopDay.getDate()-lefTopDay.getDate()+1)
        lefTopDay.setDate(lefTopDay.getDate()-lefTopDay.getDay())
        console.log(lefTopDay)
        return lefTopDay
    }

    //! @brief Get ISO '2021-11-20' formatted string from Date object.
    function getIsoDate(date) {
        var local = new Date(date)
        local.setMinutes(local.getMinutes()-local.getTimezoneOffset())
        return local.toISOString().split('T')[0].split(' ')[0]
    }
}
