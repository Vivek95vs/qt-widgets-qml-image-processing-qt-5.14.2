//import QtQuick 2.12
//import QtQuick.Controls 2.12
//import QtQuick.Layouts 1.12
//import QtQuick.Dialogs 1.2
//import DicomApp 1.0

//ApplicationWindow {
//    visible: true
//    width: 1200
//    height: 800
//    title: "DICOM PACS Client"

//    // Properties
//    property string downloadPath: "D:/DICOM_Downloads"
//    property string pendingStudyUID: ""
//    property real progressValue: 0

//    // Main layout
//    ColumnLayout {
//        anchors.fill: parent
//        anchors.margins: 10
//        spacing: 10

//        // Header Section
//        Rectangle {
//            Layout.fillWidth: true
//            Layout.preferredHeight: 100
//            color: "#2c3e50"
//            radius: 5

//            RowLayout {
//                anchors.fill: parent
//                anchors.margins: 10

//                Column {
//                    Layout.fillWidth: true
//                    spacing: 5

//                    Text {
//                        text: "DICOM PACS Client"
//                        font.pixelSize: 24
//                        font.bold: true
//                        color: "white"
//                    }

//                    Text {
//                        text: "Local AE Title: VIVEK_VS | Port: 5554"
//                        color: "#ecf0f1"
//                        font.pixelSize: 12
//                    }
//                }

//                // Status indicator
//                Rectangle {
//                    width: 20
//                    height: 20
//                    radius: 10
//                    color: dicomManager.isTransferring ? "#e74c3c" : "#2ecc71"
//                }

//                Text {
//                    text: dicomManager.currentStatus || "Ready"
//                    color: "white"
//                    font.pixelSize: 12
//                }
//            }
//        }

//        // Main Content Area (Left and Right panels)
//        RowLayout {
//            Layout.fillWidth: true
//            Layout.fillHeight: true
//            spacing: 10

//            // LEFT PANEL - Query and Results
//            Rectangle {
//                Layout.fillWidth: true
//                Layout.fillHeight: true
//                color: "#ecf0f1"
//                radius: 5

//                ColumnLayout {
//                    anchors.fill: parent
//                    anchors.margins: 10
//                    spacing: 10

//                    // Title
//                    Text {
//                        text: "Patient Study Query"
//                        font.bold: true
//                        font.pixelSize: 16
//                        color: "#2c3e50"
//                    }

//                    // Search area - Only Patient ID
//                    Rectangle {
//                        Layout.fillWidth: true
//                        Layout.preferredHeight: 80
//                        color: "white"
//                        radius: 3

//                        ColumnLayout {
//                            anchors.fill: parent
//                            anchors.margins: 10
//                            spacing: 8

//                            Text {
//                                text: "Enter Patient ID to Search"
//                                font.bold: true
//                                font.pixelSize: 12
//                                color: "#2c3e50"
//                            }

//                            RowLayout {
//                                Layout.fillWidth: true
//                                spacing: 5

//                                TextField {
//                                    id: patientIdField
//                                    Layout.fillWidth: true
//                                    placeholderText: "Enter Patient ID (e.g., GATC1)..."
//                                    onAccepted: {
//                                        if (text.trim() !== "") {
//                                            dicomManager.queryStudies(text.trim())
//                                        } else {
//                                            dicomManager.logMessage("WARNING", "Please enter a Patient ID")
//                                        }
//                                    }
//                                }

//                                Button {
//                                    text: "Search"
//                                    Layout.preferredWidth: 80
//                                    onClicked: {
//                                        if (patientIdField.text.trim() !== "") {
//                                            dicomManager.queryStudies(patientIdField.text.trim())
//                                        } else {
//                                            dicomManager.logMessage("WARNING", "Please enter a Patient ID")
//                                        }
//                                    }
//                                }
//                            }
//                        }
//                    }

//                    // Results list
//                    Rectangle {
//                        Layout.fillWidth: true
//                        Layout.fillHeight: true
//                        color: "white"
//                        radius: 3

//                        ColumnLayout {
//                            anchors.fill: parent
//                            anchors.margins: 10
//                            spacing: 5

//                            RowLayout {
//                                Layout.fillWidth: true

//                                Text {
//                                    text: "Studies Found: " + dicomManager.studies.length
//                                    font.bold: true
//                                    font.pixelSize: 12
//                                    Layout.fillWidth: true
//                                }

//                                Text {
//                                    text: "Patient ID: " + patientIdField.text
//                                    font.pixelSize: 11
//                                    color: "#7f8c8d"
//                                    visible: patientIdField.text !== ""
//                                }
//                            }

//                            ListView {
//                                id: studiesList
//                                Layout.fillWidth: true
//                                Layout.fillHeight: true
//                                model: dicomManager.studies
//                                clip: true

//                                delegate: Rectangle {
//                                    width: parent.width
//                                    height: 120
//                                    color: index % 2 ? "#f8f9fa" : "white"
//                                    border.color: "#dee2e6"
//                                    border.width: 1
//                                    radius: 3

//                                    ColumnLayout {
//                                        anchors.fill: parent
//                                        anchors.margins: 10
//                                        spacing: 5

//                                        Text {
//                                            text: modelData.patientName || "Unknown Patient"
//                                            font.bold: true
//                                            font.pixelSize: 14
//                                            color: "#2c3e50"
//                                        }

//                                        Text {
//                                            text: "Patient ID: " + (modelData.patientId || "N/A")
//                                            font.pixelSize: 11
//                                            color: "#7f8c8d"
//                                        }

//                                        Text {
//                                            text: "Study Date: " + (modelData.studyDate || "N/A")
//                                            font.pixelSize: 11
//                                            color: "#7f8c8d"
//                                        }

//                                        Text {
//                                            text: "Modalities: " + (modelData.modalities || "N/A")
//                                            font.pixelSize: 11
//                                            color: "#7f8c8d"
//                                        }

//                                        Text {
//                                            text: "Instances: " + (modelData.instances || "N/A")
//                                            font.pixelSize: 11
//                                            color: "#7f8c8d"
//                                        }

//                                        Button {
//                                            text: "Download Study"
//                                            Layout.alignment: Qt.AlignRight
//                                            Layout.preferredWidth: 120
//                                            onClicked: {
//                                                pendingStudyUID = modelData.studyUID
//                                                downloadPathDialog.open()
//                                            }
//                                        }
//                                    }
//                                }

//                                // Empty message when no results
//                                Rectangle {
//                                    visible: studiesList.count === 0
//                                    anchors.centerIn: parent
//                                    width: parent.width - 20
//                                    height: 100
//                                    color: "#f8f9fa"
//                                    radius: 5

//                                    ColumnLayout {
//                                        anchors.centerIn: parent
//                                        spacing: 10

//                                        Text {
//                                            text: "No studies found"
//                                            font.pixelSize: 14
//                                            color: "#7f8c8d"
//                                            horizontalAlignment: Text.AlignHCenter
//                                        }

//                                        Text {
//                                            text: "Enter a Patient ID and click Search"
//                                            font.pixelSize: 11
//                                            color: "#95a5a6"
//                                            horizontalAlignment: Text.AlignHCenter
//                                        }
//                                    }
//                                }
//                            }
//                        }
//                    }
//                }
//            }

//            // RIGHT PANEL - Send to PACS and Logs (same as before)
//            Rectangle {
//                Layout.preferredWidth: 400
//                Layout.fillHeight: true
//                color: "#ecf0f1"
//                radius: 5

//                ColumnLayout {
//                    anchors.fill: parent
//                    anchors.margins: 10
//                    spacing: 10

//                    // Send to PACS section
//                    Rectangle {
//                        Layout.fillWidth: true
//                        Layout.preferredHeight: 200
//                        color: "white"
//                        radius: 3

//                        ColumnLayout {
//                            anchors.fill: parent
//                            anchors.margins: 10
//                            spacing: 8

//                            Text {
//                                text: "Send to PACS"
//                                font.bold: true
//                                font.pixelSize: 14
//                                color: "#2c3e50"
//                            }

//                            ComboBox {
//                                id: serverCombo
//                                Layout.fillWidth: true
//                                model: dicomManager.servers
//                                textRole: "name"
//                            }

//                            Button {
//                                text: "Select DICOM File"
//                                Layout.fillWidth: true
//                                onClicked: fileDialog.open()
//                            }

//                            Button {
//                                text: "Select Directory"
//                                Layout.fillWidth: true
//                                onClicked: directoryDialog.open()
//                            }

//                            Button {
//                                text: "Test Connection"
//                                Layout.fillWidth: true
//                                onClicked: {
////                                    if (serverCombo.currentText) {
////                                        dicomManager.testConnection(serverCombo.currentText)
////                                    }
//                                    dicomManager.testConnection("STORAGESCP")
////                                    dicomManager.testConnection("SUVARNAQRY")
////                                    dicomManager.testConnection("STORAGESCP")
//                                }
//                            }
//                            Button {
//                                text: "Test Connection 1"
//                                Layout.fillWidth: true
//                                onClicked: {
////                                    if (serverCombo.currentText) {
////                                        dicomManager.testConnection(serverCombo.currentText)
////                                    }
////                                    dicomManager.testConnection("STORAGESCP")
//                                    dicomManager.testConnection("SUVARNAQRY")
////                                    dicomManager.testConnection("STORAGESCP")
//                                }
//                            }
//                        }
//                    }

//                    // Log area
//                    Rectangle {
//                        Layout.fillWidth: true
//                        Layout.fillHeight: true
//                        color: "#2c3e50"
//                        radius: 3

//                        ColumnLayout {
//                            anchors.fill: parent
//                            anchors.margins: 10
//                            spacing: 5

//                            RowLayout {
//                                Layout.fillWidth: true

//                                Text {
//                                    text: "Log Messages"
//                                    font.bold: true
//                                    font.pixelSize: 12
//                                    color: "white"
//                                    Layout.fillWidth: true
//                                }

//                                Button {
//                                    text: "Clear"
//                                    onClicked: logArea.clear()
//                                }
//                            }

//                            ScrollView {
//                                Layout.fillWidth: true
//                                Layout.fillHeight: true

//                                TextArea {
//                                    id: logArea
//                                    readOnly: true
//                                    color: "#ecf0f1"
//                                    font.pixelSize: 11
//                                    font.family: "Monospace"
//                                    placeholderText: "Log messages will appear here..."
//                                    background: null
//                                    wrapMode: TextEdit.Wrap
//                                }
//                            }
//                        }
//                    }
//                }
//            }
//        }

//        // Progress bar
//        Rectangle {
//            Layout.fillWidth: true
//            Layout.preferredHeight: 30
//            color: "#ecf0f1"
//            radius: 3
//            visible: dicomManager.isTransferring

//            ProgressBar {
//                id: progressBar
//                anchors.fill: parent
//                anchors.margins: 2
//                value: progressValue
//            }
//        }

//        // Status bar
//        Rectangle {
//            Layout.fillWidth: true
//            Layout.preferredHeight: 25
//            color: "#34495e"
//            radius: 3

//            RowLayout {
//                anchors.fill: parent
//                anchors.margins: 5

//                Text {
//                    text: dicomManager.currentStatus || "Ready"
//                    color: "white"
//                    font.pixelSize: 10
//                    Layout.fillWidth: true
//                }

//                Text {
//                    text: "Last Query: " + (patientIdField.text || "None")
//                    color: "white"
//                    font.pixelSize: 10
//                }
//            }
//        }
//    }

//    // Dialogs
//    FileDialog {
//        id: fileDialog
//        title: "Select DICOM File"
//        nameFilters: ["DICOM files (*.dcm *.DCM)", "All files (*.*)"]
//        onAccepted: {
//            var serverName = serverCombo.currentText
//            var filePath = fileUrl.toString().replace("file:///", "")
//            if (filePath.startsWith("/")) filePath = filePath.substring(1)
//            dicomManager.sendToPACS(filePath, serverName)
//        }
//    }

//    FileDialog {
//        id: directoryDialog
//        title: "Select Directory with DICOM Files"
//        selectFolder: true
//        onAccepted: {
//            var serverName = serverCombo.currentText
//            var folderPath = folder.toString().replace("file:///", "")
//            if (folderPath.startsWith("/")) folderPath = folderPath.substring(1)
//            dicomManager.sendDirectoryToPACS(folderPath, serverName)
//        }
//    }

//    FileDialog {
//        id: downloadPathDialog
//        title: "Select Download Directory"
//        selectFolder: true
//        onAccepted: {
//            downloadPath = folder.toString().replace("file:///", "")
//            if (downloadPath.startsWith("/")) downloadPath = downloadPath.substring(1)
//            dicomManager.downloadStudy(pendingStudyUID, downloadPath)
//        }
//    }

//    // Signal connections
//    Connections {
//        target: dicomManager

//        onLogMessage: {
//            var timestamp = new Date().toLocaleTimeString()
//            logArea.append("[" + timestamp + "] [" + level + "] " + message)
//        }

//        onOperationCompleted: {
//            var timestamp = new Date().toLocaleTimeString()
//            if (success) {
//                logArea.append("[" + timestamp + "] [SUCCESS] " + message)
//            } else {
//                logArea.append("[" + timestamp + "] [ERROR] " + message)
//            }
//        }

//        onProgressChanged: {
//            progressValue = percent / 100
//        }
//    }

import QtQuick 2.12
import QtQuick.Controls 2.12
import QtQuick.Layouts 1.12
import QtQuick.Dialogs 1.2
import DicomApp 1.0

ApplicationWindow {
    visible: true
    width: 1200
    height: 800
    title: "DICOM PACS Client"

    // Properties
    property string downloadPath: "C:/DICOM_Downloads"
    property string pendingStudyUID: ""
    property real progressValue: 0

    // Main layout
    ColumnLayout {
        anchors.fill: parent
        anchors.margins: 10
        spacing: 10

        // Header Section
        Rectangle {
            Layout.fillWidth: true
            Layout.preferredHeight: 100
            color: "#2c3e50"
            radius: 5

            RowLayout {
                anchors.fill: parent
                anchors.margins: 10

                Column {
                    Layout.fillWidth: true
                    spacing: 5

                    Text {
                        text: "DICOM PACS Client"
                        font.pixelSize: 24
                        font.bold: true
                        color: "white"
                    }

                    Text {
                        text: "Local AE Title: VIVEK_VS | Port: 5554"
                        color: "#ecf0f1"
                        font.pixelSize: 12
                    }
                }

                // Status indicator
                Rectangle {
                    width: 20
                    height: 20
                    radius: 10
                    color: dicomManager.isTransferring ? "#e74c3c" : "#2ecc71"
                }

                Text {
                    text: dicomManager.currentStatus || "Ready"
                    color: "white"
                    font.pixelSize: 12
                }
            }
        }

        // Main Content Area
        RowLayout {
            Layout.fillWidth: true
            Layout.fillHeight: true
            spacing: 10

            // LEFT PANEL - Query Section
            Rectangle {
                Layout.fillWidth: true
                Layout.fillHeight: true
                color: "#ecf0f1"
                radius: 5

                ColumnLayout {
                    anchors.fill: parent
                    anchors.margins: 10
                    spacing: 10

                    Text {
                        text: "Patient Study Query"
                        font.bold: true
                        font.pixelSize: 16
                        color: "#2c3e50"
                    }

                    // Search area
                    Rectangle {
                        Layout.fillWidth: true
                        Layout.preferredHeight: 80
                        color: "white"
                        radius: 3

                        ColumnLayout {
                            anchors.fill: parent
                            anchors.margins: 10
                            spacing: 8

                            Text {
                                text: "Enter Patient ID to Search"
                                font.bold: true
                                font.pixelSize: 12
                                color: "#2c3e50"
                            }

                            RowLayout {
                                Layout.fillWidth: true
                                spacing: 5

                                TextField {
                                    id: patientIdField
                                    Layout.fillWidth: true
                                    placeholderText: "Enter Patient ID (e.g., GATC1)..."
                                    onAccepted: {
                                        if (text.trim() !== "") {
                                            dicomManager.queryStudies(text.trim())
                                        }
                                    }
                                }

                                Button {
                                    text: "Search"
                                    Layout.preferredWidth: 80
                                    onClicked: {
                                        if (patientIdField.text.trim() !== "") {
                                            dicomManager.queryStudies(patientIdField.text.trim())
                                        }
                                    }
                                }
                            }
                        }
                    }

                    // Results list
                    Rectangle {
                        Layout.fillWidth: true
                        Layout.fillHeight: true
                        color: "white"
                        radius: 3

                        ColumnLayout {
                            anchors.fill: parent
                            anchors.margins: 10
                            spacing: 5

                            Text {
                                text: "Studies Found: " + dicomManager.studies.length
                                font.bold: true
                                font.pixelSize: 12
                            }

                            ListView {
                                id: studiesList
                                Layout.fillWidth: true
                                Layout.fillHeight: true
                                model: dicomManager.studies
                                clip: true

                                delegate: Rectangle {
                                    width: parent.width
                                    height: 120
                                    color: index % 2 ? "#f8f9fa" : "white"
                                    border.color: "#dee2e6"
                                    border.width: 1
                                    radius: 3

                                    ColumnLayout {
                                        anchors.fill: parent
                                        anchors.margins: 10
                                        spacing: 5

                                        Text {
                                            text: modelData.patientName || "Unknown Patient"
                                            font.bold: true
                                            font.pixelSize: 14
                                            color: "#2c3e50"
                                        }

                                        Text {
                                            text: "Patient ID: " + (modelData.patientId || "N/A")
                                            font.pixelSize: 11
                                            color: "#7f8c8d"
                                        }

                                        Text {
                                            text: "Study Date: " + (modelData.studyDate || "N/A")
                                            font.pixelSize: 11
                                            color: "#7f8c8d"
                                        }

                                        Text {
                                            text: "Modalities: " + (modelData.modalities || "N/A")
                                            font.pixelSize: 11
                                            color: "#7f8c8d"
                                        }

                                        Button {
                                            text: "Download Study"
                                            Layout.alignment: Qt.AlignRight
                                            Layout.preferredWidth: 120
                                            onClicked: {
                                                pendingStudyUID = modelData.studyUID
                                                downloadPathDialog.open()
                                            }
                                        }
                                    }
                                }
                            }
                        }
                    }
                }
            }

            // RIGHT PANEL - Send to PACS and Logs
            Rectangle {
                Layout.preferredWidth: 500
                Layout.fillHeight: true
                color: "#ecf0f1"
                radius: 5

                ColumnLayout {
                    anchors.fill: parent
                    anchors.margins: 10
                    spacing: 10

                    // Send to PACS section
                    Rectangle {
                        Layout.fillWidth: true
                        Layout.preferredHeight: 180
                        color: "white"
                        radius: 3

                        ColumnLayout {
                            anchors.fill: parent
                            anchors.margins: 10
                            spacing: 8

                            Text {
                                text: "Send to PACS"
                                font.bold: true
                                font.pixelSize: 14
                                color: "#2c3e50"
                            }

                            ComboBox {
                                id: serverCombo
                                Layout.fillWidth: true
                                model: dicomManager.servers
                                textRole: "name"
                            }

                            Button {
                                text: "Select DICOM File"
                                Layout.fillWidth: true
                                onClicked: fileDialog.open()
                            }

                            Button {
                                text: "Select Directory"
                                Layout.fillWidth: true
                                onClicked: directoryDialog.open()
                            }

                            Button {
                                text: "Test Connection"
                                Layout.fillWidth: true
                                onClicked: {
                                    if (serverCombo.currentText) {
                                        dicomManager.testConnection(serverCombo.currentText)
                                    }
                                }
                            }
                        }
                    }

                    // Log area with copy functionality
                    Rectangle {
                        Layout.fillWidth: true
                        Layout.fillHeight: true
                        color: "#1e1e1e"
                        radius: 3

                        ColumnLayout {
                            anchors.fill: parent
                            anchors.margins: 10
                            spacing: 5

                            RowLayout {
                                Layout.fillWidth: true

                                Text {
                                    text: "Log Messages (Select text with mouse to copy)"
                                    font.bold: true
                                    font.pixelSize: 12
                                    color: "white"
                                    Layout.fillWidth: true
                                }

                                Button {
                                    text: "Copy Selected"
                                    onClicked: {
                                        logTextArea.copy()
                                        copyNotification.visible = true
                                        copyNotificationTimer.start()
                                    }
                                }

                                Button {
                                    text: "Copy All"
                                    onClicked: {
                                        logTextArea.selectAll()
                                        logTextArea.copy()
                                        logTextArea.deselect()
                                        copyNotification.visible = true
                                        copyNotificationTimer.start()
                                    }
                                }

                                Button {
                                    text: "Clear Logs"
                                    onClicked: logTextArea.clear()
                                }
                            }

                            // Scrollable text area with mouse selection support
                            ScrollView {
                                Layout.fillWidth: true
                                Layout.fillHeight: true
                                clip: true

                                TextArea {
                                    id: logTextArea
                                    readOnly: true
                                    color: "#d4d4d4"
                                    font.pixelSize: 11
                                    font.family: "Monospace"
                                    placeholderText: "Log messages will appear here...\nYou can select any text with mouse and copy using Ctrl+C or the Copy button"
                                    background: null
                                    wrapMode: TextEdit.Wrap
                                    selectByMouse: true
                                    selectionColor: "#264f78"
                                    selectedTextColor: "white"

                                    // Context menu for copy
                                    onPressAndHold: {
                                        contextMenu.open()
                                    }

//                                    ContextMenu {
//                                        id: contextMenu
//                                        MenuItem {
//                                            text: "Copy"
//                                            onTriggered: logTextArea.copy()
//                                        }
//                                        MenuItem {
//                                            text: "Select All"
//                                            onTriggered: logTextArea.selectAll()
//                                        }
//                                    }
                                }
                            }
                        }
                    }
                }
            }
        }

        // Progress bar
        Rectangle {
            Layout.fillWidth: true
            Layout.preferredHeight: 30
            color: "#ecf0f1"
            radius: 3
            visible: dicomManager.isTransferring

            ProgressBar {
                id: progressBar
                anchors.fill: parent
                anchors.margins: 2
                value: progressValue

                background: Rectangle {
                    color: "#bdc3c7"
                    radius: 2
                }

                contentItem: Item {
                    Rectangle {
                        width: progressBar.visualPosition * parent.width
                        height: parent.height
                        color: "#3498db"
                        radius: 2
                    }
                }
            }
        }

        // Status bar
        Rectangle {
            Layout.fillWidth: true
            Layout.preferredHeight: 25
            color: "#34495e"
            radius: 3

            RowLayout {
                anchors.fill: parent
                anchors.margins: 5

                Text {
                    text: dicomManager.currentStatus || "Ready"
                    color: "white"
                    font.pixelSize: 10
                    Layout.fillWidth: true
                }

                Text {
                    text: "Last Query: " + (patientIdField.text || "None")
                    color: "white"
                    font.pixelSize: 10
                }
            }
        }
    }

    // Copy notification
    Rectangle {
        id: copyNotification
        anchors.centerIn: parent
        width: 200
        height: 40
        color: "#2ecc71"
        radius: 5
        visible: false
        opacity: 0.9
        z: 100

        Text {
            anchors.centerIn: parent
            text: "Copied to clipboard!"
            color: "white"
            font.bold: true
        }
    }

    Timer {
        id: copyNotificationTimer
        interval: 2000
        onTriggered: copyNotification.visible = false
    }

    // Dialogs
    FileDialog {
        id: fileDialog
        title: "Select DICOM File"
        nameFilters: ["DICOM files (*.dcm *.DCM)", "All files (*.*)"]
        onAccepted: {
            var serverName = serverCombo.currentText
            var filePath = fileUrl.toString().replace("file:///", "")
            if (filePath.startsWith("/")) filePath = filePath.substring(1)
            dicomManager.sendToPACS(filePath, serverName)
        }
    }

    FileDialog {
        id: directoryDialog
        title: "Select Directory with DICOM Files"
        selectFolder: true
        onAccepted: {
            var serverName = serverCombo.currentText
            var folderPath = folder.toString().replace("file:///", "")
            if (folderPath.startsWith("/")) folderPath = folderPath.substring(1)
            dicomManager.sendDirectoryToPACS(folderPath, serverName)
        }
    }

    FileDialog {
        id: downloadPathDialog
        title: "Select Download Directory"
        selectFolder: true
        onAccepted: {
            downloadPath = folder.toString().replace("file:///", "")
            if (downloadPath.startsWith("/")) downloadPath = downloadPath.substring(1)
            dicomManager.downloadStudy(pendingStudyUID, downloadPath)
        }
    }

    // Signal connections for logs
    Connections {
        target: dicomManager

        onLogMessage: {
            var timestamp = new Date().toLocaleTimeString()
            logTextArea.append("[" + timestamp + "] [" + level + "] " + message)

            // Auto-scroll to bottom
            logTextArea.cursorPosition = logTextArea.length
        }

        onOperationCompleted: {
            var timestamp = new Date().toLocaleTimeString()
            if (success) {
                logTextArea.append("[" + timestamp + "] [SUCCESS] " + message)
            } else {
                logTextArea.append("[" + timestamp + "] [ERROR] " + message)
            }
            logTextArea.cursorPosition = logTextArea.length
        }

        onProgressChanged: {
            progressValue = percent / 100
        }
    }

    // Keyboard shortcuts for copy
    Shortcut {
        sequence: "Ctrl+C"
        onActivated: {
            if (logTextArea.selectedText) {
                logTextArea.copy()
                copyNotification.visible = true
                copyNotificationTimer.start()
            }
        }
    }

    Shortcut {
        sequence: "Ctrl+A"
        onActivated: {
            logTextArea.selectAll()
        }
    }

    // Auto-query on startup
    Component.onCompleted: {
        console.log("Application started, loading configuration...")
        // Add initial log message
        logTextArea.append("[INFO] Application started")
        logTextArea.append("[INFO] Ready to query PACS")
    }
}
//}
