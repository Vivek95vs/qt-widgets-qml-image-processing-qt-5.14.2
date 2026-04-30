// main.qml
import QtQuick 2.12
import QtQuick.Controls 2.12
import QtQuick.Layouts 1.12

ApplicationWindow {
    id: mainWindow
    width: 900
    height: 650
    visible: true
    title: "My Application"

    // Main Content - Only visible when license is valid
    Rectangle {
        anchors.fill: parent
        color: "#ecf0f1"
        visible: licenseValidator.isValid

        ColumnLayout {
            anchors.centerIn: parent
            spacing: 20

            Text {
                text: "✓ Application Active"
                font.pixelSize: 32
                font.bold: true
                color: "#27ae60"
                Layout.alignment: Qt.AlignHCenter
            }

            Text {
                text: "Days Remaining: " + licenseValidator.daysRemaining
                font.pixelSize: 18
                color: licenseValidator.daysRemaining < 30 ? "#e67e22" : "#2c3e50"
                Layout.alignment: Qt.AlignHCenter
            }

            Rectangle {
                width: 300
                height: 2
                color: "#bdc3c7"
                Layout.alignment: Qt.AlignHCenter
            }

            ColumnLayout {
                spacing: 10

                Text {
                    text: "License Information"
                    font.bold: true
                    font.pixelSize: 14
                    Layout.alignment: Qt.AlignHCenter
                }

                Text {
                    text: "Serial Number: " + licenseValidator.serialNumber
                    font.pixelSize: 12
                }

                Text {
                    text: licenseValidator.message
                    font.pixelSize: 11
                    color: "#7f8c8d"
                    wrapMode: Text.WordWrap
                    Layout.maximumWidth: 400
                    Layout.alignment: Qt.AlignHCenter
                    horizontalAlignment: Text.AlignHCenter
                }
            }

            Button {
                text: "License Information"
                onClicked: licenseInfoDialog.open()
                Layout.alignment: Qt.AlignHCenter
            }
        }
    }

    // Activation Window - Shown when no valid license
    Rectangle {
        anchors.fill: parent
        color: "#ecf0f1"
        visible: !licenseValidator.isValid

        ColumnLayout {
            anchors.centerIn: parent
            width: 500
            spacing: 15

            Text {
                text: "License Activation"
                font.pixelSize: 28
                font.bold: true
                color: "#2c3e50"
                Layout.alignment: Qt.AlignHCenter
            }

            Text {
                text: "Please activate your license to use this application"
                color: "#7f8c8d"
                Layout.alignment: Qt.AlignHCenter
                wrapMode: Text.WordWrap
                horizontalAlignment: Text.AlignHCenter
            }

            Rectangle {
                height: 1
                Layout.fillWidth: true
                color: "#bdc3c7"
            }

            // License Status
            GroupBox {
                title: "License File Status"
                Layout.fillWidth: true

                ColumnLayout {
                    anchors.fill: parent
                    spacing: 10

                    RowLayout {
                        Text { text: "License File:"; font.bold: true }
                        Text {
                            text: "license.dat"
                            color: "#3498db"
                        }
                    }

                    RowLayout {
                        Text { text: "Location:"; font.bold: true }
                        Text {
                            text: "Application Directory"
                            color: "#7f8c8d"
                            font.pixelSize: 11
                        }
                    }
                }
            }

            // Serial Number Input
            GroupBox {
                title: "PC Serial Number"
                Layout.fillWidth: true

                ColumnLayout {
                    anchors.fill: parent
                    spacing: 10

                    TextField {
                        id: serialInput
                        Layout.fillWidth: true
                        placeholderText: "Enter your PC serial number"
                        text: licenseValidator.serialNumber
                    }

                    Button {
                        text: "Save Serial Number"
                        Layout.fillWidth: true
                        onClicked: {
                            if(serialInput.text.length > 0) {
                                licenseValidator.setSerialNumber(serialInput.text)
                                statusText.text = "✓ Serial number saved"
                                statusText.color = "green"
                                // Reload license
                                if(licenseValidator.loadAndValidateLicense()) {
                                    statusText.text = "✓ License validated successfully!"
                                } else {
                                    statusText.text = "✗ " + licenseValidator.message
                                    statusText.color = "red"
                                }
                            } else {
                                statusText.text = "✗ Please enter serial number"
                                statusText.color = "red"
                            }
                        }
                    }
                }
            }

            Text {
                id: statusText
                Layout.fillWidth: true
                wrapMode: Text.WordWrap
                font.pixelSize: 11
                color: "#e74c3c"
                text: licenseValidator.message
            }

            Rectangle {
                height: 1
                Layout.fillWidth: true
                color: "#bdc3c7"
            }

            Text {
                text: "Note: Place the license.dat file in the same folder as the application"
                color: "#7f8c8d"
                font.pixelSize: 10
                font.italic: true
                wrapMode: Text.WordWrap
                Layout.alignment: Qt.AlignHCenter
                horizontalAlignment: Text.AlignHCenter
            }
        }
    }

    // License Information Dialog
    Dialog {
        id: licenseInfoDialog
        title: "License Information"
        modal: true
        standardButtons: Dialog.Ok
        width: 450
        height: 350
        x: (mainWindow.width - width) / 2
        y: (mainWindow.height - height) / 2

        ColumnLayout {
            anchors.fill: parent
            anchors.margins: 15
            spacing: 15

            GroupBox {
                title: "License Details"
                Layout.fillWidth: true

                ColumnLayout {
                    anchors.fill: parent
                    spacing: 10

                    RowLayout {
                        Text { text: "Serial Number:"; font.bold: true; Layout.preferredWidth: 120 }
                        Text { text: licenseValidator.serialNumber }
                    }

                    RowLayout {
                        Text { text: "Status:"; font.bold: true; Layout.preferredWidth: 120 }
                        Text {
                            text: licenseValidator.isValid ? "Active" : "Inactive"
                            color: licenseValidator.isValid ? "#27ae60" : "#e74c3c"
                            font.bold: true
                        }
                    }

                    RowLayout {
                        Text { text: "Days Remaining:"; font.bold: true; Layout.preferredWidth: 120 }
                        Text {
                            text: licenseValidator.daysRemaining
                            color: licenseValidator.daysRemaining < 30 ? "#e67e22" : "#27ae60"
                        }
                    }
                }
            }

            GroupBox {
                title: "File Information"
                Layout.fillWidth: true

                ColumnLayout {
                    anchors.fill: parent
                    spacing: 10

                    RowLayout {
                        Text { text: "License File:"; font.bold: true; Layout.preferredWidth: 120 }
                        Text { text: "license.dat" }
                    }

                    RowLayout {
                        Text { text: "Location:"; font.bold: true; Layout.preferredWidth: 120 }
                        Text {
                            text: "Application Directory"
                            font.pixelSize: 11
                        }
                    }
                }
            }

            Text {
                text: licenseValidator.message
                wrapMode: Text.WordWrap
                Layout.fillWidth: true
                color: "#7f8c8d"
                font.pixelSize: 11
            }

            Text {
                text: "For support: support@yourcompany.com"
                color: "#3498db"
                font.pixelSize: 10
                Layout.alignment: Qt.AlignHCenter
            }
        }
    }
}
