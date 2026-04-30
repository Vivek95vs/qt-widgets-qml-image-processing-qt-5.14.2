import QtQuick 2.12
Rectangle {
    id: root
    property string tooltipText: "Tooltip"
    property bool show: false

    visible: show
    radius: 6
    color: "#1f2937"
    width: textItem.paintedWidth + 16
    height: textItem.paintedHeight + 12
    z: 999

    Text {
        id: textItem
        anchors.centerIn: parent
        text: root.tooltipText
        color: "white"
        font.pixelSize: 12
    }
}
