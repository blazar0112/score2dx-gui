import QtQuick 2.15
import QtQuick.Controls 2.15

//! @brief CareerBestScoreDiffRectangle: style of PDBS Diff column
//! PDBS Diff = Personal Diffable Best Score Difference
//! Diff may be following:
//! 1. 0 or +/- number
//! 2. 'PB': current score is the only score in career (new music)
//! 3. 'NP': current score is regarded as not played yet in this veresion
//!          cannot calculate difference
//! 4. 'N/A': no diffable record found
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
               : text==='NP' || text==='N/A' ? 'lightpink'
               : text.startsWith('+') ? 'lime'
               : text==='0' ? 'white'
               : 'red'

        font.family: 'Verdana'
        font.pixelSize: text==='PB' || text==='NP' || text==='N/A' ? 12 : 14
        font.bold: true
        style: Text.Outline
        horizontalAlignment: Text.AlignHCenter
    }
}
