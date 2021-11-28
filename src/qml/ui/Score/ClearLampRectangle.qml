import QtQuick 2.15
import QtQuick.Controls 2.15

Rectangle {
    id: root

    property string clear: ''
    property string difficulty: ''

    width: 20
    height: 40

    border.color: 'black'
    color: 'black'

    states: [
        State {
            name: 'No Play'
            when: clear==='NO PLAY'
            PropertyChanges { target: root; color: '#17202A' }
        },
        State {
            name: 'Fail'
            when: clear==='FAILED'
            PropertyChanges { target: failAnimation; running: true }
        },
        State {
            name: 'Assist'
            when: clear==='ASSIST'
            PropertyChanges { target: root; color: '#BB8FCE' }
        },
        State {
            name: 'Easy'
            when: clear==='EASY'
            PropertyChanges { target: root; color: '#58D68D' }
        },
        State {
            name: 'Clear'
            when: clear==='CLEAR'
            PropertyChanges {
                target: root
                color: difficulty==='L' ? '#f500ff'
                       : difficulty==='A' ? 'red'
                       : difficulty==='H' ? '#ffb746'
                       : '#3498DB'
            }
        },
        State {
            name: 'Hard'
            when: clear==='HARD'
            PropertyChanges { target: root; color: 'white' }

        },
        State {
            name: 'EX'
            when: clear==='EX HARD'
            PropertyChanges { target: exAnimation; running: true }
        },
        State {
            name: 'FC'
            when: clear==='FC'
            PropertyChanges { target: fcAnimation; running: true }
        }
    ]

    SequentialAnimation on color {
        id: fcAnimation
        running: false
        loops: Animation.Infinite
        ColorAnimation { from: 'white'; to: 'cyan'; duration: 100 }
        ColorAnimation { from: 'cyan'; to: 'white'; duration: 100 }
        ColorAnimation { from: 'white'; to: 'yellow'; duration: 100 }
    }

    SequentialAnimation on color {
        id: exAnimation
        running: false
        loops: Animation.Infinite
        ColorAnimation { from: 'red'; to: 'yellow'; duration: 100 }
        ColorAnimation { from: 'yellow'; to: 'white'; duration: 100 }
        ColorAnimation { from: 'white'; to: 'red'; duration: 100 }
    }

    SequentialAnimation on color {
        id: failAnimation
        running: false
        loops: Animation.Infinite
        ColorAnimation { from: '#1C2833'; to: '#A93226'; duration: 100 }
        ColorAnimation { from: '#A93226'; to: '#1C2833'; duration: 100 }
    }
}
