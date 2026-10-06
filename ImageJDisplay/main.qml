import QtQuick 2.14
import QtQuick.Controls 2.14
import QtQuick.Layouts 1.14
import QtQuick.Dialogs 1.3
ApplicationWindow {
    id: root

    visible: true

    width: 1600
    height: 950

    minimumWidth: 1200
    minimumHeight: 750

    title: "DICOM Viewer - Medical Image Enhancer"

    color: "#0B1320"

    // =========================================================
    // APPLICATION STATE
    // =========================================================

    property bool imageLoaded: false

//    property string patientName: "John Doe"
//    property string patientId: "PID123456"
//    property string patientDob: "1980-05-15"
//    property string patientGender: "M"

//    property string modality: "CT"
//    property string studyDate: "2024-05-20"

//    property string studyName: "CT Brain WO Contrast"
//    property string seriesName: "Brain Axial"

    property int currentImage: 45
    property int totalImages: 120

    property real zoomValue: 1.0

    property real brightness: 0
    property real contrast: 1.0

    property int minimumValue: 0
    property int maximumValue: 65535

    //histogram

    property real hoveredValue: 0
    property var hoveredCount: 0
    property bool histogramMouseInside: false

    function resetHistogramHover() {
        hoveredValue = 0
        hoveredCount = 0
        histogramMouseInside = false
    }

    FileDialog {
        id: saveDicomDialog

        title: "Save Processed DICOM"

        selectExisting: false

        selectMultiple: false

        nameFilters: [
            "DICOM files (*.dcm)"
        ]

        folder: shortcuts.documents

        onAccepted: {

            var path = fileUrl.toString()

            // Convert file:/// URL to local path
            path = path.replace("file:///", "")

            console.log(
                        "SAVE PATH:",
                        path)

            var result =
                    dicomLoader.saveProcessedDicom(
                        path)

            console.log(
                        "SAVE RESULT:",
                        result)
        }
    }


    // =====================================================
    // HISTOGRAM POPUP
    // =====================================================

    Popup {
        id: histogramPopup

        width: 345
        height: 370

        modal: false
        focus: true

        padding: 0

        // Floating position
        x: parent.width - width - 20
        y: 80

        closePolicy: Popup.CloseOnEscape
                     | Popup.CloseOnPressOutside


        background: Rectangle {

            color: "#1E293B"

            border.color: "#34475B"
            border.width: 1

            radius: 6
        }


        Column {
            anchors.fill: parent

            spacing: 0


            // ==========================================
            // FLOATING TITLE BAR
            // ==========================================

            Rectangle {
                id: titleBar

                width: parent.width
                height: 34

                color: "#182434"

                radius: 6


                Text {

                    anchors.left: parent.left
                    anchors.leftMargin: 12

                    anchors.verticalCenter: parent.verticalCenter

                    text: "Histogram"

                    color: "#FFFFFF"

                    font.pixelSize: 13
                }


                ToolButton {
                    id: closeButton

                    width: 34
                    height: 34

                    anchors.right: parent.right
                    anchors.rightMargin: 4

                    text: "×"


                    contentItem: Text {

                        text: parent.text

                        color: "#C9D4DF"

                        font.pixelSize: 20

                        horizontalAlignment: Text.AlignHCenter
                        verticalAlignment: Text.AlignVCenter
                    }


                    background: Rectangle {

                        radius: 4

                        color: parent.hovered
                               ? "#253850"
                               : "transparent"
                    }


                    onClicked:{

                        histogramPopup.close()
                    }
                }


                // ======================================
                // DRAG FLOATING POPUP
                // ======================================

                MouseArea {
                    id: dragArea

                    anchors.left: parent.left
                    anchors.right: closeButton.left

                    anchors.top: parent.top
                    anchors.bottom: parent.bottom

                    property real startX: 0
                    property real startY: 0


                    onPressed: {

                        startX = mouse.x
                        startY = mouse.y
                    }


                    onPositionChanged: {

                        if (pressed) {

                            histogramPopup.x +=
                                    mouse.x - startX

                            histogramPopup.y +=
                                    mouse.y - startY
                        }
                    }
                }
            }


            // ==========================================
            // IMAGE INFORMATION
            // ==========================================

            Text {

                width: parent.width

                height: 22

                leftPadding: 10

                text: "3020 × 3020 pixels"

                color: "#9CA3AF"

                font.pixelSize: 11

                verticalAlignment: Text.AlignVCenter
            }


            // ==========================================
            // MAIN HISTOGRAM AREA
            // ==========================================

            Rectangle {

                width: parent.width - 12
                height: 260

                anchors.horizontalCenter: parent.horizontalCenter

                color: "#111827"

                border.color: "#34475B"
                border.width: 1


                Column {

                    anchors.fill: parent
                    anchors.margins: 7

                    spacing: 0


                    // ==================================
                    // HISTOGRAM GRAPH
                    // ==================================

                    Rectangle {

                        id: histogramGraph

                        width: parent.width
                        height: 150

                        color: "#0F172A"

                        border.color: "#34475B"
                        border.width: 1


                        Canvas {

                            id: histogramCanvas

                            anchors.fill: parent

                            anchors.margins: 3


                            onPaint: {

                                var ctx = getContext("2d")

                                ctx.reset()


                                var data =
                                        histogramGraph.histogramData


                                if (!data
                                        || data.length === 0)
                                    return


                                var maxValue = 1


                                for (var i = 0;
                                     i < data.length;
                                     ++i) {

                                    if (data[i] > maxValue)
                                        maxValue = data[i]
                                }


                                var barWidth =
                                        width / data.length


                                ctx.fillStyle = "#9CA3AF"


                                for (var x = 0;
                                     x < data.length;
                                     ++x) {

                                    var barHeight =
                                            (data[x] / maxValue)
                                            * height


                                    ctx.fillRect(
                                                x * barWidth,
                                                height - barHeight,
                                                Math.max(
                                                    1,
                                                    barWidth - 1),
                                                barHeight)
                                }
                            }
                        }


                        property var histogramData:
                            dicomLoader.histogramData
                        MouseArea {
                            id: histogramMouseArea

                            anchors.fill: histogramCanvas

                            hoverEnabled: true

                            onEntered: {
                                root.histogramMouseInside = true
                            }

                            onPositionChanged: {

                                root.histogramMouseInside = true

                                var data = histogramGraph.histogramData

                                if (!data || data.length === 0)
                                    return

                                var binIndex = Math.floor(
                                            mouse.x / width * data.length)

                                binIndex = Math.max(
                                            0,
                                            Math.min(
                                                data.length - 1,
                                                binIndex))

                                root.hoveredCount = data[binIndex]

                                var minValue = dicomLoader.histogramMin
                                var maxValue = dicomLoader.histogramMax

                                root.hoveredValue =
                                        minValue +
                                        (binIndex /
                                         Math.max(1, data.length - 1))
                                        * (maxValue - minValue)
                            }

                            onExited: {
                                console.log("MOUSE LEFT HISTOGRAM")

                                root.resetHistogramHover()
                            }
                        }

                    }


                    // ==================================
                    // MIN / MAX RANGE
                    // ==================================


                    Item {
                        width: histogramGraph.width
                        height: 22

                        Text {
                            id: minRangeLabel

                            anchors.left: parent.left
                            anchors.bottom: parent.bottom

                            text: "0"

                            color: "#9CA3AF"

                            font.pixelSize: 10
                        }

                        Text {
                            id: maxRangeLabel

                            anchors.right: parent.right
                            anchors.bottom: parent.bottom

                            text: "65535"

                            color: "#9CA3AF"

                            font.pixelSize: 10
                        }
                    }

                    // ==================================
                    // STATISTICS
                    // ==================================

                    Grid {

                        width: parent.width

                        columns: 2

                        columnSpacing: 25
                        rowSpacing: 5


                        Text {
                            text: "Count: " + dicomLoader.histogramCount
                            font.pixelSize: 11
                            color: "#C9D4DF"
                        }

                        Text {
                            text: "Min: " + dicomLoader.histogramMin
                            font.pixelSize: 11
                            color: "#C9D4DF"
                        }


                        Text {
                            text: "Mean: "
                                  + dicomLoader.histogramMean.toFixed(3)
                            font.pixelSize: 11
                            color: "#C9D4DF"
                        }

                        Text {
                            text: "Max: " + dicomLoader.histogramMax
                            font.pixelSize: 11
                            color: "#C9D4DF"
                        }


                        Text {
                            text: "StdDev: "
                                  + dicomLoader.histogramStdDev.toFixed(3)
                            font.pixelSize: 11
                            color: "#C9D4DF"
                        }

                        Text {
                            text: "Mode: "
                                  + dicomLoader.histogramMode
                                  + " ("
                                  + dicomLoader.histogramModeCount
                                  + ")"
                            font.pixelSize: 11
                            color: "#C9D4DF"
                        }


                        Text {
                            text: "Bins: "
                                  + dicomLoader.histogramBins
                            font.pixelSize: 11
                            color: "#C9D4DF"
                        }

                        Text {
                            text: "Bin Width: "
                                  + dicomLoader.histogramBinWidth.toFixed(3)
                            font.pixelSize: 11
                            color: "#C9D4DF"
                        }
                    }
                }
            }


            // ==========================================
            // BOTTOM CONTROLS
            // ==========================================

            Item {

                width: parent.width
                height: 65


                Row {

                    anchors.left: parent.left
                    anchors.leftMargin: 8

                    anchors.top: parent.top
                    anchors.topMargin: 8

                    spacing: 6


                    Repeater {

                        model: [
                            "List",
                            "Copy",
                            "Log",
                            "Live"
                        ]


                        delegate: Button {

                            text: modelData

                            width: 40
                            height: 26


                            contentItem: Text {

                                text: parent.text

                                color: "#C9D4DF"

                                font.pixelSize: 11

                                horizontalAlignment:
                                    Text.AlignHCenter

                                verticalAlignment:
                                    Text.AlignVCenter
                            }


                            background: Rectangle {

                                radius: 3

                                color: parent.pressed
                                       ? "#3B82F6"
                                       : parent.hovered
                                         ? "#253850"
                                         : "#1A2838"

                                border.color: "#34475B"

                                border.width: 1
                            }
                        }
                    }
                }


                Column {

                    anchors.right: parent.right
                    anchors.rightMargin: 10

                    anchors.top: parent.top
                    anchors.topMargin: 8

                    spacing: 5


                    Text {

                        text: root.histogramMouseInside
                              ? "value: " + root.hoveredValue.toFixed(3)
                              : "value: -"

                        color: "#9CA3AF"

                        font.pixelSize: 10
                    }


                    Text {

                        text: root.histogramMouseInside
                              ? "count: " + root.hoveredCount
                              : "count: -"

                        color: "#9CA3AF"

                        font.pixelSize: 10
                    }
                }
            }
        }


        onOpened: {

            histogramCanvas.requestPaint()

            console.log("Histogram floating popup opened")
        }
    }
    // =========================================================
    // MAIN LAYOUT
    // =========================================================

    ColumnLayout {

        anchors.fill: parent

        spacing: 0


        // =====================================================
        // HEADER
        // =====================================================

        Loader {
            id: headerLoader

            source: "headerFooter.qml"

            Layout.fillWidth: true
            Layout.preferredHeight: 100

            onLoaded: {

                console.log("Header loaded")

                if (!item)
                    return


                item.openRequested.connect(function() {

                    console.log("Open requested")
                })


                item.saveRequested.connect(function() {

                    console.log("SAVE SIGNAL RECEIVED IN MAIN")
                    saveDicomDialog.open()
                })


                item.zoomRequested.connect(function() {

                    root.zoomValue += 0.1

                    console.log(
                                "Zoom:",
                                root.zoomValue)
                })





                item.resetRequested.connect(function() {

                    root.zoomValue = 1.0

                    root.brightness = 0

                    root.contrast = 1.5

                    root.minimumValue = 0

                    root.maximumValue = 65535
                })
            }
        }


        Connections {
            target: headerLoader.item

            function onSaveRequested() {
                console.log("SAVE SIGNAL RECEIVED IN MAIN")

                saveDicomDialog.open()
            }

            function onHistogramRequested() {
                console.log("HISTOGRAM SIGNAL RECEIVED")

                // Always calculate fresh histogram
                dicomLoader.calculateHistogram()

                // Force Canvas redraw
                histogramCanvas.requestPaint()

                // Open popup
                histogramPopup.open()

                console.log("Histogram floating popup opened")
            }
        }
        Connections {
            target: dicomLoader

            function onHistogramChanged()
            {
                console.log("HISTOGRAM DATA UPDATED")

                histogramCanvas.requestPaint()
            }


            // =============================================
            // IMAGE PROCESSING VALUES CHANGED
            // =============================================

            function onImageProcessingChanged()
            {
                console.log(
                            "SYNC PROCESSING VALUES:",
                            "Min =", dicomLoader.minimum,
                            "Max =", dicomLoader.maximum,
                            "Brightness =", dicomLoader.brightness,
                            "Contrast =", dicomLoader.contrast)

                // Update ROOT values
                root.minimumValue =
                        dicomLoader.minimum

                root.maximumValue =
                        dicomLoader.maximum

                root.brightness =
                        dicomLoader.brightness

                root.contrast =
                        dicomLoader.contrast


                // Update Right Panel
                if (rightPanelLoader.item)
                {
                    rightPanelLoader.item.minimumValue =
                            dicomLoader.minimum

                    rightPanelLoader.item.maximumValue =
                            dicomLoader.maximum

                    rightPanelLoader.item.brightnessValue =
                            dicomLoader.brightness

                    rightPanelLoader.item.contrastValue =
                            dicomLoader.contrast
                }
            }
        }
        // =====================================================
        // CONTENT
        // =====================================================

        RowLayout {

            Layout.fillWidth: true
            Layout.fillHeight: true

            Layout.leftMargin: 10
            Layout.rightMargin: 10

            Layout.topMargin: 10
            Layout.bottomMargin: 10

            spacing: 10


            // =================================================
            // LEFT PANEL
            // =================================================

            Loader {
                id: leftPanelLoader

                source: "leftPanel.qml"

                Layout.preferredWidth: 270
                Layout.minimumWidth: 250

                Layout.fillHeight: true


                onLoaded: {

                    if (!item)
                        return


                    item.patientName =
                            root.patientName

                    item.patientId =
                            root.patientId

                    item.patientDob =
                            root.patientDob

                    item.patientGender =
                            root.patientGender

                    item.modality =
                            root.modality

                    item.studyDate =
                            root.studyDate
                }
            }


            // =================================================
            // IMAGE VIEWER
            // =================================================

            Loader {
                id: imageViewerLoader

                source: "imageViewer.qml"

                Layout.fillWidth: true

                Layout.fillHeight: true


                onLoaded: {

                    if (!item)
                        return


                    // =========================================
                    // IMAGE STATE
                    // =========================================

                    item.imageLoaded =
                            root.imageLoaded


                    // =========================================
                    // STUDY
                    // =========================================

                    item.studyName =
                            root.studyName

                    item.seriesName =
                            root.seriesName


                    // =========================================
                    // PATIENT
                    // =========================================

                    item.patientId =
                            root.patientId

                    item.patientName =
                            root.patientName

                    item.patientDob =
                            root.patientDob

                    item.patientGender =
                            root.patientGender


                    // =========================================
                    // IMAGE NAVIGATION
                    // =========================================

                    item.currentImage =
                            root.currentImage

                    item.totalImages =
                            root.totalImages


                    // =========================================
                    // ZOOM
                    // =========================================

                    item.zoomValue =
                            root.zoomValue


                    // =========================================
                    // SIGNALS
                    // =========================================

                    item.previousImageRequested.connect(
                                function() {

                        if (root.currentImage > 1) {

                            root.currentImage--
                        }

                        item.currentImage =
                                root.currentImage
                    })


                    item.nextImageRequested.connect(
                                function() {

                        if (root.currentImage <
                                root.totalImages) {

                            root.currentImage++
                        }

                        item.currentImage =
                                root.currentImage
                    })


                    item.imagePositionChanged.connect(
                                function(value) {

                        root.currentImage = value

                        item.currentImage = value
                    })
                }
            }


            // =================================================
            // RIGHT PANEL
            // =================================================

            Loader {
                id: rightPanelLoader

                source: "rightPanel.qml"

                Layout.preferredWidth: 260
                Layout.minimumWidth: 240

                Layout.fillHeight: true


                onLoaded: {



                    if (!item)
                        return


                    // =========================================
                    // VALUES
                    // =========================================

                    item.minimumValue =
                            root.minimumValue

                    item.maximumValue =
                            root.maximumValue

                    item.brightnessValue =
                            root.brightness

                    item.contrastValue =
                            root.contrast

                    item.dicomLoader = dicomLoader


                    // =====================================================
                    // BRIGHTNESS
                    // =====================================================

                    item.brightnessChangedByUser.connect(
                        function(value) {
                            dicomLoader.setBrightness(value)
                        })


                    // =====================================================
                    // CONTRAST
                    // =====================================================

                    item.contrastChangedByUser.connect(
                        function(value) {
                            dicomLoader.setContrast(value)
                        })


                    // =====================================================
                    // MINIMUM
                    // =====================================================

                    item.minimumChangedByUser.connect(
                        function(value) {
                            dicomLoader.setMinimum(value)
                        })


                    // =====================================================
                    // MAXIMUM
                    // =====================================================

                    item.maximumChangedByUser.connect(
                        function(value) {
                            dicomLoader.setMaximum(value)
                        })


                    // AUTO
                           item.autoRequested.connect(function() {

                               console.log("AUTO SIGNAL RECEIVED IN MAIN")

                               dicomLoader.autoImageProcessing()

                               console.log(
                                           "AUTO RESULT:",
                                           dicomLoader.minimum,
                                           dicomLoader.maximum)

                               root.minimumValue = dicomLoader.minimum
                               root.maximumValue = dicomLoader.maximum

                               item.minimumValue = dicomLoader.minimum
                               item.maximumValue = dicomLoader.maximum

                               item.brightnessValue = dicomLoader.brightness
                               item.contrastValue = dicomLoader.contrast
                           })
                    // =====================================================
                    // RESET
                    // =====================================================



                    item.resetRequested.connect(
                        function() {

                            console.log("RESET SIGNAL RECEIVED IN MAIN")

                            dicomLoader.resetImageProcessing()

                            // Update RightPanel from actual DicomLoader values
                            item.minimumValue = dicomLoader.minimum
                            item.maximumValue = dicomLoader.maximum

                            item.brightnessValue = dicomLoader.brightness
                            item.contrastValue = dicomLoader.contrast

                            console.log(
                                                "RESET RESULT:",
                                                dicomLoader.minimum,
                                                dicomLoader.maximum,
                                                dicomLoader.brightness,
                                                dicomLoader.contrast)
                        })

                    // =====================================================
                    // APPLY
                    // =====================================================

                    item.applyRequested.connect(
                        function() {

                            console.log("APPLY SIGNAL RECEIVED IN MAIN")

                            dicomLoader.applyImageProcessing()

                            console.log(
                                        "APPLY RESULT:",
                                        "Min =", dicomLoader.minimum,
                                        "Max =", dicomLoader.maximum,
                                        "Brightness =", dicomLoader.brightness,
                                        "Contrast =", dicomLoader.contrast)
                        })
                }
            }
        }


        // =====================================================
        // FOOTER
        // =====================================================

        Rectangle {

            Layout.fillWidth: true

            Layout.preferredHeight: 40

            color: "#0D1724"

            border.color: "#263548"

            border.width: 1

            RowLayout {

                anchors.fill: parent

                anchors.leftMargin: 4
                anchors.rightMargin: 4

                spacing: 8


                // STATUS DOT
                Rectangle {

                    width: 8
                    height: 8

                    radius: 4

                    color: {
                        switch (dicomLoader.statusType)
                        {
                        case "ready":
                            return "#22C55E"

                        case "editing":
                            return "#3B82F6"

                        case "saving":
                        case "loading":
                            return "#F59E0B"

                        case "saved":
                            return "#10B981"

                        case "notSaved":
                        case "error":
                            return "#EF4444"

                        default:
                            return "#718096"
                        }
                    }
                }


                // STATUS TEXT
                Text {

                    text: dicomLoader.statusText

                    color: {
                        switch (dicomLoader.statusType)
                        {
                        case "ready":
                            return "#22C55E"

                        case "editing":
                            return "#3B82F6"

                        case "saving":
                        case "loading":
                            return "#F59E0B"

                        case "saved":
                            return "#10B981"

                        case "notSaved":
                        case "error":
                            return "#EF4444"

                        default:
                            return "#718096"
                        }
                    }

                    font.pixelSize: 13
                }


                Item {
                    Layout.fillWidth: true
                }
                Text {

                    text: "DICOM Viewer v1.0"

                    color: "#94A3B8"

                    font.pixelSize: 12
                }
            }

       }
    }
}
