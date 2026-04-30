// Updated main.qml - Replace the entire file with this:

import QtQuick 2.14
import QtQuick.Controls 2.14
import QtQuick.Layouts 1.14
import MedicalImaging 1.0

ApplicationWindow {
    id: mainWindow
    visible: true
    width: 1400
    height: 900
    title: "Medical Imaging Viewer - DICOM + 3D Volume"
    color: "#1e1e2e"

    // InfoRow Component
    Component {
        id: infoRowComponent

        RowLayout {
            property string labelText: ""
            property string valueText: ""

            Layout.fillWidth: true

            Text {
                text: labelText
                color: "#a0a0b0"
                font.pixelSize: 12
                Layout.preferredWidth: 100
            }

            Text {
                text: valueText
                color: "white"
                font.pixelSize: 12
                font.bold: true
                Layout.fillWidth: true
                horizontalAlignment: Text.AlignRight
            }
        }
    }

    // ControlRow Component
    Component {
        id: controlRowComponent

        Column {
            id: controlColumn
            property string label: ""
            property real fromValue: 0
            property real toValue: 100
            property real currentValue: 0
            property int decimals: 1

            signal valueChanged(real value)

            Layout.fillWidth: true
            spacing: 5

            Text {
                text: label + ": " + currentValue.toFixed(decimals)
                color: "white"
                font.pixelSize: 12
            }

            Slider {
                id: controlSlider
                width: parent.width
                from: fromValue
                to: toValue
                value: currentValue
                stepSize: (toValue - fromValue) / 100
                onValueChanged: controlColumn.valueChanged(value)

                background: Rectangle {
                    x: controlSlider.leftPadding
                    y: controlSlider.topPadding + controlSlider.availableHeight / 2 - height / 2
                    implicitWidth: 200
                    implicitHeight: 4
                    width: controlSlider.availableWidth
                    height: implicitHeight
                    radius: 2
                    color: "#3d3d4d"

                    Rectangle {
                        width: controlSlider.visualPosition * parent.width
                        height: parent.height
                        color: "#7DD3FC"
                        radius: 2
                    }
                }

                handle: Rectangle {
                    x: controlSlider.leftPadding + controlSlider.visualPosition * (controlSlider.availableWidth - width)
                    y: controlSlider.topPadding + controlSlider.availableHeight / 2 - height / 2
                    implicitWidth: 16
                    implicitHeight: 16
                    radius: 8
                    color: controlSlider.pressed ? "#f0f0f0" : "white"
                    border.color: "#7DD3FC"
                    border.width: 2
                }
            }
        }
    }

    // Sidebar
    Rectangle {
        id: sidebar
        width: 300
        anchors {
            top: parent.top
            bottom: parent.bottom
            left: parent.left
        }
        color: "#2d2d3d"

        ColumnLayout {
            anchors.fill: parent
            anchors.margins: 20
            spacing: 15

            // Title
            Text {
                text: "MEDICAL IMAGING"
                color: "#7DD3FC"
                font.pixelSize: 24
                font.bold: true
                Layout.alignment: Qt.AlignHCenter
            }

            // Separator
            Rectangle {
                Layout.fillWidth: true
                height: 2
                color: "#3d3d4d"
            }

            // Load DICOM section
            Column {
                Layout.fillWidth: true
                spacing: 10

                Text {
                    text: "DICOM LOADER"
                    color: "white"
                    font.pixelSize: 16
                    font.bold: true
                }

                Button {
                    text: "Load DICOM Folder"
                    Layout.fillWidth: true
                    height: 40
                    background: Rectangle {
                        color: parent.pressed ? "#0F766E" :
                               (parent.hovered ? "#14B8A6" : "#5EEAD4")
                        radius: 6
                    }
                    contentItem: Text {
                        text: parent.text
                        color: "white"
                        font.pixelSize: 14
                        font.bold: true
                        horizontalAlignment: Text.AlignHCenter
                        verticalAlignment: Text.AlignVCenter
                    }
                    onClicked: {
                        dicomHandler.loadDicomSeries("D:/Your/DICOM/Folder")
                    }
                }

                Text {
                    text: dicomHandler.seriesName || "No series loaded"
                    color: "#a0a0b0"
                    font.pixelSize: 12
                    elide: Text.ElideMiddle
                    width: parent.width
                }
            }

            // Volume information
            Column {
                id: volumeInfoColumn
                Layout.fillWidth: true
                spacing: 8
                visible: dicomHandler.sliceCount > 0

                Text {
                    text: "VOLUME INFO"
                    color: "white"
                    font.pixelSize: 16
                    font.bold: true
                }

                Loader {
                    sourceComponent: infoRowComponent
                    onLoaded: {
                        item.labelText = "Slices:"
                        item.valueText = dicomHandler.sliceCount
                    }
                }

                Loader {
                    sourceComponent: infoRowComponent
                    onLoaded: {
                        var dims = dicomHandler.volumeDimensions
                        item.labelText = "Dimensions:"
                        item.valueText = dims.x + "×" + dims.y + "×" + dims.z
                    }
                }

                Loader {
                    sourceComponent: infoRowComponent
                    onLoaded: {
                        item.labelText = "Window Width:"
                        item.valueText = dicomHandler.windowWidth.toFixed(1)
                    }
                }

                Loader {
                    sourceComponent: infoRowComponent
                    onLoaded: {
                        item.labelText = "Window Level:"
                        item.valueText = dicomHandler.windowCenter.toFixed(1)
                    }
                }
            }

            // Slice controls
            Column {
                Layout.fillWidth: true
                spacing: 10
                visible: dicomHandler.sliceCount > 0

                Text {
                    text: "SLICE NAVIGATION"
                    color: "white"
                    font.pixelSize: 16
                    font.bold: true
                }

                Slider {
                    id: sliceSlider
                    Layout.fillWidth: true
                    from: 0
                    to: Math.max(0, dicomHandler.sliceCount - 1)
                    stepSize: 1
                    value: 0
                    onValueChanged: {
                        dicomHandler.showSlice(value)
                    }

                    background: Rectangle {
                        x: parent.leftPadding
                        y: parent.topPadding + parent.availableHeight / 2 - height / 2
                        implicitWidth: 200
                        implicitHeight: 4
                        width: parent.availableWidth
                        height: implicitHeight
                        radius: 2
                        color: "#3d3d4d"

                        Rectangle {
                            width: parent.width * sliceSlider.visualPosition
                            height: parent.height
                            color: "#7DD3FC"
                            radius: 2
                        }
                    }

                    handle: Rectangle {
                        x: sliceSlider.leftPadding + sliceSlider.visualPosition *
                           (sliceSlider.availableWidth - width)
                        y: sliceSlider.topPadding + sliceSlider.availableHeight / 2 - height / 2
                        implicitWidth: 20
                        implicitHeight: 20
                        radius: 10
                        color: sliceSlider.pressed ? "#f0f0f0" : "white"
                        border.color: "#7DD3FC"
                        border.width: 2
                    }
                }

                RowLayout {
                    Layout.fillWidth: true

                    Text {
                        text: "Current: " + sliceSlider.value.toFixed(0)
                        color: "white"
                        font.pixelSize: 12
                    }

                    Text {
                        text: "Total: " + dicomHandler.sliceCount
                        color: "#a0a0b0"
                        font.pixelSize: 12
                        Layout.alignment: Qt.AlignRight
                    }
                }
            }

            // Window/Level controls
            Column {
                id: windowLevelColumn
                Layout.fillWidth: true
                spacing: 10
                visible: dicomHandler.sliceCount > 0

                Text {
                    text: "WINDOW/LEVEL"
                    color: "white"
                    font.pixelSize: 16
                    font.bold: true
                }

                Loader {
                    sourceComponent: controlRowComponent
                    onLoaded: {
                        item.label = "Width"
                        item.fromValue = 1
                        item.toValue = 2000
                        item.currentValue = dicomHandler.windowWidth
                        item.valueChanged.connect(function(value) {
                            dicomHandler.windowWidth = value
                        })
                    }
                }

                Loader {
                    sourceComponent: controlRowComponent
                    onLoaded: {
                        item.label = "Center"
                        item.fromValue = -1000
                        item.toValue = 3000
                        item.currentValue = dicomHandler.windowCenter
                        item.valueChanged.connect(function(value) {
                            dicomHandler.windowCenter = value
                        })
                    }
                }
            }

            // 3D Volume controls
            Column {
                id: volumeControlsColumn
                Layout.fillWidth: true
                spacing: 10
                visible: vtkRenderer && vtkRenderer.volumeLoaded

                Text {
                    text: "3D VOLUME"
                    color: "white"
                    font.pixelSize: 16
                    font.bold: true
                }

                Loader {
                    sourceComponent: controlRowComponent
                    onLoaded: {
                        item.label = "Opacity"
                        item.fromValue = 0
                        item.toValue = 1
                        item.currentValue = vtkRenderer.opacity
                        item.decimals = 2
                        item.valueChanged.connect(function(value) {
                            vtkRenderer.opacity = value
                        })
                    }
                }

                Loader {
                    sourceComponent: controlRowComponent
                    onLoaded: {
                        item.label = "Brightness"
                        item.fromValue = 0.1
                        item.toValue = 2
                        item.currentValue = vtkRenderer.brightness
                        item.decimals = 2
                        item.valueChanged.connect(function(value) {
                            vtkRenderer.brightness = value
                        })
                    }
                }

                ComboBox {
                    Layout.fillWidth: true
                    model: ["Composite", "Maximum", "Minimum"]
                    currentIndex: 0
                    onCurrentIndexChanged: {
                        if (vtkRenderer) {
                            vtkRenderer.setRenderMode(currentIndex)
                        }
                    }

                    background: Rectangle {
                        color: "#3d3d4d"
                        radius: 4
                        border.width: 1
                        border.color: "#4d4d5d"
                    }

                    contentItem: Text {
                        text: parent.currentText
                        color: "white"
                        font.pixelSize: 13
                        leftPadding: 10
                        verticalAlignment: Text.AlignVCenter
                    }

                    popup: Popup {
                        y: parent.height + 2
                        width: parent.width
                        implicitHeight: contentItem.implicitHeight + 10
                        padding: 5

                        contentItem: ListView {
                            clip: true
                            implicitHeight: contentHeight
                            model: parent.parent.model
                            currentIndex: parent.parent.currentIndex

                            delegate: ItemDelegate {
                                width: parent.width
                                text: modelData
                                highlighted: parent.currentIndex === index

                                background: Rectangle {
                                    color: highlighted ? "#7DD3FC" : "transparent"
                                }
                            }
                        }

                        background: Rectangle {
                            color: "#2d2d3d"
                            border.width: 1
                            border.color: "#4d4d5d"
                            radius: 4
                        }
                    }
                }

                Button {
                    text: "Reset View"
                    Layout.fillWidth: true
                    height: 35
                    onClicked: {
                        if (vtkRenderer) {
                            vtkRenderer.resetCamera()
                        }
                    }

                    background: Rectangle {
                        color: parent.pressed ? "#3d3d4d" :
                               (parent.hovered ? "#4d4d5d" : "#2d2d3d")
                        radius: 4
                        border.width: 1
                        border.color: "#4d4d5d"
                    }

                    contentItem: Text {
                        text: parent.text
                        color: "#7DD3FC"
                        font.pixelSize: 13
                        horizontalAlignment: Text.AlignHCenter
                        verticalAlignment: Text.AlignVCenter
                    }
                }
            }

            Item { Layout.fillHeight: true }

            // Status
            Text {
                text: "Status: " + (dicomHandler.sliceCount > 0 ? "Loaded" : "Ready")
                color: dicomHandler.sliceCount > 0 ? "#5EEAD4" : "#a0a0b0"
                font.pixelSize: 12
                Layout.alignment: Qt.AlignHCenter
            }
        }
    }

    // Main content area
    Rectangle {
        id: contentArea
        anchors {
            top: parent.top
            bottom: parent.bottom
            left: sidebar.right
            right: parent.right
        }
        color: "#1e1e2e"

        GridLayout {
            anchors.fill: parent
            anchors.margins: 20
            columns: 2
            rows: 2

            // 2D Slice View
            Rectangle {
                id: sliceViewContainer
                Layout.fillWidth: true
                Layout.fillHeight: true
                Layout.columnSpan: (vtkRenderer && vtkRenderer.volumeLoaded) ? 1 : 2
                color: "#2d2d3d"
                radius: 8

                ColumnLayout {
                    anchors.fill: parent
                    anchors.margins: 15

                    Text {
                        text: "2D SLICE VIEW"
                        color: "white"
                        font.pixelSize: 16
                        font.bold: true
                        Layout.alignment: Qt.AlignHCenter
                    }

                    Rectangle {
                        Layout.fillWidth: true
                        Layout.fillHeight: true
                        color: "black"
                        radius: 4

                        Image {
                            id: dicomImage
                            anchors.fill: parent
                            anchors.margins: 2
                            source: "image://dicom/" + sliceSlider.value
                            fillMode: Image.PreserveAspectFit
                            cache: false
                        }

                        // Crosshair
                        Rectangle {
                            anchors.centerIn: parent
                            width: parent.width * 0.8
                            height: 1
                            color: "#FFD700"
                            opacity: 0.5
                        }

                        Rectangle {
                            anchors.centerIn: parent
                            width: 1
                            height: parent.height * 0.8
                            color: "#FFD700"
                            opacity: 0.5
                        }
                    }
                }
            }

            // 3D Volume View (only shown when volume loaded)
            Rectangle {
                id: volumeViewContainer
                Layout.fillWidth: true
                Layout.fillHeight: true
                visible: vtkRenderer && vtkRenderer.volumeLoaded
                color: "#2d2d3d"
                radius: 8

                ColumnLayout {
                    anchors.fill: parent
                    anchors.margins: 15

                    Text {
                        text: "3D VOLUME RENDERING"
                        color: "white"
                        font.pixelSize: 16
                        font.bold: true
                        Layout.alignment: Qt.AlignHCenter
                    }

                    Rectangle {
                        Layout.fillWidth: true
                        Layout.fillHeight: true
                        color: "black"
                        radius: 4

                        Text {
                            anchors.centerIn: parent
                            text: "VTK 3D View\n(Integration in progress)"
                            color: "white"
                            font.pixelSize: 14
                            horizontalAlignment: Text.AlignHCenter
                        }
                    }
                }
            }

            // Viewport controls
            Rectangle {
                Layout.fillWidth: true
                Layout.columnSpan: 2
                height: 60
                color: "transparent"

                RowLayout {
                    anchors.fill: parent
                    spacing: 10

                    Button {
                        text: "Axial"
                        checked: true
                        onClicked: console.log("Axial view")
                        checkable: true
                    }

                    Button {
                        text: "Coronal"
                        onClicked: console.log("Coronal view")
                        checkable: true
                    }

                    Button {
                        text: "Sagittal"
                        onClicked: console.log("Sagittal view")
                        checkable: true
                    }

                    Item { Layout.fillWidth: true }

                    Button {
                        text: "Save Slice"
                        onClicked: {
                            console.log("Save slice clicked")
                        }
                    }

                    Button {
                        text: "Export Volume"
                        onClicked: console.log("Export volume")
                    }
                }
            }
        }
    }
}
