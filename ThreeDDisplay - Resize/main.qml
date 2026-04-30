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
    property bool showCrosshair: true
    property color crosshairColor: "red"

    // Orientation properties
    property bool flipCoronalHorizontal: true
    property bool flipCoronalVertical: true
    property bool flipSagittalVertical: true

    // Layout state management
    property string expandedView: ""
    property real expandedWidthPercent: 0.7

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
            }

            RowLayout {
                Layout.fillWidth: true
                spacing: 15

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
                            refreshImages();
                        }
                    }
                }

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
                            refreshImages();
                        }
                    }
                }

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

            RowLayout {
                Layout.fillWidth: true

                Button {
                    text: "Reset Layout"
                    visible: expandedView !== ""
                    onClicked: {
                        expandedView = "";
                    }
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

    // Main content area
    Item {
        id: mainContent
        anchors.top: header.bottom
        anchors.bottom: footer.top
        anchors.left: parent.left
        anchors.right: parent.right
        anchors.margins: 10

        // Normal layout (2x2 grid)
        GridLayout {
            id: normalLayout
            anchors.fill: parent
            columns: 2
            rows: 2
            columnSpacing: 10
            rowSpacing: 10
            visible: expandedView === ""

            // 3D View
            Rectangle {
                Layout.fillWidth: true
                Layout.fillHeight: true
                color: "transparent"

                ColumnLayout {
                    anchors.fill: parent
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
                        Layout.fillWidth: true
                        Layout.fillHeight: true
                        renderWindow: vtkWindow
                        Component.onCompleted: {
                            VolumeRenderer.setRenderer(vtkWindow);
                        }
                    }
                }

                MouseArea {
                    anchors.fill: parent
                    onDoubleClicked: {
                        expandedView = "3d";
                    }
                }
            }

            // Axial View
            Rectangle {
                id: axialContainer
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

                    Rectangle {
                        Layout.fillWidth: true
                        Layout.fillHeight: true
                        color: "black"

                        Image {
                            id: axialImage
                            anchors.fill: parent
                            source: dicomHandler.volumeLoaded ? "image://dicom/axial" : ""
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

                    ColumnLayout {
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
                                axialImage.source = "image://dicom/axial?_t=" + Date.now();
                            }
                        }
                    }
                }

                MouseArea {
                    anchors.fill: parent
                    onDoubleClicked: {
                        expandedView = "axial";
                    }
                }
            }

            // Coronal View
            Rectangle {
                id: coronalContainer
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

                    Rectangle {
                        Layout.fillWidth: true
                        Layout.fillHeight: true
                        color: "black"

                        Item {
                            anchors.fill: parent

                            Image {
                                id: coronalImage
                                anchors.fill: parent
                                source: dicomHandler.volumeLoaded ? "image://dicom/coronal" : ""
                                fillMode: Image.PreserveAspectFit
                                cache: false

                                transform: [
                                    Scale {
                                        xScale: flipCoronalHorizontal ? -1 : 1
                                        yScale: flipCoronalVertical ? -1 : 1
                                        origin.x: coronalImage.width / 2
                                        origin.y: coronalImage.height / 2
                                    }
                                ]
                            }

                            MouseArea {
                                anchors.fill: parent
                                onClicked: {
                                    if (coronalImage.sourceSize.width > 0) {
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
                                coronalImage.source = "image://dicom/coronal?_t=" + Date.now();
                            }
                        }
                    }
                }

                MouseArea {
                    anchors.fill: parent
                    onDoubleClicked: {
                        expandedView = "coronal";
                    }
                }
            }

            // Sagittal View
            Rectangle {
                id: sagittalContainer
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

                    Rectangle {
                        Layout.fillWidth: true
                        Layout.fillHeight: true
                        color: "black"

                        Item {
                            anchors.fill: parent

                            Image {
                                id: sagittalImage
                                anchors.fill: parent
                                source: dicomHandler.volumeLoaded ? "image://dicom/sagittal" : ""
                                fillMode: Image.PreserveAspectFit
                                cache: false

                                transform: [
                                    Scale {
                                        xScale: 1
                                        yScale: flipSagittalVertical ? -1 : 1
                                        origin.x: sagittalImage.width / 2
                                        origin.y: sagittalImage.height / 2
                                    }
                                ]
                            }

                            MouseArea {
                                anchors.fill: parent
                                onClicked: {
                                    if (sagittalImage.sourceSize.width > 0) {
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
                                sagittalImage.source = "image://dicom/sagittal?_t=" + Date.now();
                            }
                        }
                    }
                }

                MouseArea {
                    anchors.fill: parent
                    onDoubleClicked: {
                        expandedView = "sagittal";
                    }
                }
            }
        }

        // Split layout for expanded view
        RowLayout {
            id: splitLayout
            anchors.fill: parent
            spacing: 10
            visible: expandedView !== ""

            // Expanded view
            Item {
                Layout.fillWidth: true
                Layout.fillHeight: true
                Layout.preferredWidth: parent.width * expandedWidthPercent

                Loader {
                    id: expandedLoader
                    anchors.fill: parent
                    sourceComponent: {
                        switch(expandedView) {
                            case "3d": return expanded3DComponent;
                            case "axial": return expandedAxialComponent;
                            case "coronal": return expandedCoronalComponent;
                            case "sagittal": return expandedSagittalComponent;
                            default: return null;
                        }
                    }
                }
            }

            // Small views stack
            Item {
                Layout.fillWidth: true
                Layout.fillHeight: true

                ColumnLayout {
                    anchors.fill: parent
                    spacing: 10

                    Loader {
                        id: smallView1
                        Layout.fillWidth: true
                        Layout.fillHeight: true
                        sourceComponent: getSmallComponent(0)
                    }

                    Loader {
                        id: smallView2
                        Layout.fillWidth: true
                        Layout.fillHeight: true
                        sourceComponent: getSmallComponent(1)
                    }

                    Loader {
                        id: smallView3
                        Layout.fillWidth: true
                        Layout.fillHeight: true
                        sourceComponent: getSmallComponent(2)
                    }
                }
            }
        }
    }

    function getSmallComponent(index) {
        var views = [];
        if (expandedView !== "3d") views.push(small3DComponent);
        if (expandedView !== "axial") views.push(smallAxialComponent);
        if (expandedView !== "coronal") views.push(smallCoronalComponent);
        if (expandedView !== "sagittal") views.push(smallSagittalComponent);

        if (index < views.length) {
            return views[index];
        }
        return null;
    }

    // Expanded view components
    Component {
        id: expanded3DComponent
        Rectangle {
            color: "transparent"
            ColumnLayout {
                anchors.fill: parent
                spacing: 5
                Label { text: "3D Render (Expanded)"; color: "White"; font.bold: true; font.pixelSize: 14; Layout.alignment: Qt.AlignHCenter }
                VTKRenderItem { Layout.fillWidth: true; Layout.fillHeight: true; renderWindow: vtkWindow }
            }
            MouseArea { anchors.fill: parent; onDoubleClicked: expandedView = "" }
        }
    }

    Component {
        id: expandedAxialComponent
        Rectangle {
            color: "#252526"
            ColumnLayout {
                anchors.fill: parent; anchors.margins: 5
                Label { text: "AXIAL (Expanded)"; color: "#4CAF50"; font.bold: true; Layout.alignment: Qt.AlignHCenter }
                Rectangle {
                    Layout.fillWidth: true; Layout.fillHeight: true; color: "black"
                    Image {
                        anchors.fill: parent
                        source: dicomHandler.volumeLoaded ? "image://dicom/axial" : ""
                        fillMode: Image.PreserveAspectFit
                        cache: false
                    }
                }
                Slider {
                    id: expandedAxialSlider
                    Layout.fillWidth: true
                    from: 0; to: Math.max(dicomHandler.getMaxSliceCount(0) - 1, 1)
                    value: dicomHandler.crosshairPosition.z
                    onMoved: { var pos = dicomHandler.crosshairPosition; pos.z = value; dicomHandler.crosshairPosition = pos; }
                }
            }
            MouseArea { anchors.fill: parent; onDoubleClicked: expandedView = "" }
        }
    }

    Component {
        id: expandedCoronalComponent
        Rectangle {
            color: "#252526"
            ColumnLayout {
                anchors.fill: parent; anchors.margins: 5
                Label { text: "CORONAL (Expanded)"; color: "#2196F3"; font.bold: true; Layout.alignment: Qt.AlignHCenter }
                Rectangle {
                    Layout.fillWidth: true; Layout.fillHeight: true; color: "black"
                    Image {
                        anchors.fill: parent
                        source: dicomHandler.volumeLoaded ? "image://dicom/coronal" : ""
                        fillMode: Image.PreserveAspectFit
                        cache: false
                    }
                }
                Slider {
                    id: expandedCoronalSlider
                    Layout.fillWidth: true
                    from: 0; to: Math.max(dicomHandler.getMaxSliceCount(1) - 1, 1)
                    value: dicomHandler.crosshairPosition.y
                    onMoved: { var pos = dicomHandler.crosshairPosition; pos.y = value; dicomHandler.crosshairPosition = pos; }
                }
            }
            MouseArea { anchors.fill: parent; onDoubleClicked: expandedView = "" }
        }
    }

    Component {
        id: expandedSagittalComponent
        Rectangle {
            color: "#252526"
            ColumnLayout {
                anchors.fill: parent; anchors.margins: 5
                Label { text: "SAGITTAL (Expanded)"; color: "#FF9800"; font.bold: true; Layout.alignment: Qt.AlignHCenter }
                Rectangle {
                    Layout.fillWidth: true; Layout.fillHeight: true; color: "black"
                    Image {
                        anchors.fill: parent
                        source: dicomHandler.volumeLoaded ? "image://dicom/sagittal" : ""
                        fillMode: Image.PreserveAspectFit
                        cache: false
                    }
                }
                Slider {
                    id: expandedSagittalSlider
                    Layout.fillWidth: true
                    from: 0; to: Math.max(dicomHandler.getMaxSliceCount(2) - 1, 1)
                    value: dicomHandler.crosshairPosition.x
                    onMoved: { var pos = dicomHandler.crosshairPosition; pos.x = value; dicomHandler.crosshairPosition = pos; }
                }
            }
            MouseArea { anchors.fill: parent; onDoubleClicked: expandedView = "" }
        }
    }

    // Small view components
    Component {
        id: small3DComponent
        Rectangle {
            color: "transparent"
            ColumnLayout {
                anchors.fill: parent; spacing: 2
                Label { text: "3D"; color: "White"; font.pixelSize: 10; Layout.alignment: Qt.AlignHCenter }
                VTKRenderItem { Layout.fillWidth: true; Layout.fillHeight: true; renderWindow: vtkWindow }
            }
            MouseArea { anchors.fill: parent; onDoubleClicked: expandedView = "" }
        }
    }

    Component {
        id: smallAxialComponent
        Rectangle {
            color: "#252526"
            ColumnLayout {
                anchors.fill: parent; anchors.margins: 2; spacing: 2
                Label { text: "AXIAL"; color: "#4CAF50"; font.pixelSize: 10; Layout.alignment: Qt.AlignHCenter }
                Rectangle {
                    Layout.fillWidth: true; Layout.fillHeight: true; color: "black"
                    Image {
                        anchors.fill: parent
                        source: dicomHandler.volumeLoaded ? "image://dicom/axial" : ""
                        fillMode: Image.PreserveAspectFit
                        cache: false
                    }
                }
            }
            MouseArea { anchors.fill: parent; onDoubleClicked: expandedView = "" }
        }
    }

    Component {
        id: smallCoronalComponent
        Rectangle {
            color: "#252526"
            ColumnLayout {
                anchors.fill: parent; anchors.margins: 2; spacing: 2
                Label { text: "CORONAL"; color: "#2196F3"; font.pixelSize: 10; Layout.alignment: Qt.AlignHCenter }
                Rectangle {
                    Layout.fillWidth: true; Layout.fillHeight: true; color: "black"
                    Image {
                        anchors.fill: parent
                        source: dicomHandler.volumeLoaded ? "image://dicom/coronal" : ""
                        fillMode: Image.PreserveAspectFit
                        cache: false
                    }
                }
            }
            MouseArea { anchors.fill: parent; onDoubleClicked: expandedView = "" }
        }
    }

    Component {
        id: smallSagittalComponent
        Rectangle {
            color: "#252526"
            ColumnLayout {
                anchors.fill: parent; anchors.margins: 2; spacing: 2
                Label { text: "SAGITTAL"; color: "#FF9800"; font.pixelSize: 10; Layout.alignment: Qt.AlignHCenter }
                Rectangle {
                    Layout.fillWidth: true; Layout.fillHeight: true; color: "black"
                    Image {
                        anchors.fill: parent
                        source: dicomHandler.volumeLoaded ? "image://dicom/sagittal" : ""
                        fillMode: Image.PreserveAspectFit
                        cache: false
                    }
                }
            }
            MouseArea { anchors.fill: parent; onDoubleClicked: expandedView = "" }
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

            Column {
                Label { text: "Position (mm):"; color: "#888"; font.pixelSize: 11 }
                Label {
                    text: {
                        if (!dicomHandler.volumeLoaded) return "N/A";
                        var pos = dicomHandler.crosshairPosition;
                        return pos.x.toFixed(1) + ", " + pos.y.toFixed(1) + ", " + pos.z.toFixed(1);
                    }
                    color: "white"; font.pixelSize: 12; font.bold: true
                }
            }

            Column {
                Label { text: "Voxel (X,Y,Z):"; color: "#888"; font.pixelSize: 11 }
                Label {
                    text: {
                        if (!dicomHandler.volumeLoaded) return "N/A";
                        var pos = dicomHandler.crosshairPosition;
                        return Math.round(pos.x) + ", " + Math.round(pos.y) + ", " + Math.round(pos.z);
                    }
                    color: "white"; font.pixelSize: 12; font.bold: true
                }
            }

            Column {
                Label { text: "Dimensions:"; color: "#888"; font.pixelSize: 11 }
                Label {
                    text: {
                        if (!dicomHandler.volumeLoaded) return "N/A";
                        var dims = dicomHandler.getVolumeDimensions();
                        return dims.x + " × " + dims.y + " × " + dims.z;
                    }
                    color: "white"; font.pixelSize: 12; font.bold: true
                }
            }

            Column {
                Label { text: "Spacing (mm):"; color: "#888"; font.pixelSize: 11 }
                Label {
                    text: {
                        if (!dicomHandler.volumeLoaded) return "N/A";
                        var spacing = dicomHandler.getVoxelSpacing();
                        return spacing.x.toFixed(2) + " × " + spacing.y.toFixed(2) + " × " + spacing.z.toFixed(2);
                    }
                    color: "white"; font.pixelSize: 12; font.bold: true
                }
            }
        }
    }

    Connections {
        target: dicomHandler
        onVolumeLoadedChanged: {
            if (dicomHandler.volumeLoaded) {
                axialSlider.to = Math.max(dicomHandler.getMaxSliceCount(0) - 1, 1);
                coronalSlider.to = Math.max(dicomHandler.getMaxSliceCount(1) - 1, 1);
                sagittalSlider.to = Math.max(dicomHandler.getMaxSliceCount(2) - 1, 1);

                statusText.text = "DICOM Loaded Successfully";
                statusText.color = "#4CAF50";

                flipCoronalHorizontal = true;
                flipCoronalVertical = true;
                flipSagittalVertical = true;

                refreshImages();
            }
        }

        onCrosshairPositionChanged: {
            refreshImages();
        }
    }

    function refreshImages() {
        if (!dicomHandler.volumeLoaded) return;

        var timestamp = Date.now();
        axialImage.source = "image://dicom/axial?_t=" + timestamp;
        coronalImage.source = "image://dicom/coronal?_t=" + timestamp;
        sagittalImage.source = "image://dicom/sagittal?_t=" + timestamp;
    }

    function updateThresholdDisplay() {
        var lower = levelSlider.value - windowSlider.value / 2;
        var upper = levelSlider.value + windowSlider.value / 2;
        currentThresholds.text = "Lower: " + Math.round(lower) + "\nUpper: " + Math.round(upper);
    }

    Component.onCompleted: {
        updateThresholdDisplay();
    }
}
