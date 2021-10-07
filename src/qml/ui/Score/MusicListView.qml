import QtQuick 2.15
import QtQuick.Controls 2.15

//! @brief MusicListView is section expandable ListView
// custom model requires to implement: count, get() to behave like ListModel
Item {
    property alias listView: listView
    property int musicId: 0

    Column {
        width: parent.width
        height: parent.height

        Rectangle {
            id: toolRow
            width: parent.width
            height: buttonCollapse.height+10

            color: 'transparent'

            Button {
                id: buttonCollapse
                width: imageCollapse.width+10
                height: imageCollapse.height+10
                anchors {
                    right: buttonExpand.left
                    verticalCenter: parent.verticalCenter
                }

                Image {
                    id: imageCollapse
                    width: 30
                    height: 30
                    anchors.centerIn: parent
                    source: 'qrc:/qml/image/collapse.png'
                }

                onClicked: listView.enableAllSections(false)
            }

            Button {
                id: buttonExpand
                width: imageExpand.width+10
                height: imageExpand.height+10
                anchors {
                    right: parent.right
                    verticalCenter: parent.verticalCenter
                }

                Image {
                    id: imageExpand
                    width: 30
                    height: 30
                    anchors.centerIn: parent
                    source: 'qrc:/qml/image/expand.png'
                }

                onClicked: listView.enableAllSections(true)
            }
        }

        ListView {
            id: listView

            property var collapsed: ({})
            property var sections: null
            readonly property int rowHeight: 30
            // user need provide model.count since model may come from C++.
            //property int modelRowCount: 0

            implicitWidth: 300
            implicitHeight: parent.height-toolRow.height

            focus: true
            clip: true

            cacheBuffer: 5*rowHeight
            highlightMoveVelocity: -1

            delegate: Rectangle {
                property bool expanded: listView.isSectionExpanded(model.version)

                width: listView.width
                implicitHeight: expanded ? textMusic.implicitHeight+10 : 0
                color: ListView.isCurrentItem ? '#F1948A' : '#7F8C8D'
                radius: 10
                border.color: 'black'

                Text {
                    id: textMusic
                    width: parent.width
                    height: listView.rowHeight
                    anchors.centerIn: parent

                    color: 'white'
                    visible: parent.expanded

                    text: title
                    minimumPixelSize: 8
                    font.pixelSize: 24
                    font.bold: true
                    fontSizeMode: Text.Fit
                    horizontalAlignment: Text.AlignHCenter
                    verticalAlignment: Text.AlignVCenter
                }

                MouseArea {
                    anchors.fill: parent
                    onClicked: {
                        listView.currentIndex = index
                        parent.forceActiveFocus()
                    }
                }

                Behavior on height {
                    NumberAnimation { duration: 100 }
                }
            }

            section {
                property: 'version'
                delegate: Rectangle {
                    width: listView.width
                    implicitHeight: textSection.implicitHeight+20
                    color: '#3498DB'
                    radius: 10
                    border.color: 'black'

                    Text {
                        id: textSection

                        width: parent.width
                        height: 30
                        anchors.centerIn: parent

                        color: '#F0F3F4'

                        text: section
                        minimumPixelSize: 10
                        font.pixelSize: 24
                        font.bold: true
                        fontSizeMode: Text.Fit
                        horizontalAlignment: Text.AlignHCenter
                        verticalAlignment: Text.AlignVCenter
                    }

                    MouseArea {
                        anchors.fill: parent
                        onClicked: {
                            listView.toggleSection(section)
                        }
                    }
                }
            }

            Keys.onUpPressed: {
                modifyIndex(-1)
            }

            Keys.onDownPressed: {
                modifyIndex(1)
            }

            Keys.onPressed: {
                if (event.key===Qt.Key_PageUp)
                {
                    modifyIndex(-5)
                }
                if (event.key===Qt.Key_PageDown)
                {
                    modifyIndex(5)
                }
            }

            onCurrentIndexChanged: {
                musicId = model.get(currentIndex).id
            }

            function modifyIndex(distance)
            {
                listView.currentIndex = (listView.currentIndex+distance+model.count) % model.count
            }

            function isSectionExpanded(section)
            {
                return !(section in collapsed);
            }

            function enableSection(section, enabled)
            {
                if (enabled)
                {
                    delete collapsed[section];
                }
                else
                {
                    collapsed[section] = true;
                }
                collapsedChanged();
            }

            //'' using positionViewAtBeginning() cause 'Object 0x1eb65650 destroyed while one of its QML signal handlers is in progress'
            //'' called inside delegate, so it's not possible to position properly
            //'' (temporary delegate destoryed?)
            //'' unlike enableAllSections is called when button clicked.
            function toggleSection(section)
            {
                enableSection(section, !isSectionExpanded(section))
            }

            function enableAllSections(enabled)
            {
                if (sections)
                {
                    for (const section of sections)
                    {
                        enableSection(section, enabled)
                    }
                    listView.forceActiveFocus()
                    //'' known problem: currentSection may not be the one was containing index item.
                    //'' when scroll upward after section collapsed
                    //'' workaround: user click collapse all twice to center correctly.
                    positionViewAtBeginning()
                    positionViewAtIndex(listView.currentIndex, ListView.Center)
                }
            }
        }
    }
}


