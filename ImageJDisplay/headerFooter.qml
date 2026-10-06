import QtQuick 2.14
import QtQuick.Controls 2.14
import QtQuick.Layouts 1.14
import QtQuick.Dialogs 1.3

Rectangle {
    id: root

    color: "#101A29"
    border.color: "#29384B"
    border.width: 1

    signal dicomLoaded()
    signal saveRequested()
    signal histogramRequested()

    function loadSelectedFile(fileUrl)
    {
        console.log("Selected file URL:", fileUrl)

        var path = fileUrl.toString()

        if (path.indexOf("file:///") === 0) {
            path = path.substring(8)
        }

        path = decodeURIComponent(path)

        console.log("Local file path:", path)

        if (path.length === 0) {
            console.log("ERROR: Invalid file path")
            return
        }

        var result = dicomLoader.loadDicom(path)

        console.log("loadDicom result:", result)

        if (result) {
            console.log("DICOM loaded successfully")
            root.dicomLoaded()
        } else {
            console.log("ERROR: DICOM loading failed")
        }
    }

    RowLayout {
        anchors.fill: parent
        anchors.leftMargin: 18
        anchors.rightMargin: 18

        spacing: 8

        // =====================================================
        // APPLICATION TITLE
        // =====================================================

        Row {
            Layout.preferredWidth: 340

            spacing: 15

            Rectangle {
                width: 44
                height: 44

                anchors.verticalCenter: parent.verticalCenter

                radius: 6

                color: "#172638"
                border.color: "#29384B"

                Text {
                    anchors.centerIn: parent

                    text: "◆"

                    color: "#D7E2EF"

                    font.pixelSize: 24
                }
            }

            Column {
                anchors.verticalCenter: parent.verticalCenter

                spacing: 2

                Text {
                    text: "DICOM Viewer"

                    color: "#F1F5F9"

                    font.pixelSize: 25

                    font.bold: true
                }

                Text {
                    text: "Imaging & Enhancement Suite"

                    color: "#94A3B8"

                    font.pixelSize: 13
                }
            }
        }

        Item {
            Layout.fillWidth: true
        }

        // =====================================================
        // OPEN
        // =====================================================

        ToolButton {
            text: "⌂\nOpen"

            Layout.preferredWidth: 62
            Layout.preferredHeight: 62

            hoverEnabled: true

            contentItem: Text {
                text: parent.text

                color: "#D6DEE8"

                font.pixelSize: 12

                horizontalAlignment: Text.AlignHCenter
                verticalAlignment: Text.AlignVCenter

                lineHeight: 1.3
            }

            background: Rectangle {
                radius: 4

                color: parent.hovered
                       ? "#253850"
                       : "#182434"

                border.color: "#25364B"
                border.width: 1
            }

            onClicked: {
                console.log("Open button clicked")

                loadDialog.open()
            }
        }

        // =====================================================
        // SAVE
        // =====================================================

        ToolButton {
            text: "□\nSave"

            Layout.preferredWidth: 62
            Layout.preferredHeight: 62

            hoverEnabled: true

            contentItem: Text {
                text: parent.text

                color: "#D6DEE8"

                font.pixelSize: 12

                horizontalAlignment: Text.AlignHCenter
                verticalAlignment: Text.AlignVCenter
            }

            background: Rectangle {
                radius: 4

                color: parent.hovered
                       ? "#253850"
                       : "#182434"

                border.color: "#25364B"
                border.width: 1
            }

            onClicked: {
                console.log("Save clicked")
                root.saveRequested()
                console.log("saveRequested emitted")
            }
        }

        // =====================================================
        // SERIES
        // =====================================================

        ToolButton {
            text: "⊙\nSeries"

            Layout.preferredWidth: 62
            Layout.preferredHeight: 62

            contentItem: Text {
                text: parent.text
                color: "#D6DEE8"
                font.pixelSize: 12
                horizontalAlignment: Text.AlignHCenter
                verticalAlignment: Text.AlignVCenter
            }

            background: Rectangle {
                radius: 4
                color: parent.hovered ? "#253850" : "#182434"
                border.color: "#25364B"
            }
        }

        // =====================================================
        // LAYOUT
        // =====================================================

        ToolButton {
            text: "⊞\nLayout"

            Layout.preferredWidth: 62
            Layout.preferredHeight: 62

            contentItem: Text {
                text: parent.text
                color: "#D6DEE8"
                font.pixelSize: 12
                horizontalAlignment: Text.AlignHCenter
                verticalAlignment: Text.AlignVCenter
            }

            background: Rectangle {
                radius: 4
                color: parent.hovered ? "#253850" : "#182434"
                border.color: "#25364B"
            }
        }

        // =====================================================
        // ZOOM
        // =====================================================

        ToolButton {
            text: "−\nZoom"

            Layout.preferredWidth: 62
            Layout.preferredHeight: 62

            contentItem: Text {
                text: parent.text
                color: "#D6DEE8"
                font.pixelSize: 12
                horizontalAlignment: Text.AlignHCenter
                verticalAlignment: Text.AlignVCenter
            }

            background: Rectangle {
                radius: 4
                color: parent.hovered ? "#253850" : "#182434"
                border.color: "#25364B"
            }
        }
        ToolButton {
            text: "+\nZoom"

            Layout.preferredWidth: 62
            Layout.preferredHeight: 62

            contentItem: Text {
                text: parent.text
                color: "#D6DEE8"
                font.pixelSize: 12
                horizontalAlignment: Text.AlignHCenter
                verticalAlignment: Text.AlignVCenter
            }

            background: Rectangle {
                radius: 4
                color: parent.hovered ? "#253850" : "#182434"
                border.color: "#25364B"
            }
        }

        // =====================================================
        // PAN
        // =====================================================

        ToolButton {
            text: "↥\nPan"

            Layout.preferredWidth: 62
            Layout.preferredHeight: 62

            contentItem: Text {
                text: parent.text
                color: "#D6DEE8"
                font.pixelSize: 12
                horizontalAlignment: Text.AlignHCenter
                verticalAlignment: Text.AlignVCenter
            }

            background: Rectangle {
                radius: 4
                color: parent.hovered ? "#253850" : "#182434"
                border.color: "#25364B"
            }
        }

        // =====================================================
        // WINDOW
        // =====================================================

        ToolButton {
            text: "⤢\nTransform"

            Layout.preferredWidth: 62
            Layout.preferredHeight: 62

            contentItem: Text {
                text: parent.text
                color: "#D6DEE8"
                font.pixelSize: 12
                horizontalAlignment: Text.AlignHCenter
                verticalAlignment: Text.AlignVCenter
            }

            background: Rectangle {
                radius: 4
                color: parent.hovered ? "#253850" : "#182434"
                border.color: "#25364B"
            }
        }


        // =====================================================
        // ROTATE
        // =====================================================

        ToolButton {
            text: "↻\nRotate"

            Layout.preferredWidth: 62
            Layout.preferredHeight: 62

            contentItem: Text {
                text: parent.text
                color: "#D6DEE8"
                font.pixelSize: 12
                horizontalAlignment: Text.AlignHCenter
                verticalAlignment: Text.AlignVCenter
            }

            background: Rectangle {
                radius: 4
                color: parent.hovered ? "#253850" : "#182434"
                border.color: "#25364B"
            }
        }

        // =====================================================
        // INVERT
        // =====================================================

        ToolButton {
            text: "↔\nInvert"

            Layout.preferredWidth: 62
            Layout.preferredHeight: 62

            contentItem: Text {
                text: parent.text
                color: "#D6DEE8"
                font.pixelSize: 12
                horizontalAlignment: Text.AlignHCenter
                verticalAlignment: Text.AlignVCenter
            }

            background: Rectangle {
                radius: 4
                color: parent.hovered ? "#253850" : "#182434"
                border.color: "#25364B"
            }
        }

        // =====================================================
        // Histogram
        // =====================================================

        ToolButton {
            id: histogramButton

            text: "▥\nHistogram"

            Layout.preferredWidth: 62
            Layout.preferredHeight: 62

            hoverEnabled: true

            contentItem: Text {
                text: histogramButton.text

                color: "#D6DEE8"
                font.pixelSize: 12

                horizontalAlignment: Text.AlignHCenter
                verticalAlignment: Text.AlignVCenter
            }

            background: Rectangle {
                radius: 4

                color: histogramButton.pressed
                       ? "#2F5F96"
                       : histogramButton.hovered
                         ? "#253850"
                         : "#182434"

                border.color: histogramButton.hovered
                              ? "#3B82F6"
                              : "#25364B"

                border.width: 1
            }

            onClicked: {
                console.log("Histogram clicked")

                root.histogramRequested()
            }
        }


        // =====================================================
        // RESET
        // =====================================================

        ToolButton {
            text: "↶\nReset"

            Layout.preferredWidth: 62
            Layout.preferredHeight: 62

            contentItem: Text {
                text: parent.text
                color: "#D6DEE8"
                font.pixelSize: 12
                horizontalAlignment: Text.AlignHCenter
                verticalAlignment: Text.AlignVCenter
            }

            background: Rectangle {
                radius: 4
                color: parent.hovered ? "#253850" : "#182434"
                border.color: "#25364B"
            }

            onClicked: {
                dicomLoader.clear()
            }
        }
    }

    // =========================================================
    // FILE OPEN DIALOG
    // =========================================================

    FileDialog {
        id: loadDialog

        title: "Open DICOM Image"

        selectExisting: true

        nameFilters: [
            "DICOM Files (*.dcm *.dicom)",
              "RAW Files (*.raw)",
            "All Files (*)"
        ]

        onAccepted: {
            root.loadSelectedFile(fileUrl)
        }
    }
}
