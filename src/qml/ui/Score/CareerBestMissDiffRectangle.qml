import QtQuick 2.15
import QtQuick.Controls 2.15

//! @brief CareerBestMissDiffRectangle: style of PDBM Diff column
//! PDBM Diff = Personal Diffable Best Miss Difference
//! Diff may be following:
//! 1. 0 or +/- number
//! 2. 'PB': current score is the only score in career (new music)
//! 4. 'N/A': no diffable record found or current is N/A, cannot calculate difference.
//!          (for example: current is best score, but not second-score record, all records have same score.)
Rectangle {
    property alias innerText: innerText

    width: 60
    height: 30

    border.color: 'black'
    color: 'darkslategray'

    Text {
        id: innerText

        width: parent.width
        anchors.centerIn: parent

        color: text==='PB' ? 'cyan'
               : text==='N/A' ? 'lightpink'
               : text.startsWith('-') ? 'lime'
               : text==='0' ? 'white'
               : 'red'

        font.family: 'Verdana'
        font.pixelSize: text==='PB' || text==='N/A' ? 12 : 14
        font.bold: true
        style: Text.Outline
        horizontalAlignment: Text.AlignHCenter
    }
}
