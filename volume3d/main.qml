import QtQuick 2.14
import QtQuick.Controls 2.14
import QtQuick.Layouts 1.14

ApplicationWindow {
    id: mainWindow
    visible: true
    width: 1200
    height: 800
    title: "DICOM Viewer v1.0"
    color: "#1e1e2e"

    // Status properties
    property bool isLoading: false
    property string statusMessage: "Ready"
    property int currentSlice: dicomHandler ? dicomHandler.getCurrentSlice() : 0
    property int totalSlices: dicomHandler ? dicomHandler.getTotalSlices() : 0

    // Main layout
    ColumnLayout {
        anchors.fill: parent
        anchors.margins: 10
        spacing: 10

        // Top control panel
        Rectangle {
            id: controlPanel
            Layout.fillWidth: true
            height: 80
            color: "#2d2d3e"
            radius: 5

            RowLayout {
                anchors.fill: parent
                anchors.margins: 10
                spacing: 10

                // Load button
                Button {
                    id: loadButton
                    text: "📁 Load DICOM Series"
                    font.pixelSize: 14
                    font.bold: true
                    padding: 12

                    background: Rectangle {
                        radius: 4
                        color: loadButton.down ? "#4a9eff" : (loadButton.hovered ? "#5aaeff" : "#3d8ee8")
                    }

                    contentItem: Text {
                        text: loadButton.text
                        font: loadButton.font
                        color: "white"
                        horizontalAlignment: Text.AlignHCenter
                        verticalAlignment: Text.AlignVCenter
                    }

                    onClicked: {
                        statusMessage = "Loading DICOM series...";
                        isLoading = true;
                        dicomHandler.loadDicomSeries();
                    }
                }

                // Navigation buttons
                Button {
                    text: "⏮ First"
                    onClicked: {
                        if (dicomHandler && totalSlices > 0) {
                            dicomHandler.showSlice(0);
                        }
                    }
                    enabled: totalSlices > 0
                }

                Button {
                    text: "◀ Prev"
                    onClicked: {
                        if (dicomHandler) {
                            dicomHandler.prevSlice();
                        }
                    }
                    enabled: currentSlice > 0
                }

                Button {
                    text: "Next ▶"
                    onClicked: {
                        if (dicomHandler) {
                            dicomHandler.nextSlice();
                        }
                    }
                    enabled: currentSlice < totalSlices - 1
                }

                Button {
                    text: "Last ⏭"
                    onClicked: {
                        if (dicomHandler && totalSlices > 0) {
                            dicomHandler.showSlice(totalSlices - 1);
                        }
                    }
                    enabled: totalSlices > 0
                }

                // Slice info
                Rectangle {
                    Layout.fillWidth: true
                    height: 40
                    color: "transparent"

                    Column {
                        anchors.centerIn: parent
                        spacing: 2

                        Text {
                            text: "Slice: " + (currentSlice + 1) + " / " + totalSlices
                            color: "white"
                            font.pixelSize: 14
                        }

                        Text {
                            text: "Drag slider or use arrow keys"
                            color: "#888888"
                            font.pixelSize: 10
                        }
                    }
                }

                // Window/Level controls
                Button {
                    text: "⚙️ Settings"
                    onClicked: settingsPopup.open()
                }
            }
        }

        // Main content area
        Rectangle {
            Layout.fillWidth: true
            Layout.fillHeight: true
            color: "#0f0f1e"
            radius: 5

            // DICOM Image display
            Image {
                id: dicomImage
                anchors.fill: parent
                anchors.margins: 10
                fillMode: Image.PreserveAspectFit
                source: "image://dicom/slice"
                asynchronous: true
                cache: false

                // Status handling
                onStatusChanged: {
                    if (status === Image.Ready) {
                        console.log("Image loaded successfully");
                    } else if (status === Image.Loading) {
                        console.log("Image loading...");
                    } else if (status === Image.Error) {
                        console.log("Image error:", errorString);
                        // Force reload
                        var oldSource = source;
                        source = "";
                        source = oldSource;
                    }
                }

                // Mouse wheel for slice navigation
                MouseArea {
                    anchors.fill: parent
                    acceptedButtons: Qt.NoButton
                    onWheel: {
                        if (wheel.angleDelta.y > 0) {
                            // Scroll up - previous slice
                            if (currentSlice > 0) {
                                dicomHandler.prevSlice();
                            }
                        } else {
                            // Scroll down - next slice
                            if (currentSlice < totalSlices - 1) {
                                dicomHandler.nextSlice();
                            }
                        }
                    }
                }

                // Placeholder when no image
                Rectangle {
                    anchors.fill: parent
                    color: "#1a1a2e"
                    visible: dicomImage.status !== Image.Ready && !isLoading

                    Column {
                        anchors.centerIn: parent
                        spacing: 20

                        Text {
                            text: "No DICOM Image Loaded"
                            color: "#888888"
                            font.pixelSize: 24
                            font.bold: true
                        }

                        Text {
                            text: "Click 'Load DICOM Series' to begin"
                            color: "#666666"
                            font.pixelSize: 16
                        }

                        Button {
                            anchors.horizontalCenter: parent.horizontalCenter
                            text: "📁 Load DICOM Series"
                            onClicked: loadButton.clicked()
                        }
                    }
                }

                // Loading overlay
                Rectangle {
                    anchors.fill: parent
                    color: "#80000000"
                    visible: isLoading

                    Column {
                        anchors.centerIn: parent
                        spacing: 20

                        BusyIndicator {
                            anchors.horizontalCenter: parent.horizontalCenter
                            running: true
                            width: 60
                            height: 60
                        }

                        Text {
                            text: statusMessage
                            color: "white"
                            font.pixelSize: 16
                        }
                    }
                }
            }
        }

        // Bottom panel with slider and info
        Rectangle {
            Layout.fillWidth: true
            height: 120
            color: "#2d2d3e"
            radius: 5

            ColumnLayout {
                anchors.fill: parent
                anchors.margins: 10
                spacing: 5

                // Slice slider
                RowLayout {
                    Layout.fillWidth: true
                    spacing: 10

                    Text {
                        text: "Slice:"
                        color: "white"
                        font.pixelSize: 12
                    }

                    Slider {
                        id: sliceSlider
                        Layout.fillWidth: true
                        from: 0
                        to: Math.max(0, totalSlices - 1)
                        value: currentSlice
                        stepSize: 1
                        enabled: totalSlices > 0

                        onValueChanged: {
                            if (dicomHandler && Math.round(value) !== currentSlice) {
                                dicomHandler.showSlice(Math.round(value));
                            }
                        }

                        background: Rectangle {
                            x: sliceSlider.leftPadding
                            y: sliceSlider.topPadding + sliceSlider.availableHeight / 2 - height / 2
                            implicitWidth: 200
                            implicitHeight: 4
                            width: sliceSlider.availableWidth
                            height: implicitHeight
                            radius: 2
                            color: "#4a4a6a"

                            Rectangle {
                                width: sliceSlider.visualPosition * parent.width
                                height: parent.height
                                color: "#3d8ee8"
                                radius: 2
                            }
                        }

                        handle: Rectangle {
                            x: sliceSlider.leftPadding + sliceSlider.visualPosition * (sliceSlider.availableWidth - width)
                            y: sliceSlider.topPadding + sliceSlider.availableHeight / 2 - height / 2
                            implicitWidth: 20
                            implicitHeight: 20
                            radius: 10
                            color: sliceSlider.pressed ? "#ffffff" : "#f0f0f0"
                            border.color: "#3d8ee8"
                            border.width: 2
                        }
                    }

                    Text {
                        text: (currentSlice + 1) + "/" + totalSlices
                        color: "white"
                        font.pixelSize: 12
                        width: 60
                    }
                }

                // Volume information
                GridLayout {
                    Layout.fillWidth: true
                    columns: 3
                    columnSpacing: 20
                    rowSpacing: 5

                    Text {
                        text: "Dimensions:"
                        color: "#888888"
                        font.pixelSize: 12
                    }

                    Text {
                        text: dicomHandler && dicomHandler.volumeLoaded ?
                              dicomHandler.volumeDimensions.x + " × " +
                              dicomHandler.volumeDimensions.y + " × " +
                              dicomHandler.volumeDimensions.z : "N/A"
                        color: "white"
                        font.pixelSize: 12
                    }

                    Text {
                        text: "Spacing:"
                        color: "#888888"
                        font.pixelSize: 12
                    }

                    Text {
                        text: dicomHandler && dicomHandler.volumeLoaded ?
                              dicomHandler.volumeSpacing.x.toFixed(2) + " × " +
                              dicomHandler.volumeSpacing.y.toFixed(2) + " × " +
                              dicomHandler.volumeSpacing.z.toFixed(2) + " mm" : "N/A"
                        color: "white"
                        font.pixelSize: 12
                    }

                    Text {
                        text: "Status:"
                        color: "#888888"
                        font.pixelSize: 12
                    }

                    Text {
                        text: dicomHandler && dicomHandler.volumeLoaded ? "Loaded ✓" : "Not Loaded"
                        color: dicomHandler && dicomHandler.volumeLoaded ? "#4CAF50" : "#F44336"
                        font.pixelSize: 12
                    }
                }

                // Status message
                Text {
                    Layout.fillWidth: true
                    text: statusMessage
                    color: "#888888"
                    font.pixelSize: 11
                    elide: Text.ElideRight
                }
            }
        }
    }

    // Settings popup
    Popup {
        id: settingsPopup
        width: 400
        height: 300
        x: (parent.width - width) / 2
        y: (parent.height - height) / 2
        modal: true
        focus: true

        background: Rectangle {
            color: "#2d2d3e"
            radius: 5
            border.color: "#4a4a6a"
            border.width: 1
        }

        ColumnLayout {
            anchors.fill: parent
            anchors.margins: 20
            spacing: 15

            Text {
                text: "Display Settings"
                color: "white"
                font.pixelSize: 18
                font.bold: true
                Layout.alignment: Qt.AlignHCenter
            }

            // Window/Level controls
            ColumnLayout {
                spacing: 10

                Text {
                    text: "Window/Level"
                    color: "white"
                    font.pixelSize: 14
                }

                RowLayout {
                    Text {
                        text: "Window:"
                        color: "#888888"
                    }

                    Slider {
                        id: windowSlider
                        Layout.fillWidth: true
                        from: 100
                        to: 4000
                        value: 2000
                    }

                    Text {
                        text: Math.round(windowSlider.value)
                        color: "white"
                        width: 50
                    }
                }

                RowLayout {
                    Text {
                        text: "Level:"
                        color: "#888888"
                    }

                    Slider {
                        id: levelSlider
                        Layout.fillWidth: true
                        from: -1000
                        to: 3000
                        value: 500
                    }

                    Text {
                        text: Math.round(levelSlider.value)
                        color: "white"
                        width: 50
                    }
                }

                Button {
                    text: "Apply Window/Level"
                    Layout.alignment: Qt.AlignHCenter
                    onClicked: {
                        dicomHandler.setWindowLevel(windowSlider.value, levelSlider.value);
                    }
                }
            }

            Item { Layout.fillHeight: true }

            Button {
                text: "Close"
                Layout.alignment: Qt.AlignHCenter
                onClicked: settingsPopup.close()
            }
        }
    }

    // Keyboard shortcuts
    Shortcut {
        sequence: "Left"
        onActivated: {
            if (dicomHandler && currentSlice > 0) {
                dicomHandler.prevSlice();
            }
        }
    }

    Shortcut {
        sequence: "Right"
        onActivated: {
            if (dicomHandler && currentSlice < totalSlices - 1) {
                dicomHandler.nextSlice();
            }
        }
    }

    Shortcut {
        sequence: "Home"
        onActivated: {
            if (dicomHandler && totalSlices > 0) {
                dicomHandler.showSlice(0);
            }
        }
    }

    Shortcut {
        sequence: "End"
        onActivated: {
            if (dicomHandler && totalSlices > 0) {
                dicomHandler.showSlice(totalSlices - 1);
            }
        }
    }

    Shortcut {
        sequence: "Ctrl+L"
        onActivated: loadButton.clicked()
    }

    // Connections to C++ signals
    Connections {
        target: dicomHandler

        onVolumeLoadedChanged: {
            if (dicomHandler.volumeLoaded) {
                statusMessage = "Volume loaded successfully";
                totalSlices = dicomHandler.getTotalSlices();
                sliceSlider.to = Math.max(0, totalSlices - 1);
                sliceSlider.value = Math.floor(totalSlices / 2);
                console.log("Volume loaded, slices:", totalSlices);
            } else {
                statusMessage = "Volume not loaded";
            }
            isLoading = false;
        }

        onSliceChanged: {
            currentSlice = sliceIndex;
            sliceSlider.value = sliceIndex;
            statusMessage = "Showing slice: " + (sliceIndex + 1) + "/" + totalSlices;
        }

        onErrorOccurred: {
            statusMessage = "Error: " + message;
            isLoading = false;
            console.error("DICOM Error:", message);

            // Show error message
            errorPopup.errorMessage = message;
            errorPopup.open();
        }

        onLoadingStarted: {
            isLoading = true;
            statusMessage = "Loading DICOM series...";
        }

        onLoadingFinished: {
            if (!dicomHandler.volumeLoaded) {
                statusMessage = "Loading finished (no volume loaded)";
                isLoading = false;
            }
        }
    }

    // Error popup
    Popup {
        id: errorPopup
        property string errorMessage: ""

        width: 500
        height: 200
        x: (parent.width - width) / 2
        y: (parent.height - height) / 2
        modal: true

        background: Rectangle {
            color: "#2d1e1e"
            radius: 5
            border.color: "#ff6b6b"
            border.width: 2
        }

        ColumnLayout {
            anchors.fill: parent
            anchors.margins: 20
            spacing: 15

            Text {
                text: "⚠️ Error"
                color: "#ff6b6b"
                font.pixelSize: 20
                font.bold: true
            }

            ScrollView {
                Layout.fillWidth: true
                Layout.fillHeight: true

                TextArea {
                    id: errorText
                    text: errorPopup.errorMessage
                    color: "#ffcccc"
                    font.pixelSize: 12
                    wrapMode: Text.Wrap
                    readOnly: true
                    background: Rectangle {
                        color: "transparent"
                    }
                }
            }

            Button {
                text: "OK"
                Layout.alignment: Qt.AlignRight
                onClicked: errorPopup.close()

                background: Rectangle {
                    radius: 3
                    color: parent.down ? "#ff6b6b" : "#ff8a8a"
                }

                contentItem: Text {
                    text: parent.text
                    color: "white"
                    horizontalAlignment: Text.AlignHCenter
                    verticalAlignment: Text.AlignVCenter
                }
            }
        }
    }

    // Initialization
    Component.onCompleted: {
        console.log("DICOM Viewer started");
        console.log("OpenGL backend:", Qt.platform.openGLModuleType);

        // Set initial window size
        mainWindow.width = 1200;
        mainWindow.height = 800;
    }
}
