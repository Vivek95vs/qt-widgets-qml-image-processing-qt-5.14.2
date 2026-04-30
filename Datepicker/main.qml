import QtQuick 2.12
import QtQuick.Window 2.12
import QtQuick.Controls 2.12
import QtQuick.Controls.Material 2.12
import Qt.labs.calendar 1.0

Window {
    visible: true
    width: 400
    height: 500
    title: "Patient DOB Selection"

    // Main properties
    property date patientDob: new Date(1990, 0, 1) // Default date
    property bool isValidDob: true

    // Background rectangle
    Rectangle {
        anchors.fill: parent
        color: "#f5f5f5"

        Column {
            anchors.centerIn: parent
            spacing: 25

            // Title
            Text {
                text: "Select Patient Date of Birth"
                font.pixelSize: 20
                font.bold: true
                color: Material.primary
            }

            // Selected date display
            Rectangle {
                width: 200
                height: 50
                color: "white"
                border.color: isValidDob ? Material.primary : "red"
                border.width: 2
                radius: 5

                Text {
                    anchors.centerIn: parent
                    text: Qt.formatDate(patientDob, "MMMM d, yyyy")
                    font.pixelSize: 16
                    color: Material.foreground
                }
            }

            // Open calendar button
            Button {
                text: "Choose Date"
                width: 200
                height: 50
                font.pixelSize: 16
                Material.background: Material.primary
                Material.foreground: "white"

                onClicked: calendarPopup.open()
            }

            // Validation message
            Text {
                visible: !isValidDob
                text: "Please select a valid date (not in future)"
                color: "red"
                font.pixelSize: 14
            }
        }

        // Calendar popup
        Popup {
            id: calendarPopup
            width: 320
            height: 370
            anchors.centerIn: parent
            modal: true
            closePolicy: Popup.CloseOnEscape | Popup.CloseOnPressOutside

            background: Rectangle {
                color: "white"
                border.color: Material.primary
                border.width: 2
                radius: 5
            }

            Column {
                anchors.fill: parent
                spacing: 10
                padding: 10

                // Month/year navigation
                Row {
                    width: parent.width
                    height: 40
                    spacing: 10

                    Button {
                        width: 40
                        text: "<"
                        onClicked: {
                            if (calendar.month > 0) {
                                calendar.month--
                            } else {
                                calendar.month = 11
                                calendar.year--
                            }
                        }
                    }

                    Label {
                        width: parent.width - 100
                        text: Qt.locale().monthName(calendar.month) + " " + calendar.year
                        font.bold: true
                        font.pixelSize: 18
                        horizontalAlignment: Text.AlignHCenter
                        verticalAlignment: Text.AlignVCenter
                    }

                    Button {
                        width: 40
                        text: ">"
                        onClicked: {
                            if (calendar.month < 11) {
                                calendar.month++
                            } else {
                                calendar.month = 0
                                calendar.year++
                            }
                        }
                    }
                }

                // Day names header
                Row {
                    width: parent.width
                    height: 30
                    spacing: 2

                    Repeater {
                        model: ["S", "M", "T", "W", "T", "F", "S"]
                        Label {
                            width: (parent.width - 12) / 7
                            height: parent.height
                            text: modelData
                            horizontalAlignment: Text.AlignHCenter
                            verticalAlignment: Text.AlignVCenter
                            font.bold: true
                        }
                    }
                }

                // Calendar grid
                MonthGrid {
                    id: calendar
                    width: parent.width
                    height: parent.height - 120
                    month: patientDob.getMonth()
                    year: patientDob.getFullYear()

                    delegate: Rectangle {
                        width: (calendar.width - 12) / 7
                        height: (calendar.height - 12) / 6
                        color: {
                            if (model.day === patientDob.getDate() &&
                                model.month === calendar.month &&
                                model.year === calendar.year) {
                                return Material.primary
                            }
                            return model.month === calendar.month ? "transparent" : Qt.lighter(Material.primary, 1.8)
                        }

                        Text {
                            anchors.centerIn: parent
                            text: model.day
                            color: {
                                if (model.day === patientDob.getDate() &&
                                    model.month === calendar.month &&
                                    model.year === calendar.year) {
                                    return "white"
                                }
                                return model.month === calendar.month ? "black" : Qt.darker(Material.primary, 1.5)
                            }
                            opacity: model.day !== undefined ? 1 : 0
                        }

                        MouseArea {
                            anchors.fill: parent
                            enabled: model.day !== undefined && model.month === calendar.month
                            onClicked: {
                                patientDob = new Date(model.year, model.month, model.day)
                                isValidDob = patientDob <= new Date()
                                calendarPopup.close()
                            }
                        }
                    }
                }

                // Today button
                Button {
                    width: parent.width
                    text: "Today"
                    onClicked: {
                        var today = new Date()
                        calendar.month = today.getMonth()
                        calendar.year = today.getFullYear()
                        patientDob = today
                        isValidDob = true
                        calendarPopup.close()
                    }
                }
            }
        }
    }
}
