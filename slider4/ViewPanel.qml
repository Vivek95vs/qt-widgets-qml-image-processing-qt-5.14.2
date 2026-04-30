import QtQuick 2.14
import QtQuick.Controls 2.14
import QtQuick.Layouts 1.14

Rectangle {
    id: root
    color: "#0f3460"
    radius: 8
    border.width: 1
    border.color: "#2d6bc6"

    property string viewName: "VIEW"
    property int sliceIndex: 0
    property int maxSlices: 0
    property string imageSource: ""

    signal sliceChanged(int newSlice)

    ColumnLayout {
        anchors.fill: parent
        anchors.margins: 10
        spacing: 5

        // Header
        Rectangle {
            Layout.fillWidth: true
            height: 30
            color: "transparent"

            RowLayout {
                anchors.fill: parent

                Text {
                    text: root.viewName
                    color: "#ffffff"
                    font.bold: true
                    font.pixelSize: 16
                }

                Item { Layout.fillWidth: true }

                Text {
                    text: root.maxSlices > 0 ? (root.sliceIndex + 1) + "/" + (root.maxSlices + 1) : "-/-"
                    color: "#90cdf4"
                    font.pixelSize: 14
                }
            }
        }

        // Image display
        Rectangle {
            Layout.fillWidth: true
            Layout.fillHeight: true
            color: "#000000"
            radius: 4
            clip: true

            Image {
                id: viewImage
                anchors.fill: parent
                source: root.imageSource
                fillMode: Image.PreserveAspectFit
                smooth: true
                mipmap: true
                cache: false

                onStatusChanged: {
                    if (status === Image.Error) {
                        console.log("Image loading error for", root.viewName, "source:", source)
                    }
                }
            }

            // Crosshair
            Rectangle {
                anchors.centerIn: parent
                width: parent.width
                height: 1
                color: "#ff0000"
                opacity: 0.5
                visible: viewImage.status === Image.Ready
            }

            Rectangle {
                anchors.centerIn: parent
                width: 1
                height: parent.height
                color: "#ff0000"
                opacity: 0.5
                visible: viewImage.status === Image.Ready
            }
        }

        // Slider controls
        Rectangle {
            Layout.fillWidth: true
            height: 60
            color: "transparent"

            ColumnLayout {
                anchors.fill: parent

                Slider {
                    id: sliceSlider
                    Layout.fillWidth: true
                    from: 0
                    to: root.maxSlices
                    stepSize: 1
                    value: root.sliceIndex
                    live: true
                    enabled: root.maxSlices > 0

                    background: Rectangle {
                        x: sliceSlider.leftPadding
                        y: sliceSlider.topPadding + sliceSlider.availableHeight / 2 - height / 2
                        implicitWidth: 200
                        implicitHeight: 4
                        width: sliceSlider.availableWidth
                        height: implicitHeight
                        radius: 2
                        color: "#2d3748"

                        Rectangle {
                            width: sliceSlider.visualPosition * parent.width
                            height: parent.height
                            color: "#4299e1"
                            radius: 2
                        }
                    }

                    handle: Rectangle {
                        x: sliceSlider.leftPadding + sliceSlider.visualPosition * (sliceSlider.availableWidth - width)
                        y: sliceSlider.topPadding + sliceSlider.availableHeight / 2 - height / 2
                        implicitWidth: 20
                        implicitHeight: 20
                        radius: 10
                        color: sliceSlider.pressed ? "#63b3ed" : "#4299e1"
                        border.color: "#ffffff"
                        border.width: 2
                    }

                    onMoved: {
                        root.sliceIndex = Math.round(value)
                        root.sliceChanged(root.sliceIndex)
                    }
                }

                RowLayout {
                    Layout.fillWidth: true

                    Button {
                        text: "◀◀"
                        Layout.preferredWidth: 40
                        enabled: root.maxSlices > 0
                        onClicked: {
                            var newValue = Math.max(0, sliceSlider.value - 10)
                            sliceSlider.value = newValue
                            root.sliceIndex = newValue
                            root.sliceChanged(root.sliceIndex)
                        }
                        background: Rectangle {
                            color: parent.down ? "#2d6bc6" :
                                   (parent.hovered ? "#4299e1" : "transparent")
                            radius: 4
                            border.color: "#4299e1"
                            border.width: 1
                        }
                        contentItem: Text {
                            text: parent.text
                            color: parent.enabled ? "#ffffff" : "#666666"
                            horizontalAlignment: Text.AlignHCenter
                            verticalAlignment: Text.AlignVCenter
                        }
                    }

                    Button {
                        text: "◀"
                        Layout.preferredWidth: 40
                        enabled: root.maxSlices > 0
                        onClicked: {
                            var newValue = Math.max(0, sliceSlider.value - 1)
                            sliceSlider.value = newValue
                            root.sliceIndex = newValue
                            root.sliceChanged(root.sliceIndex)
                        }
                        background: Rectangle {
                            color: parent.down ? "#2d6bc6" :
                                   (parent.hovered ? "#4299e1" : "transparent")
                            radius: 4
                            border.color: "#4299e1"
                            border.width: 1
                        }
                        contentItem: Text {
                            text: parent.text
                            color: parent.enabled ? "#ffffff" : "#666666"
                            horizontalAlignment: Text.AlignHCenter
                            verticalAlignment: Text.AlignVCenter
                        }
                    }

                    Item { Layout.fillWidth: true }

                    Button {
                        text: "▶"
                        Layout.preferredWidth: 40
                        enabled: root.maxSlices > 0
                        onClicked: {
                            var newValue = Math.min(root.maxSlices, sliceSlider.value + 1)
                            sliceSlider.value = newValue
                            root.sliceIndex = newValue
                            root.sliceChanged(root.sliceIndex)
                        }
                        background: Rectangle {
                            color: parent.down ? "#2d6bc6" :
                                   (parent.hovered ? "#4299e1" : "transparent")
                            radius: 4
                            border.color: "#4299e1"
                            border.width: 1
                        }
                        contentItem: Text {
                            text: parent.text
                            color: parent.enabled ? "#ffffff" : "#666666"
                            horizontalAlignment: Text.AlignHCenter
                            verticalAlignment: Text.AlignVCenter
                        }
                    }

                    Button {
                        text: "▶▶"
                        Layout.preferredWidth: 40
                        enabled: root.maxSlices > 0
                        onClicked: {
                            var newValue = Math.min(root.maxSlices, sliceSlider.value + 10)
                            sliceSlider.value = newValue
                            root.sliceIndex = newValue
                            root.sliceChanged(root.sliceIndex)
                        }
                        background: Rectangle {
                            color: parent.down ? "#2d6bc6" :
                                   (parent.hovered ? "#4299e1" : "transparent")
                            radius: 4
                            border.color: "#4299e1"
                            border.width: 1
                        }
                        contentItem: Text {
                            text: parent.text
                            color: parent.enabled ? "#ffffff" : "#666666"
                            horizontalAlignment: Text.AlignHCenter
                            verticalAlignment: Text.AlignVCenter
                        }
                    }
                }
            }
        }
    }

    // Update slider when sliceIndex changes from outside
    onSliceIndexChanged: {
        if (sliceSlider.value !== sliceIndex) {
            sliceSlider.value = sliceIndex
        }
    }
}
