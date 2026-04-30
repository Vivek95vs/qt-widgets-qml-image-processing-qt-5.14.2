// SecondaryWindow.qml
import QtQuick 2.12
import QtQuick.Window 2.12
import QtQuick.Controls 2.12
import QtQuick.Layouts 1.12

Window {
    id: secondaryWindow
    width: 800
    height: 600
    title: "TCP Control System - Secondary"
    color: "#f0f0f0"
    flags: Qt.Window

    // Secondary monitor content (simplified version of the primary)
    ColumnLayout {
        anchors.fill: parent
        anchors.margins: 20

        // Header section
        RowLayout {
            Layout.fillWidth: true

            Text {
                text: "TCP"
                font.bold: true
                font.pixelSize: 24
                color: "#333"
            }

            Item { Layout.fillWidth: true }

            ColumnLayout {
                Text {
                    text: "C-3"
                    font.bold: true
                    font.pixelSize: 16
                    color: "#333"
                }

                Text {
                    text: "PANAGEO"
                    font.bold: true
                    font.pixelSize: 16
                    color: "#333"
                }

                Text {
                    text: "Engineering functions"
                    font.pixelSize: 12
                    color: "#666"
                }
            }
        }

        Rectangle {
            Layout.fillWidth: true
            height: 2
            color: "#ccc"
            Layout.topMargin: 10
            Layout.bottomMargin: 10
        }

        // Main content area for secondary display
        GridLayout {
            Layout.fillWidth: true
            Layout.fillHeight: true
            columns: 2
            columnSpacing: 20
            rowSpacing: 20

            // Left side - C-ARM login (same as primary)
            Rectangle {
                Layout.fillWidth: true
                Layout.fillHeight: true
                color: "white"
                border.color: "#ddd"
                radius: 5

                ColumnLayout {
                    anchors.fill: parent
                    anchors.margins: 15

                    Text {
                        text: "C-ARM"
                        font.bold: true
                        font.pixelSize: 18
                        color: "#333"
                        Layout.alignment: Qt.AlignHCenter
                    }

                    Text {
                        text: "SIGN IN"
                        font.bold: true
                        font.pixelSize: 14
                        color: "#666"
                        Layout.alignment: Qt.AlignHCenter
                        Layout.topMargin: 10
                    }

                    ColumnLayout {
                        Layout.fillWidth: true
                        Layout.topMargin: 20

                        Label {
                            text: "Start ID"
                            font.bold: true
                            color: "#333"
                        }

                        TextField {
                            id: secondaryStartId
                            Layout.fillWidth: true
                            placeholderText: "Enter Start ID"
                        }

                        Label {
                            text: "Password"
                            font.bold: true
                            color: "#333"
                            Layout.topMargin: 10
                        }

                        TextField {
                            id: secondaryPassword
                            Layout.fillWidth: true
                            placeholderText: "Enter Password"
                            echoMode: TextInput.Password
                        }
                    }

                    RowLayout {
                        Layout.fillWidth: true
                        Layout.topMargin: 20

                        Button {
                            text: "Login"
                            Layout.preferredWidth: 100
                            onClicked: {
                                // Handle login logic for secondary display
                                console.log("Secondary Login clicked")
                            }
                        }

                        Button {
                            text: "GEN"
                            Layout.preferredWidth: 80
                        }
                    }

                    // Software Packet section
                    Rectangle {
                        Layout.fillWidth: true
                        Layout.topMargin: 20
                        height: 80
                        color: "#f9f9f9"
                        border.color: "#eee"

                        ScrollView {
                            anchors.fill: parent
                            anchors.margins: 5

                            Text {
                                text: "This section contains proprietary components and is available in public databases. Available in distributed databases are available at the http://github.com/anonymous-software-packet-and-data-paradise/and/or software-packets-without-software-to-be"
                                wrapMode: Text.Wrap
                                font.pixelSize: 10
                                color: "#666"
                            }
                        }
                    }

                    Text {
                        text: "Managed Management"
                        font.bold: true
                        font.pixelSize: 12
                        color: "#333"
                        Layout.alignment: Qt.AlignHCenter
                        Layout.topMargin: 10
                    }

                    // Bottom buttons
                    RowLayout {
                        Layout.fillWidth: true
                        Layout.topMargin: 20

                        Button {
                            text: "Help"
                            Layout.preferredWidth: 80
                        }

                        Item { Layout.fillWidth: true }

                        Button {
                            text: "SPE"
                            Layout.preferredWidth: 80
                        }
                    }
                }
            }

            // Right side - Additional controls for secondary display
            Rectangle {
                Layout.fillWidth: true
                Layout.fillHeight: true
                color: "white"
                border.color: "#ddd"
                radius: 5

                ColumnLayout {
                    anchors.fill: parent
                    anchors.margins: 15

                    Text {
                        text: "System Status"
                        font.bold: true
                        font.pixelSize: 18
                        color: "#333"
                        Layout.alignment: Qt.AlignHCenter
                    }

                    // Status indicators
                    ColumnLayout {
                        Layout.fillWidth: true
                        Layout.topMargin: 20
                        spacing: 10

                        StatusIndicator {
                            label: "Network Connection"
                            status: true
                        }

                        StatusIndicator {
                            label: "Database Access"
                            status: true
                        }

                        StatusIndicator {
                            label: "Hardware Interface"
                            status: false
                        }

                        StatusIndicator {
                            label: "Security Protocol"
                            status: true
                        }
                    }

                    Item { Layout.fillHeight: true }

                    // Control buttons
                    RowLayout {
                        Layout.fillWidth: true

                        Button {
                            text: "System Check"
                            Layout.fillWidth: true
                        }

                        Button {
                            text: "Diagnostics"
                            Layout.fillWidth: true
                        }
                    }

                    RowLayout {
                        Layout.fillWidth: true

                        Button {
                            text: "Restart Services"
                            Layout.fillWidth: true
                        }

                        Button {
                            text: "Emergency Stop"
                            Layout.fillWidth: true
                            background: Rectangle {
                                color: "#d9534f"
                                radius: 3
                            }
                        }
                    }
                }
            }
        }
    }

    // Component for status indicators
    Component {
        id: statusIndicatorComponent

        RowLayout {
            property string label: ""
            property bool status: false

            Layout.fillWidth: true

            Text {
                text: label
                font.pixelSize: 12
                color: "#333"
                Layout.fillWidth: true
            }

            Rectangle {
                width: 12
                height: 12
                radius: 6
                color: parent.status ? "#5cb85c" : "#d9534f"
            }
        }
    }
}
