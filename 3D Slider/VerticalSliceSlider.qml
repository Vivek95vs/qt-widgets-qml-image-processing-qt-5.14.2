// VerticalSliceSlider.qml
import QtQuick 2.12
import QtQuick.Controls 2.12

Item {
    id: root
    property int value: 0
    property int from: 0
    property int to: 100

    // step size controlled by the arrows
    property int sliceStepSize: 1
    property int minStepSize: 1
    property int maxStepSize: 20

    width: 44
    height: 360

    Rectangle {           // background
        anchors.fill: parent
        color: "black"
    }

    // vertical track
    Rectangle {
        id: track
        width: 4
        anchors.horizontalCenter: parent.horizontalCenter
        anchors.top: parent.top
        anchors.bottom: parent.bottom
        color: "#808080"
        radius: width / 2
    }

    // main slider (no default handle visible)
    Slider {
        id: slider
        anchors.fill: track
        orientation: Qt.Vertical
        from: root.from
        to: root.to
        value: root.value
        stepSize: root.sliceStepSize
        background: Item {}     // hide default background
        handle: Item {}         // hide default handle

        onValueChanged: root.value = value
    }

    // custom handle with arrows
    Rectangle {
        id: handle
        width: 36
        height: 70
        radius: 4
        color: "#253040"
        anchors.horizontalCenter: track.horizontalCenter

        y: track.y + (1 - (slider.visualPosition)) * (track.height - height)

        // drag to change slider value
        MouseArea {
            anchors.fill: parent
            drag.target: parent
            drag.axis: Drag.YAxis
            drag.minimumY: track.y
            drag.maximumY: track.y + track.height - parent.height

            onPositionChanged: {
                const pos = 1 - (parent.y - track.y) / (track.height - parent.height);
                slider.value = slider.from + pos * (slider.to - slider.from);
            }
        }

        // up arrow: increase step size
        MouseArea {
            id: upArrow
            width: parent.width
            height: parent.height / 3
            anchors.top: parent.top

            onClicked: {
                root.sliceStepSize = Math.min(root.sliceStepSize + 1, root.maxStepSize);
            }
        }

        // down arrow: decrease step size
        MouseArea {
            id: downArrow
            width: parent.width
            height: parent.height / 3
            anchors.bottom: parent.bottom

            onClicked: {
                root.sliceStepSize = Math.max(root.sliceStepSize - 1, root.minStepSize);
            }
        }

        // simple arrows and center icon (replace with your SVGs)
        Column {
            anchors.centerIn: parent
            spacing: 4
            Text { text: "▲"; color: "white"; font.pixelSize: 12 }
            Rectangle { width: 16; height: 2; color: "white" }
            Rectangle { width: 16; height: 2; color: "white" }
            Rectangle { width: 16; height: 2; color: "white" }
            Text { text: "▼"; color: "white"; font.pixelSize: 12 }
        }
    }
}
