import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Layouts 1.12

//'' To avoid confusion:
//''    type 'Date': datetime class in Qt/Javascript
//''    variable name with 'dateTime': instance of datetime, even if it's new Date().
//''    variable name with 'date': date part of datetime, not padded to two digit as in ISO format.
//''    variable name with 'isoDate': ISO format 'YYYY-MM-DD' from datetime
Item {
    id: root
    property date leftTopDateTime: (getLeftTopDateTimeFromToday())
    property date iteratingDateTime: leftTopDateTime
    property string currentYearMonth: getYearMonthOfToday()
    property string currentIsoDate: ''

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

            width: 280
            height: 30
            Layout.alignment: Qt.AlignHCenter

            color: 'lightsalmon'

            Text {
                anchors.fill: parent
                color: 'white'
                text: root.currentYearMonth
                font.family: 'Verdana'
                font.pixelSize: 16
                font.bold: true
                style: Text.Outline
                horizontalAlignment: Text.AlignHCenter
                verticalAlignment: Text.AlignVCenter
            }
        }

        RowLayout {
            id: weekDaysRow
            Layout.alignment: Qt.AlignHCenter
            spacing: 0

            Repeater {
                model: weekDays
                delegate: Rectangle {
                    implicitWidth: 40
                    implicitHeight: 40
                    border.color: 'black'
                    color: 'dodgerblue'

                    Text {
                        anchors.fill: parent
                        color: 'white'
                        text: weekDays[index]
                        font.family: 'Verdana'
                        font.pixelSize: 14
                        font.bold: true
                        style: Text.Outline
                        horizontalAlignment: Text.AlignHCenter
                        verticalAlignment: Text.AlignVCenter
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
                    id: dateRectangle
                    property string isoDate: ''

                    implicitWidth: 40
                    implicitHeight: 40
                    border.color: 'black'
                    color: root.currentIsoDate===isoDate ? 'lightskyblue'
                           : 'slategray'

                    Text {
                        id: textDate
                        anchors.fill: parent
                        color: dateRectangle.isoDate.startsWith(root.currentYearMonth)
                               ? 'white'
                               : 'silver'
                        text: weekDays[index]
                        font.family: 'Verdana'
                        font.pixelSize: 14
                        font.bold: true
                        style: Text.Outline
                        horizontalAlignment: Text.AlignHCenter
                        verticalAlignment: Text.AlignVCenter
                    }

                    MouseArea {
                        anchors.fill: parent
                        onClicked: {
                            root.currentIsoDate = isoDate
                            dateClicked(isoDate)
                        }
                    }

                    Component.onCompleted: {
                        isoDate = getIsoDate(root.iteratingDateTime)
                        textDate.text = root.iteratingDateTime.getDate()
                        let next = new Date(root.iteratingDateTime)
                        next.setDate(next.getDate()+1)
                        root.iteratingDateTime = next
                    }
                }
            }
        }

    }

    function getYearMonthOfToday() {
        let today = new Date()
        let isoDate = getIsoDate(today)
        return isoDate.substring(0, 7)
    }

    function getLeftTopDateTimeFromToday() {
        let lefTop = new Date()
        lefTop.setDate(lefTop.getDate()-lefTop.getDate()+1)
        lefTop.setDate(lefTop.getDate()-lefTop.getDay())
        return lefTop
    }

    //! @brief Get ISO '2021-11-20' formatted string from Date object.
    function getIsoDate(dateTime) {
        let local = new Date(dateTime)
        local.setMinutes(local.getMinutes()-local.getTimezoneOffset())
        return local.toISOString().split('T')[0].split(' ')[0]
    }
}
