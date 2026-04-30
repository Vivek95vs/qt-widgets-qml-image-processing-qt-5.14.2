import QtQuick 2.14
import QtQuick.Controls 2.14
import QtQuick.Layouts 1.14

Rectangle {
    id: root
    color: "#16213e"
    radius: 8
    border.width: 1
    border.color: "#2d6bc6"

    property var dicomHandler: null

    ColumnLayout {
        anchors.fill: parent
        anchors.margins: 15

        Text {
            text: "VOLUME CONTROLS"
            color: "#ffffff"
            font.bold: true
            font.pixelSize: 16
            Layout.bottomMargin: 10
        }

        // Brightness/Contrast
        GroupBox {
            Layout.fillWidth: true
            title: "Image Adjustment"
            background: Rectangle {
                color: "#1a1a2e"
                radius: 4
            }
            label: Text {
                text: parent.title
                color: "#90cdf4"
                font.bold: true
                leftPadding: 5
            }

            ColumnLayout {
                width: parent.width

                // Custom slider component defined inline
                ColumnLayout {
                    Layout.fillWidth: true
                    property string label: "Brightness"
                    property real from: -100
                    property real to: 100
                    property real value: 0

                    RowLayout {
                        Layout.fillWidth: true

                        Text {
                            text: parent.parent.label
                            color: "#e2e8f0"
                            font.pixelSize: 12
                            Layout.fillWidth: true
                        }

                        Text {
                            text: parent.parent.value.toFixed(1)
                            color: "#90cdf4"
                            font.pixelSize: 12
                            font.bold: true
                        }
                    }

                    Slider {
                        Layout.fillWidth: true
                        from: parent.parent.from
                        to: parent.parent.to
                        value: parent.parent.value
                        stepSize: (parent.parent.to - parent.parent.from) / 100

                        background: Rectangle {
                            x: parent.leftPadding
                            y: parent.topPadding + parent.availableHeight / 2 - height / 2
                            implicitWidth: 200
                            implicitHeight: 4
                            width: parent.availableWidth
                            height: implicitHeight
                            radius: 2
                            color: "#2d3748"

                            Rectangle {
                                width: parent.parent.visualPosition * parent.width
                                height: parent.height
                                color: "#4299e1"
                                radius: 2
                            }
                        }

                        handle: Rectangle {
                            x: parent.leftPadding + parent.visualPosition * (parent.availableWidth - width)
                            y: parent.topPadding + parent.availableHeight / 2 - height / 2
                            implicitWidth: 16
                            implicitHeight: 16
                            radius: 8
                            color: parent.pressed ? "#63b3ed" : "#4299e1"
                            border.color: "#ffffff"
                            border.width: 2
                        }

                        onValueChanged: parent.parent.value = value
                    }
                }

                ColumnLayout {
                    Layout.fillWidth: true
                    property string label: "Contrast"
                    property real from: 0.5
                    property real to: 2.0
                    property real value: 1.0

                    RowLayout {
                        Layout.fillWidth: true

                        Text {
                            text: parent.parent.label
                            color: "#e2e8f0"
                            font.pixelSize: 12
                            Layout.fillWidth: true
                        }

                        Text {
                            text: parent.parent.value.toFixed(1)
                            color: "#90cdf4"
                            font.pixelSize: 12
                            font.bold: true
                        }
                    }

                    Slider {
                        Layout.fillWidth: true
                        from: parent.parent.from
                        to: parent.parent.to
                        value: parent.parent.value
                        stepSize: (parent.parent.to - parent.parent.from) / 100

                        background: Rectangle {
                            x: parent.leftPadding
                            y: parent.topPadding + parent.availableHeight / 2 - height / 2
                            implicitWidth: 200
                            implicitHeight: 4
                            width: parent.availableWidth
                            height: implicitHeight
                            radius: 2
                            color: "#2d3748"

                            Rectangle {
                                width: parent.parent.visualPosition * parent.width
                                height: parent.height
                                color: "#4299e1"
                                radius: 2
                            }
                        }

                        handle: Rectangle {
                            x: parent.leftPadding + parent.visualPosition * (parent.availableWidth - width)
                            y: parent.topPadding + parent.availableHeight / 2 - height / 2
                            implicitWidth: 16
                            implicitHeight: 16
                            radius: 8
                            color: parent.pressed ? "#63b3ed" : "#4299e1"
                            border.color: "#ffffff"
                            border.width: 2
                        }

                        onValueChanged: parent.parent.value = value
                    }
                }

                ColumnLayout {
                    Layout.fillWidth: true
                    property string label: "Gamma"
                    property real from: 0.5
                    property real to: 2.5
                    property real value: 1.0

                    RowLayout {
                        Layout.fillWidth: true

                        Text {
                            text: parent.parent.label
                            color: "#e2e8f0"
                            font.pixelSize: 12
                            Layout.fillWidth: true
                        }

                        Text {
                            text: parent.parent.value.toFixed(1)
                            color: "#90cdf4"
                            font.pixelSize: 12
                            font.bold: true
                        }
                    }

                    Slider {
                        Layout.fillWidth: true
                        from: parent.parent.from
                        to: parent.parent.to
                        value: parent.parent.value
                        stepSize: (parent.parent.to - parent.parent.from) / 100

                        background: Rectangle {
                            x: parent.leftPadding
                            y: parent.topPadding + parent.availableHeight / 2 - height / 2
                            implicitWidth: 200
                            implicitHeight: 4
                            width: parent.availableWidth
                            height: implicitHeight
                            radius: 2
                            color: "#2d3748"

                            Rectangle {
                                width: parent.parent.visualPosition * parent.width
                                height: parent.height
                                color: "#4299e1"
                                radius: 2
                            }
                        }

                        handle: Rectangle {
                            x: parent.leftPadding + parent.visualPosition * (parent.availableWidth - width)
                            y: parent.topPadding + parent.availableHeight / 2 - height / 2
                            implicitWidth: 16
                            implicitHeight: 16
                            radius: 8
                            color: parent.pressed ? "#63b3ed" : "#4299e1"
                            border.color: "#ffffff"
                            border.width: 2
                        }

                        onValueChanged: parent.parent.value = value
                    }
                }
            }
        }

        // Window Level
        GroupBox {
            Layout.fillWidth: true
            Layout.topMargin: 10
            title: "Window Level"
            background: Rectangle {
                color: "#1a1a2e"
                radius: 4
            }
            label: Text {
                text: parent.title
                color: "#90cdf4"
                font.bold: true
                leftPadding: 5
            }

            ColumnLayout {
                width: parent.width

                ColumnLayout {
                    id: windowSlider
                    Layout.fillWidth: true
                    property string label: "Window Width"
                    property real from: 1
                    property real to: 4000
                    property real value: 2000

                    RowLayout {
                        Layout.fillWidth: true

                        Text {
                            text: parent.parent.label
                            color: "#e2e8f0"
                            font.pixelSize: 12
                            Layout.fillWidth: true
                        }

                        Text {
                            text: parent.parent.value.toFixed(1)
                            color: "#90cdf4"
                            font.pixelSize: 12
                            font.bold: true
                        }
                    }

                    Slider {
                        Layout.fillWidth: true
                        from: parent.parent.from
                        to: parent.parent.to
                        value: parent.parent.value
                        stepSize: (parent.parent.to - parent.parent.from) / 100

                        background: Rectangle {
                            x: parent.leftPadding
                            y: parent.topPadding + parent.availableHeight / 2 - height / 2
                            implicitWidth: 200
                            implicitHeight: 4
                            width: parent.availableWidth
                            height: implicitHeight
                            radius: 2
                            color: "#2d3748"

                            Rectangle {
                                width: parent.parent.visualPosition * parent.width
                                height: parent.height
                                color: "#4299e1"
                                radius: 2
                            }
                        }

                        handle: Rectangle {
                            x: parent.leftPadding + parent.visualPosition * (parent.availableWidth - width)
                            y: parent.topPadding + parent.availableHeight / 2 - height / 2
                            implicitWidth: 16
                            implicitHeight: 16
                            radius: 8
                            color: parent.pressed ? "#63b3ed" : "#4299e1"
                            border.color: "#ffffff"
                            border.width: 2
                        }

                        onValueChanged: parent.parent.value = value
                    }
                }

                ColumnLayout {
                    id: levelSlider
                    Layout.fillWidth: true
                    property string label: "Window Level"
                    property real from: -1000
                    property real to: 3000
                    property real value: 500

                    RowLayout {
                        Layout.fillWidth: true

                        Text {
                            text: parent.parent.label
                            color: "#e2e8f0"
                            font.pixelSize: 12
                            Layout.fillWidth: true
                        }

                        Text {
                            text: parent.parent.value.toFixed(1)
                            color: "#90cdf4"
                            font.pixelSize: 12
                            font.bold: true
                        }
                    }

                    Slider {
                        Layout.fillWidth: true
                        from: parent.parent.from
                        to: parent.parent.to
                        value: parent.parent.value
                        stepSize: (parent.parent.to - parent.parent.from) / 100

                        background: Rectangle {
                            x: parent.leftPadding
                            y: parent.topPadding + parent.availableHeight / 2 - height / 2
                            implicitWidth: 200
                            implicitHeight: 4
                            width: parent.availableWidth
                            height: implicitHeight
                            radius: 2
                            color: "#2d3748"

                            Rectangle {
                                width: parent.parent.visualPosition * parent.width
                                height: parent.height
                                color: "#4299e1"
                                radius: 2
                            }
                        }

                        handle: Rectangle {
                            x: parent.leftPadding + parent.visualPosition * (parent.availableWidth - width)
                            y: parent.topPadding + parent.availableHeight / 2 - height / 2
                            implicitWidth: 16
                            implicitHeight: 16
                            radius: 8
                            color: parent.pressed ? "#63b3ed" : "#4299e1"
                            border.color: "#ffffff"
                            border.width: 2
                        }

                        onValueChanged: parent.parent.value = value
                    }
                }

                ComboBox {
                    Layout.fillWidth: true
                    model: ["Soft Tissue", "Bone", "Lung", "Brain", "Custom"]
                    currentIndex: 0
                    background: Rectangle {
                        color: "#2d3748"
                        radius: 4
                    }
                    contentItem: Text {
                        text: parent.displayText
                        color: "#ffffff"
                        leftPadding: 10
                        verticalAlignment: Text.AlignVCenter
                    }
                    onCurrentIndexChanged: {
                        switch(currentIndex) {
                            case 0: // Soft Tissue
                                windowSlider.value = 400
                                levelSlider.value = 40
                                break
                            case 1: // Bone
                                windowSlider.value = 2000
                                levelSlider.value = 500
                                break
                            case 2: // Lung
                                windowSlider.value = 1500
                                levelSlider.value = -600
                                break
                            case 3: // Brain
                                windowSlider.value = 80
                                levelSlider.value = 40
                                break
                        }
                    }
                }
            }
        }

        // Sync Views
        GroupBox {
            Layout.fillWidth: true
            Layout.topMargin: 10
            title: "View Synchronization"
            background: Rectangle {
                color: "#1a1a2e"
                radius: 4
            }
            label: Text {
                text: parent.title
                color: "#90cdf4"
                font.bold: true
                leftPadding: 5
            }

            Column {
                width: parent.width
                spacing: 8

                CheckBox {
                    id: syncViews
                    text: "Sync Slice Navigation"
                    checked: true
                    contentItem: Text {
                        text: parent.text
                        color: "#e2e8f0"
                        leftPadding: parent.indicator.width + parent.spacing
                        verticalAlignment: Text.AlignVCenter
                    }
                }

                CheckBox {
                    text: "Show Crosshairs"
                    checked: true
                    contentItem: Text {
                        text: parent.text
                        color: "#e2e8f0"
                        leftPadding: parent.indicator.width + parent.spacing
                        verticalAlignment: Text.AlignVCenter
                    }
                }

                CheckBox {
                    text: "Show Ruler"
                    checked: false
                    contentItem: Text {
                        text: parent.text
                        color: "#e2e8f0"
                        leftPadding: parent.indicator.width + parent.spacing
                        verticalAlignment: Text.AlignVCenter
                    }
                }
            }
        }

        // Reset button
        Button {
            Layout.topMargin: 20
            Layout.fillWidth: true
            text: "⟳ Reset All Views"
            padding: 12
            background: Rectangle {
                color: parent.down ? "#2d6bc6" :
                       (parent.hovered ? "#4299e1" : "#0f3460")
                radius: 6
                border.width: 1
                border.color: "#2d6bc6"
            }
            contentItem: Text {
                text: parent.text
                color: "white"
                horizontalAlignment: Text.AlignHCenter
                verticalAlignment: Text.AlignVCenter
                font.bold: true
            }
            onClicked: {
                if (dicomHandler) {
                    dicomHandler.setCurrentAxialSlice(Math.floor(dicomHandler.volumeDimensions.z / 2))
                    dicomHandler.setCurrentCoronalSlice(Math.floor(dicomHandler.volumeDimensions.y / 2))
                    dicomHandler.setCurrentSagittalSlice(Math.floor(dicomHandler.volumeDimensions.x / 2))
                }
            }
        }

        Item { Layout.fillHeight: true }
    }
}
