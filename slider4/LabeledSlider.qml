import QtQuick 2.14
import QtQuick.Controls 2.14
import QtQuick.Layouts 1.14

ColumnLayout {
    id: root
    property string label: ""
    property real from: 0
    property real to: 100
    property real value: 50

    Layout.fillWidth: true

    RowLayout {
        Layout.fillWidth: true

        Text {
            text: root.label
            color: "#e2e8f0"
            font.pixelSize: 12
            Layout.fillWidth: true
        }

        Text {
            text: root.value.toFixed(1)
            color: "#90cdf4"
            font.pixelSize: 12
            font.bold: true
        }
    }

    Slider {
        id: slider
        Layout.fillWidth: true
        from: root.from
        to: root.to
        value: root.value
        stepSize: (root.to - root.from) / 100

        background: Rectangle {
            x: slider.leftPadding
            y: slider.topPadding + slider.availableHeight / 2 - height / 2
            implicitWidth: 200
            implicitHeight: 4
            width: slider.availableWidth
            height: implicitHeight
            radius: 2
            color: "#2d3748"

            Rectangle {
                width: slider.visualPosition * parent.width
                height: parent.height
                color: "#4299e1"
                radius: 2
            }
        }

        handle: Rectangle {
            x: slider.leftPadding + slider.visualPosition * (slider.availableWidth - width)
            y: slider.topPadding + slider.availableHeight / 2 - height / 2
            implicitWidth: 16
            implicitHeight: 16
            radius: 8
            color: slider.pressed ? "#63b3ed" : "#4299e1"
            border.color: "#ffffff"
            border.width: 2
        }

        onValueChanged: root.value = value
    }
}
