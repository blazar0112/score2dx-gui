import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Layouts 1.12

//'' To avoid confusion:
//''    type 'Date': datetime class in Qt/Javascript
//''    variable name with 'dateTime': instance of datetime.
//''    variable name with 'date': date part of datetime, i.e. Date.getDate(),
//''                               not padded to two digit as in ISO format.
//''    variable name with 'isoDate': 'YYYY-MM-DD' part in ISO format (strip time part) from datetime
Item {
    id: root
    property date leftTopDateTime: getLeftTopDateTime(new Date())
    property date iteratingDateTime: leftTopDateTime
    property string currentYearMonth: getYearMonthOfToday()
    property string selectedIsoDate: ''

    //! @brief Control Date Text color, user can redefine to make new color. return '' to use default.
    //! Default to use white for date in month, and gray for date out of month.
    property var customColorFunction: function(isoDate) { return '' }

    readonly property var weekDays: ['Sun', 'Mon', 'Tue', 'Wed', 'Thu', 'Fri', 'Sat']

    signal dateClicked(string date)

    width: 300
    implicitHeight: columnLayout.implicitHeight

    ColumnLayout {
        id: columnLayout
        anchors.fill: parent
        spacing: 0

        Row {
            Rectangle {
                width: 30
                height: 30

                color: 'lightsalmon'

                Text {
                    anchors.fill: parent
                    color: 'white'
                    text: '<'
                    font.family: 'Verdana'
                    font.pixelSize: 16
                    font.bold: true
                    style: Text.Outline
                    horizontalAlignment: Text.AlignHCenter
                    verticalAlignment: Text.AlignVCenter
                }

                MouseArea {
                    anchors.fill: parent
                    onClicked: {
                        let monthFirstIsoDate = root.currentYearMonth+'-01'
                        let dateTime = getDateTime(monthFirstIsoDate)
                        dateTime.setMonth(dateTime.getMonth()-1)
                        let isoDate = getIsoDate(dateTime)
                        updateCalender(isoDate)
                    }
                }
            }

            Rectangle {
                width: 30
                height: 30

                color: 'lightsalmon'
            }

            Rectangle {
                id: title

                width: 160
                height: 30

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

            Rectangle {
                width: 30
                height: 30

                color: 'lightsalmon'

                Rectangle {
                    width: 25
                    height: 25
                    anchors.horizontalCenter: parent.horizontalCenter
                    anchors.verticalCenter: parent.verticalCenter
                    radius: 5

                    color: 'yellow'

                    Text {
                        anchors.fill: parent
                        color: 'white'
                        text: 'T'
                        font.family: 'Verdana'
                        font.pixelSize: 16
                        font.bold: true
                        style: Text.Outline
                        horizontalAlignment: Text.AlignHCenter
                        verticalAlignment: Text.AlignVCenter
                    }

                    MouseArea {
                        anchors.fill: parent
                        onClicked: {
                            let isoDate = getIsoDate(new Date())
                            updateCalender(isoDate)
                        }
                    }
                }
            }

            Rectangle {
                width: 30
                height: 30

                color: 'lightsalmon'

                Text {
                    anchors.fill: parent
                    color: 'white'
                    text: '>'
                    font.family: 'Verdana'
                    font.pixelSize: 16
                    font.bold: true
                    style: Text.Outline
                    horizontalAlignment: Text.AlignHCenter
                    verticalAlignment: Text.AlignVCenter
                }

                MouseArea {
                    anchors.fill: parent
                    onClicked: {
                        let monthFirstIsoDate = root.currentYearMonth+'-01'
                        let dateTime = getDateTime(monthFirstIsoDate)
                        dateTime.setMonth(dateTime.getMonth()+1)
                        let isoDate = getIsoDate(dateTime)
                        updateCalender(isoDate)
                    }
                }
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
                id: repeater
                model: 42

                delegate: Rectangle {
                    id: dateRectangle
                    property string isoDate: ''

                    implicitWidth: 40
                    implicitHeight: 40
                    border.color: 'black'
                    color: root.selectedIsoDate===isoDate ? 'lightskyblue'
                           : 'slategray'

                    Text {
                        id: textDate
                        anchors.fill: parent
                        color: customColorFunction(dateRectangle.isoDate)
                               ? customColorFunction(dateRectangle.isoDate)
                               : dateRectangle.isoDate.startsWith(root.currentYearMonth)
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
                            updateCalender(isoDate)
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

    function getYearMonth(isoDate) {
        return isoDate.substring(0, 7)
    }

    function getYearMonthOfToday() {
        let today = new Date()
        let isoDate = getIsoDate(today)
        return getYearMonth(isoDate)
    }

    function getLeftTopDateTime(dateTime) {
        let lefTop = new Date(dateTime)
        lefTop.setDate(1)
        lefTop.setDate(lefTop.getDate()-lefTop.getDay())
        return lefTop
    }

    //! @brief Get ISO '2021-11-20' formatted string from Date object.
    function getIsoDate(dateTime) {
        let local = new Date(dateTime)
        local.setMinutes(local.getMinutes()-local.getTimezoneOffset())
        return local.toISOString().split('T')[0]
    }

    function getDateTime(isoDate) {
        let dateTime = new Date()
        dateTime.setFullYear(Number(isoDate.split('-')[0]))
        dateTime.setMonth(Number(isoDate.split('-')[1])-1)
        dateTime.setDate(Number(isoDate.split('-')[2]))
        return dateTime
    }

    function updateCalender(isoDate) {
        if (!isoDate) { return; }

        root.selectedIsoDate = isoDate
        if (currentYearMonth!==getYearMonth(isoDate))
        {
            let dateTime = getDateTime(isoDate)

            root.leftTopDateTime = getLeftTopDateTime(dateTime)
            root.iteratingDateTime = root.leftTopDateTime
            root.currentYearMonth = getYearMonth(isoDate)

            repeater.model = 0
            repeater.model = 42
        }

        dateClicked(isoDate)
    }
}
