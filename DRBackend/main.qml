import QtQuick 2.12
import QtQuick.Controls 2.12
import QtQuick.Layouts 1.12
import DigitalRadiography 1.0

ApplicationWindow {
    id: root
    width: 1280
    height: 800
    visible: true
    title: "Digital Radiography Control System"

    property bool isConnected: backend.connectionStatus
    property string connectionStatus: backend.connectionStatus ? "Connected" : "Disconnected"
    property color connectionStatusColor: backend.connectionStatus ? "green" : "red"

    BackendManager {
        id: backend
        onConnectionStatusChanged: {
            isConnected = status
            connectionStatus = status ? "Connected" : "Disconnected"
            connectionStatusColor = status ? "green" : "red"
        }
        onErrorOccurred: {
            errorDialog.show(message)
        }
        onFeedbackReceived: {
            feedbackModel.updateFeedback(data)
        }
        onPositionUpdated: {
            positionModel.updatePosition(motorType, position)
        }
        onMotorStatusChanged: {
            statusModel.updateStatus(motorType, status)
        }
    }

    // Connection Dialog
    Dialog {
        id: connectionDialog
        title: "Board Connection"
        anchors.centerIn: parent
        modal: true
        standardButtons: Dialog.Ok | Dialog.Cancel

        ColumnLayout {
            width: parent.width
            TextField {
                id: ipAddressField
                Layout.fillWidth: true
                placeholderText: "IP Address"
                text: "192.168.3.20"
            }
            TextField {
                id: portField
                Layout.fillWidth: true
                placeholderText: "Port"
                text: "4444"
                validator: IntValidator { bottom: 1; top: 65535 }
            }
        }

        onAccepted: {
            backend.connectToBoard(ipAddressField.text, parseInt(portField.text))
        }
    }

    // Error Dialog
    Dialog {
        id: errorDialog
        title: "Error"
        anchors.centerIn: parent
        modal: true
        standardButtons: Dialog.Ok

        property alias message: errorText.text

        Label {
            id: errorText
            width: parent.width
            wrapMode: Text.Wrap
        }

        function show(msg) {
            message = msg
            open()
        }
    }

    // Main UI
    ColumnLayout {
        anchors.fill: parent
        spacing: 10

        // Header
        RowLayout {
            Layout.fillWidth: true
            spacing: 20

            Button {
                text: isConnected ? "Disconnect" : "Connect"
                onClicked: {
                    if (isConnected) {
                        backend.disconnectFromBoard()
                    } else {
                        connectionDialog.open()
                    }
                }
            }

            Label {
                text: connectionStatus
                color: connectionStatusColor
                font.bold: true
            }

            Button {
                text: "Reset All Motors"
                enabled: isConnected
                onClicked: backend.resetAllMotors()
            }

            Button {
                text: "Boot Parameters"
                enabled: isConnected
                onClicked: backend.bootParameters()
            }
        }

        // Feedback Display
        TabBar {
            id: tabBar
            Layout.fillWidth: true
            TabButton { text: "Motor Status" }
            TabButton { text: "Position Feedback" }
            TabButton { text: "Error Codes" }
        }

        StackLayout {
            Layout.fillWidth: true
            Layout.fillHeight: true
            currentIndex: tabBar.currentIndex

            // Motor Status Tab
            GridLayout {
                columns: 3
                columnSpacing: 20
                rowSpacing: 10

                Repeater {
                    model: [
                        { name: "Tube Lateral", id: BackendManager.TubeLateral },
                        { name: "Tube Longitudinal", id: BackendManager.TubeLongitudinal },
                        { name: "Tube Vertical", id: BackendManager.TubeVertical },
                        { name: "Tube Vertical Rotation", id: BackendManager.TubeVerticalRotation },
                        { name: "Tube Horizontal Rotation", id: BackendManager.TubeHorizontalRotation },
                        { name: "Detector Vertical", id: BackendManager.DetectorVertical },
                        { name: "Detector Tilt", id: BackendManager.DetectorTilt }
                    ]

                    delegate: ColumnLayout {
                        Label {
                            text: modelData.name
                            font.bold: true
                        }
                        Label {
                            text: statusModel.getMotorStatus(modelData.id)
                            color: statusModel.getMotorStatusColor(modelData.id)
                        }
                        RowLayout {
                            Button {
                                text: "Enable"
                                enabled: isConnected
                                onClicked: backend.motorEnable(modelData.id)
                            }
                            Button {
                                text: "Disable"
                                enabled: isConnected
                                onClicked: backend.motorDisable(modelData.id)
                            }
                            Button {
                                text: "Reset"
                                enabled: isConnected
                                onClicked: backend.resetMotor(modelData.id)
                            }
                        }
                    }
                }
            }

            // Position Feedback Tab
            TableView {
                model: positionModel
                columnWidthProvider: function(column) { return 150 }
                rowHeightProvider: function(row) { return 40 }

                delegate: Rectangle {
                    implicitWidth: 150
                    implicitHeight: 40
                    border.color: "lightgray"
                    Text {
                        text: display
                        anchors.centerIn: parent
                    }
                }
            }

            // Error Codes Tab
            ListView {
                model: errorModel
                delegate: ItemDelegate {
                    width: parent.width
                    text: model.display
                    highlighted: model.isError
                    onClicked: errorDialog.show(model.details)
                }
            }
        }

        // Control Panel
        RowLayout {
            Layout.fillWidth: true
            spacing: 10

            Button {
                text: "Start Auto Mode"
                enabled: isConnected
                onClicked: backend.startAutoMode()
            }

            Button {
                text: "Emergency Stop"
                enabled: isConnected
                onClicked: backend.emergencyStop()
            }
        }
    }

    // Models
    ListModel {
        id: feedbackModel
        dynamicRoles: true

        function updateFeedback(data) {
            clear();
            for (var key in data) {
                append({
                    motor: key,
                    position: data[key].position,
                    status: data[key].status,
                    errorCode: data[key].errorCode,
                    digitalInput: data[key].digitalInput
                })
            }
        }
    }

    ListModel {
        id: positionModel
        dynamicRoles: true

        function updatePosition(motorType, position) {
            for (var i = 0; i < count; i++) {
                if (get(i).motor === motorType) {
                    setProperty(i, "position", position);
                    return;
                }
            }
            append({ motor: motorType, position: position });
        }
    }

    ListModel {
        id: statusModel
        dynamicRoles: true

        function updateStatus(motorType, status) {
            for (var i = 0; i < count; i++) {
                if (get(i).motor === motorType) {
                    setProperty(i, "status", status);
                    return;
                }
            }
            append({ motor: motorType, status: status });
        }

        function getMotorStatus(motorId) {
            for (var i = 0; i < count; i++) {
                if (get(i).motor === motorId) {
                    return get(i).status === 1 ? "Running" : "Stopped";
                }
            }
            return "Unknown"
        }

        function getMotorStatusColor(motorId) {
            for (var i = 0; i < count; i++) {
                if (get(i).motor === motorId) {
                    return get(i).status === 1 ? "green" : "red";
                }
            }
            return "black"
        }
    }

    ListModel {
        id: errorModel
        dynamicRoles: true

        function updateErrors(errors) {
            clear();
            for (var i = 0; i < errors.length; i++) {
                append({
                    display: errors[i].code + ": " + errors[i].description,
                    details: errors[i].details,
                    isError: errors[i].isError
                })
            }
        }
    }
}
