import QtQuick 2.12
import QtQuick.Window 2.12
import QtQuick.Controls 2.12
import QtQuick.Layouts 1.12
import DRController 1.0

Window {
    width: 800
    height: 600
    visible: true
    title: "DR Controller Feedback"

    ProtocolHandler {
        id: protocolHandler
        client: socketClient

        onDisplayCountsChanged: {
            // Update UI when counts change
            positionView.updatePositions()
        }

        onErrorCodesChanged: {
            errorDisplay.text = "Error Codes: " + errorCodes
        }

        onConnectionStatusChanged: {
            connectionStatus.text = connected ? "Connected" : "Disconnected"
        }
    }

    SocketClient {
        id: socketClient
        ipAddress: "127.0.0.1"
        port: 1234
    }

    ColumnLayout {
        anchors.fill: parent
        spacing: 10

        Text {
            id: connectionStatus
            text: "Disconnected"
            font.bold: true
        }

        Button {
            text: "Connect"
            onClicked: {
                socketClient.connectToServer()
                protocolHandler.startThreads()
            }
        }

        Button {
            text: "Disconnect"
            onClicked: {
                protocolHandler.stopThreads()
                socketClient.disconnectFromServer()
            }
        }

        GridLayout {
            id: positionView
            columns: 2
            rowSpacing: 5
            columnSpacing: 10

            function updatePositions() {
                // This will be called when display counts change
                for (var i = 0; i < 7; i++) {
                    repeater.itemAt(i).value = protocolHandler["displayCount" + (i+1)]
                }
            }

            Repeater {
                model: [
                    "Tube Lateral",
                    "Tube Longitudinal",
                    "Tube Vertical",
                    "Tube Vertical Rotation",
                    "Tube Horizontal Rotation",
                    "Detector Vertical",
                    "Detector Tilt"
                ]

                delegate: RowLayout {
                    property int value: 0

                    Text {
                        text: modelData + ":"
                        Layout.minimumWidth: 150
                    }

                    Text {
                        text: value
                        font.bold: true
                    }
                }
            }
        }

        Text {
            id: errorDisplay
            text: "Error Codes: None"
        }

        ScrollView {
            Layout.fillHeight: true
            Layout.fillWidth: true

            TextArea {
                id: logDisplay
                readOnly: true
                wrapMode: Text.Wrap
            }
        }
    }

    Component.onCompleted: {
        socketClient.connectToServer()
        protocolHandler.startThreads()
    }
}
