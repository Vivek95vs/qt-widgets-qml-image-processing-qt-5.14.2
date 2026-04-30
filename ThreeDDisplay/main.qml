import QtQuick 2.14
import QtQuick.Controls 2.14
import QtQuick.Layouts 1.14
import VTK 9.2

ApplicationWindow {
    visible: true
    visibility: "FullScreen"
    width: 1200
    height: 800
    title: "DICOM 3-View Viewer"
    color: "#1e1e1e"

    property string defaultDicomPath: "D:/DICOM_SERIES"
//    property string defaultDicomPath: "D:/Dental CBCT/Dental Data"
    property bool showCrosshair: true
    property color crosshairColor: "red"

    // Orientation properties
    property bool flipCoronalHorizontal: true
    property bool flipCoronalVertical: true
    property bool flipSagittalVertical: true


    VTKRenderWindow {
        id: vtkWindow
    }

    // Header
    Rectangle {
        id: header
        width: parent.width
        height: 140
        color: "#2d2d30"

        ColumnLayout {
            anchors.fill: parent
            anchors.margins: 10
            spacing: 5

            // Row 1: Load button and basic info
            RowLayout {
                Layout.fillWidth: true
                spacing: 15

                Button {
                    text: "📁 Load DICOM Series"
                    font.pixelSize: 14
                    implicitHeight: 40
                    onClicked: {
                        dicomHandler.loadDicomSeries(defaultDicomPath);
                        VolumeRenderer.startRender(defaultDicomPath)
                        statusText.text = "Loading DICOM...";
                        statusText.color = "#FF9800";
                    }

                    background: Rectangle {
                        color: parent.down ? "#005a9e" : parent.hovered ? "#0078d7" : "#007acc"
                        radius: 4
                    }
                }

                Label {
                    text: "Fixed Thresholds: " + dicomHandler.lowerThreshold + " to " + dicomHandler.upperThreshold
                    color: "#4CAF50"
                    font.bold: true
                    font.pixelSize: 14
                }

                // Orientation controls
//                Row {
//                    spacing: 10
//                    Label {
//                        text: "Orientation:"
//                        color: "white"
//                        anchors.verticalCenter: parent.verticalCenter
//                    }
//                    CheckBox {
//                        text: "Flip Coronal"
//                        checked: flipCoronalHorizontal && flipCoronalVertical
//                        onCheckedChanged: {
//                            flipCoronalHorizontal = checked;
//                            flipCoronalVertical = checked;
//                            refreshImages();
//                        }
//                    }
//                    CheckBox {
//                        text: "Flip Sagittal"
//                        checked: flipSagittalVertical
//                        onCheckedChanged: {
//                            flipSagittalVertical = checked;
//                            refreshImages();
//                        }
//                    }
//                }
            }

            // Row 2: Window/Level controls
            RowLayout {
                Layout.fillWidth: true
                spacing: 15

                // Window control
                Column {
                    Layout.fillWidth: true
                    spacing: 2

                    Label {
                        text: "Window: " + Math.round(windowSlider.value)
                        color: "white"
                        font.pixelSize: 11
                    }

                    Slider {
                        id: windowSlider
                        width: parent.width
                        from: 100
                        to: 4000
                        value: dicomHandler.window
                        stepSize: 10
                        onValueChanged: {
                            dicomHandler.setWindowLevel(value, levelSlider.value);
                            updateThresholdDisplay();
                        }
                    }
                }

                // Level control
                Column {
                    Layout.fillWidth: true
                    spacing: 2

                    Label {
                        text: "Level: " + Math.round(levelSlider.value)
                        color: "white"
                        font.pixelSize: 11
                    }

                    Slider {
                        id: levelSlider
                        width: parent.width
                        from: -2000
                        to: 2000
                        value: dicomHandler.level
                        stepSize: 10
                        onValueChanged: {
                            dicomHandler.setWindowLevel(windowSlider.value, value);
                            updateThresholdDisplay();
                        }
                    }
                }

                // Current thresholds
                Column {
                    Layout.preferredWidth: 200
                    spacing: 2

                    Label {
                        text: "Current Thresholds:"
                        color: "#888"
                        font.pixelSize: 11
                    }

                    Label {
                        id: currentThresholds
                        text: "Lower: -1000\nUpper: 1000"
                        color: "#FF9800"
                        font.pixelSize: 12
                        font.bold: true
                    }
                }
            }

            // Row 3: Presets and status
            RowLayout {
                Layout.fillWidth: true

                // Preset buttons
                Row {
                    spacing: 5

//                    Button {
//                        text: "CT Abdomen"
//                        onClicked: {
//                            windowSlider.value = 400;
//                            levelSlider.value = 40;
//                        }
//                    }

//                    Button {
//                        text: "CT Lung"
//                        onClicked: {
//                            windowSlider.value = 1500;
//                            levelSlider.value = -600;
//                        }
//                    }

//                    Button {
//                        text: "CT Bone"
//                        onClicked: {
//                            windowSlider.value = 2000;
//                            levelSlider.value = 400;
//                        }
//                    }

//                    Button {
//                        text: "Reset WL"
//                        onClicked: {
//                            windowSlider.value = 2000;
//                            levelSlider.value = 0;
//                        }
//                    }
                }

                Item { Layout.fillWidth: true }

                Label {
                    id: statusText
                    text: "Ready to load DICOM"
                    color: "#4CAF50"
                    font.pixelSize: 12
                }
            }
        }
    }

    GridLayout {
        anchors.top: header.bottom
        anchors.bottom: footer.top
        anchors.left: parent.left
        anchors.right: parent.right
        anchors.margins: 10
        columns: 3
        rows: 1
        columnSpacing: 10
        //3D Render
        Rectangle {
            id: vtkContainer
            Layout.fillWidth: true
            Layout.fillHeight: true
            color: "transparent"

            ColumnLayout {

                anchors.fill: parent
                anchors.margins: 5
                spacing: 5

                Label {
                    text: "3D Render"
                    color: "White"
                    font.bold: true
                    font.pixelSize: 14
                    Layout.alignment: Qt.AlignHCenter
                }

            VTKRenderItem {
                id: vtkView3D
//                anchors.fill: parent
                Layout.fillWidth: true
                Layout.fillHeight: true  // This will make it expand
                renderWindow: vtkWindow
                Component.onCompleted: VolumeRenderer.setRenderer(vtkWindow)
            }


            }
        }

        // AXIAL VIEW
        Rectangle {
            Layout.fillWidth: true
            Layout.fillHeight: true
            color: "#252526"
            radius: 5

            ColumnLayout {
                anchors.fill: parent
                anchors.margins: 5
                spacing: 5

                Label {
                    text: "AXIAL (Z = " + Math.round(dicomHandler.crosshairPosition.z) + ")"
                    color: "#4CAF50"
                    font.bold: true
                    font.pixelSize: 14
                    Layout.alignment: Qt.AlignHCenter
                }

                // Image display - No flip for axial
                Rectangle {
                    Layout.fillWidth: true
                    Layout.fillHeight: true
                    color: "black"

                    Image {
                        id: axialImage
                        anchors.fill: parent
                        source: "image://dicom/axial_" + Date.now()
                        fillMode: Image.PreserveAspectFit
                        cache: false

                        MouseArea {
                            anchors.fill: parent
                            onClicked: {
                                if (axialImage.sourceSize.width > 0) {
                                    var normX = mouse.x / parent.width;
                                    var normY = mouse.y / parent.height;
                                    var pos = Qt.vector3d(
                                        normX * axialImage.sourceSize.width,
                                        normY * axialImage.sourceSize.height,
                                        axialSlider.value
                                    );
                                    dicomHandler.crosshairPosition = pos;
                                }
                            }
                        }
                    }

                    BusyIndicator {
                        anchors.centerIn: parent
                        running: axialImage.status === Image.Loading
                        visible: running
                    }
                }
                // Slider for axial view
                ColumnLayout  {
                    Layout.fillWidth: true
                    spacing: 2

                    Label {
                        text: "Slice: " + Math.round(axialSlider.value) + " / " + Math.round(axialSlider.to)
                        color: "white"
                        font.pixelSize: 12
                        Layout.alignment: Qt.AlignHCenter
                    }

                    Slider {
                        id: axialSlider
                        Layout.fillWidth: true
                        from: 0
                        to: Math.max(dicomHandler.getMaxSliceCount(0) - 1, 1)
                        value: dicomHandler.crosshairPosition.z
                        stepSize: 1
                        live: true
                        onMoved: {
                            var pos = dicomHandler.crosshairPosition;
                            pos.z = value;
                            dicomHandler.crosshairPosition = pos;
                        }
                    }
                }
            }
        }

        // CORONAL VIEW (with flipping)
        Rectangle {
            Layout.fillWidth: true
            Layout.fillHeight: true
            color: "#252526"
            radius: 5

            ColumnLayout {
                anchors.fill: parent
                anchors.margins: 5
                spacing: 5

                Label {
                    text: "CORONAL (Y = " + Math.round(dicomHandler.crosshairPosition.y) + ")"
                    color: "#2196F3"
                    font.bold: true
                    font.pixelSize: 14
                    Layout.alignment: Qt.AlignHCenter
                }

                // Image display with flipping
                Rectangle {
                    Layout.fillWidth: true
                    Layout.fillHeight: true
                    color: "black"

                    Item {
                        id: coronalContainer
                        anchors.fill: parent

                        Image {
                            id: coronalImage
                            anchors.fill: parent
                            source: "image://dicom/coronal_" + Date.now()
                            fillMode: Image.PreserveAspectFit
                            cache: false

//                            // Apply transformations based on orientation
//                            transform: [
//                                Scale {
//                                    xScale: flipCoronalHorizontal ? -1 : 1
//                                    yScale: flipCoronalVertical ? -1 : 1
//                                    origin.x: coronalImage.width / 2
//                                    origin.y: coronalImage.height / 2
//                                }
//                            ]
                        }

                        MouseArea {
                            anchors.fill: parent
                            onClicked: {
                                if (coronalImage.sourceSize.width > 0) {
                                    // Adjust coordinates based on flipping
                                    var adjX = flipCoronalHorizontal ?
                                        coronalImage.width - mouse.x : mouse.x;
                                    var adjY = flipCoronalVertical ?
                                        coronalImage.height - mouse.y : mouse.y;

                                    var normX = adjX / parent.width;
                                    var normY = adjY / parent.height;
                                    var pos = Qt.vector3d(
                                        normX * coronalImage.sourceSize.width,
                                        coronalSlider.value,
                                        normY * coronalImage.sourceSize.height
                                    );
                                    dicomHandler.crosshairPosition = pos;
                                }
                            }
                        }
                    }

                    BusyIndicator {
                        anchors.centerIn: parent
                        running: coronalImage.status === Image.Loading
                        visible: running
                    }
                }

                // Slider for coronal view
                ColumnLayout {
                    Layout.fillWidth: true
                    spacing: 2

                    Label {
                        text: "Slice: " + Math.round(coronalSlider.value) + " / " + Math.round(coronalSlider.to)
                        color: "white"
                        font.pixelSize: 12
                        Layout.alignment: Qt.AlignHCenter
                    }

                    Slider {
                        id: coronalSlider
                        Layout.fillWidth: true
                        from: 0
                        to: Math.max(dicomHandler.getMaxSliceCount(1) - 1, 1)
                        value: dicomHandler.crosshairPosition.y
                        stepSize: 1
                        live: true
                        onMoved: {
                            var pos = dicomHandler.crosshairPosition;
                            pos.y = value;
                            dicomHandler.crosshairPosition = pos;
                        }
                    }
                }
            }
        }

        // SAGITTAL VIEW (with flipping)
        Rectangle {
            Layout.fillWidth: true
            Layout.fillHeight: true
            color: "#252526"
            radius: 5

            ColumnLayout {
                anchors.fill: parent
                anchors.margins: 5
                spacing: 5

                Label {
                    text: "SAGITTAL (X = " + Math.round(dicomHandler.crosshairPosition.x) + ")"
                    color: "#FF9800"
                    font.bold: true
                    font.pixelSize: 14
                    Layout.alignment: Qt.AlignHCenter
                }

                // Image display with flipping
                Rectangle {
                    Layout.fillWidth: true
                    Layout.fillHeight: true
                    color: "black"

                    Item {
                        id: sagittalContainer
                        anchors.fill: parent

                        Image {
                            id: sagittalImage
                            anchors.fill: parent
                            source: "image://dicom/sagittal_" + Date.now()
                            fillMode: Image.PreserveAspectFit
                            cache: false

//                            // Apply vertical flip only (as per your code)
//                            transform: [
//                                Scale {
//                                    xScale: 1  // No horizontal flip for sagittal
//                                    yScale: flipSagittalVertical ? -1 : 1
//                                    origin.x: sagittalImage.width / 2
//                                    origin.y: sagittalImage.height / 2
//                                }
//                            ]
                        }

                        MouseArea {
                            anchors.fill: parent
                            onClicked: {
                                if (sagittalImage.sourceSize.width > 0) {
                                    // Adjust coordinates based on flipping
                                    var adjY = flipSagittalVertical ?
                                        sagittalImage.height - mouse.y : mouse.y;

                                    var normX = mouse.x / parent.width;
                                    var normY = adjY / parent.height;
                                    var pos = Qt.vector3d(
                                        sagittalSlider.value,
                                        normX * sagittalImage.sourceSize.width,
                                        normY * sagittalImage.sourceSize.height
                                    );
                                    dicomHandler.crosshairPosition = pos;
                                }
                            }
                        }
                    }

                    BusyIndicator {
                        anchors.centerIn: parent
                        running: sagittalImage.status === Image.Loading
                        visible: running
                    }
                }

                // Slider for sagittal view
                ColumnLayout {
                    Layout.fillWidth: true
                    spacing: 2

                    Label {
                        text: "Slice: " + Math.round(sagittalSlider.value) + " / " + Math.round(sagittalSlider.to)
                        color: "white"
                        font.pixelSize: 12
                        Layout.alignment: Qt.AlignHCenter
                    }

                    Slider {
                        id: sagittalSlider
                        Layout.fillWidth: true
                        from: 0
                        to: Math.max(dicomHandler.getMaxSliceCount(2) - 1, 1)
                        value: dicomHandler.crosshairPosition.x
                        stepSize: 1
                        live: true
                        onMoved: {
                            var pos = dicomHandler.crosshairPosition;
                            pos.x = value;
                            dicomHandler.crosshairPosition = pos;
                        }
                    }
                }
            }
        }
    }

    // Footer
    Rectangle {
        id: footer
        anchors.bottom: parent.bottom
        width: parent.width
        height: 80
        color: "#2d2d30"

        GridLayout {
            anchors.fill: parent
            anchors.margins: 10
            columns: 4
            columnSpacing: 20

            // Position info
            Column {
                Label {
                    text: "Position (mm):"
                    color: "#888"
                    font.pixelSize: 11
                }
                Label {
                    text: {
                        if (!dicomHandler.volumeLoaded) return "N/A";
                        var pos = dicomHandler.crosshairPosition;
                        var spacing = dicomHandler.getVoxelSpacing();
                        return pos.x.toFixed(1) + ", " + pos.y.toFixed(1) + ", " + pos.z.toFixed(1);
                    }
                    color: "white"
                    font.pixelSize: 12
                    font.bold: true
                }
            }

            // Voxel coordinates
            Column {
                Label {
                    text: "Voxel (X,Y,Z):"
                    color: "#888"
                    font.pixelSize: 11
                }
                Label {
                    text: {
                        if (!dicomHandler.volumeLoaded) return "N/A";
                        var pos = dicomHandler.crosshairPosition;
                        return Math.round(pos.x) + ", " + Math.round(pos.y) + ", " + Math.round(pos.z);
                    }
                    color: "white"
                    font.pixelSize: 12
                    font.bold: true
                }
            }

            // Dimensions
            Column {
                Label {
                    text: "Dimensions:"
                    color: "#888"
                    font.pixelSize: 11
                }
                Label {
                    text: {
                        if (!dicomHandler.volumeLoaded) return "N/A";
                        var dims = dicomHandler.getVolumeDimensions();
                        return dims.x + " × " + dims.y + " × " + dims.z;
                    }
                    color: "white"
                    font.pixelSize: 12
                    font.bold: true
                }
            }

            // Spacing
            Column {
                Label {
                    text: "Spacing (mm):"
                    color: "#888"
                    font.pixelSize: 11
                }
                Label {
                    text: {
                        if (!dicomHandler.volumeLoaded) return "N/A";
                        var spacing = dicomHandler.getVoxelSpacing();
                        return spacing.x.toFixed(2) + " × " + spacing.y.toFixed(2) + " × " + spacing.z.toFixed(2);
                    }
                    color: "white"
                    font.pixelSize: 12
                    font.bold: true
                }
            }
        }
    }

    // Connections
    Connections {
        target: dicomHandler
        onVolumeLoadedChanged: {
            if (dicomHandler.volumeLoaded) {
                // Initialize sliders
                console.log("Dicom Loaded successfully")
                axialSlider.to = Math.max(dicomHandler.getMaxSliceCount(0) - 1, 1);
                coronalSlider.to = Math.max(dicomHandler.getMaxSliceCount(1) - 1, 1);
                sagittalSlider.to = Math.max(dicomHandler.getMaxSliceCount(2) - 1, 1);

                statusText.text = "DICOM Loaded Successfully";
                statusText.color = "#4CAF50";

                // Enable flip controls by default (as per your QtWidgets code)
                flipCoronalHorizontal = true;
                flipCoronalVertical = true;
                flipSagittalVertical = true;

                refreshImages();
            }
        }

        onCrosshairPositionChanged: {
            refreshImages();
        }

        onSliceUpdated: {
            refreshImages();
        }
    }

    // Helper functions
    function refreshImages() {
        if (!dicomHandler.volumeLoaded) return;

        var timestamp = Date.now();
        axialImage.source = "";
        coronalImage.source = "";
        sagittalImage.source = "";

        Qt.callLater(function() {
            axialImage.source = "image://dicom/axial_" + timestamp;
            coronalImage.source = "image://dicom/coronal_" + timestamp;
            sagittalImage.source = "image://dicom/sagittal_" + timestamp;
        });
    }

    function updateThresholdDisplay() {
        var lower = levelSlider.value - windowSlider.value / 2;
        var upper = levelSlider.value + windowSlider.value / 2;
        currentThresholds.text = "Lower: " + Math.round(lower) + "\nUpper: " + Math.round(upper);
    }

    // Initialize
    Component.onCompleted: {
        updateThresholdDisplay();
    }

}
