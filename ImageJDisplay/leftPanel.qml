import QtQuick 2.14
import QtQuick.Controls 2.14
import QtQuick.Layouts 1.14

Rectangle {
    id: root

    color: "#111827"
    border.color: "#334155"
    border.width: 1
    radius: 6

    // =========================================================
    // PUBLIC PROPERTIES
    // =========================================================

    property string patientName: "John Doe"
    property string patientId: "PID123456"
    property string patientDob: "1980-05-15"
    property string patientGender: "M"
    property string modality: "CT"
    property string studyDate: "2024-05-20"

    property int selectedStudy: 0
    property int selectedSeries: 1

    // =========================================================
    // STUDY MODEL
    // =========================================================

    ListModel {
        id: studyModel

       /* ListElement {
            studyName: "CT Brain WO Contrast"
            studyDateText: "2024-05-20"
            imageCount: "120 Images"
        }

        ListElement {
            studyName: "MR Spine Lumbar"
            studyDateText: "2024-05-18"
            imageCount: "68 Images"
        }

        ListElement {
            studyName: "CT Chest W Contrast"
            studyDateText: "2024-05-15"
            imageCount: "320 Images"
        }

        ListElement {
            studyName: "MR Knee Right"
            studyDateText: "2024-05-10"
            imageCount: "45 Images"
        }*/
    }

    // =========================================================
    // SERIES MODEL
    // =========================================================

    ListModel {
        id: seriesModel

       /* ListElement {
            seriesName: "1 Topogram"
            imageCount: "1 Image"
        }

        ListElement {
            seriesName: "2 Brain Axial"
            imageCount: "120 Images"
        }

        ListElement {
            seriesName: "3 Brain Sagittal"
            imageCount: "120 Images"
        }

        ListElement {
            seriesName: "4 Brain Coronal"
            imageCount: "120 Images"
        }*/
    }

    // =========================================================
    // MAIN CONTENT
    // =========================================================

    ColumnLayout {
        anchors.fill: parent

        anchors.margins: 8

        spacing: 10

        // =====================================================
        // STUDY TITLE
        // =====================================================

        Text {
            text: "STUDY"

            color: "#AEBFD2"

            font.pixelSize: 13
            font.bold: true

            Layout.fillWidth: true
        }

        // =====================================================
        // SEARCH
        // =====================================================

        TextField {
            id: searchField

            Layout.fillWidth: true
            Layout.preferredHeight: 42

            placeholderText: "Search Studies..."

            color: "#E2E8F0"

            placeholderTextColor: "#7F91A5"

            font.pixelSize: 13

            background: Rectangle {

                radius: 5

                color: "#1E293B"

                border.color: searchField.activeFocus
                              ? "#3B82F6"
                              : "#334155"

                border.width: 1
            }
        }

        // =====================================================
        // STUDY LIST
        // =====================================================

        ListView {
            id: studyList

            Layout.fillWidth: true
            Layout.preferredHeight: 280

            clip: true

            spacing: 6

            interactive: false

            model: studyModel

            delegate: Rectangle {

                width: studyList.width

                height: 65

                radius: 5

                color: index === root.selectedStudy
                       ? "#3A5C88"
                       : "#1A2737"

                border.color: index === root.selectedStudy
                              ? "#4B73A8"
                              : "#334155"

                border.width: 1

                ColumnLayout {

                    anchors.fill: parent

                    anchors.leftMargin: 12
                    anchors.rightMargin: 12

                    anchors.topMargin: 10
                    anchors.bottomMargin: 10

                    spacing: 5

                    Text {

                        Layout.fillWidth: true

                        text: studyName

                        color: "#F1F5F9"

                        font.pixelSize: 14
                        font.bold: false

                        elide: Text.ElideRight
                    }

                    RowLayout {

                        Layout.fillWidth: true

                        spacing: 5

                        Text {

                            text: studyDateText

                            color: "#A9B8C9"

                            font.pixelSize: 12
                        }

                        Item {
                            Layout.fillWidth: true
                        }

                        Text {

                            text: imageCount

                            color: "#B7C7D9"

                            font.pixelSize: 12
                        }
                    }
                }

                MouseArea {

                    anchors.fill: parent

                    cursorShape: Qt.PointingHandCursor

                    onClicked: {

                        root.selectedStudy = index

                        console.log(
                                    "Selected study:",
                                    studyName)
                    }
                }
            }
        }

        // =====================================================
        // DIVIDER
        // =====================================================

        Rectangle {

            Layout.fillWidth: true
            Layout.preferredHeight: 1

            color: "#334155"
        }

        // =====================================================
        // SERIES TITLE
        // =====================================================

        Text {

            text: "SERIES"

            color: "#AEBFD2"

            font.pixelSize: 13
            font.bold: true

            Layout.fillWidth: true
        }

        // =====================================================
        // SERIES LIST
        // =====================================================

        ListView {

            id: seriesList

            Layout.fillWidth: true
            Layout.preferredHeight: 176

            clip: true

            spacing: 4

            interactive: false

            model: seriesModel

            delegate: Rectangle {

                width: seriesList.width

                height: 40

                radius: 4

                color: index === root.selectedSeries
                       ? "#42689C"
                       : "#182536"

                RowLayout {

                    anchors.fill: parent

                    anchors.leftMargin: 12
                    anchors.rightMargin: 12

                    spacing: 8

                    Text {

                        Layout.fillWidth: true

                        text: seriesName

                        color: "#E2E8F0"

                        font.pixelSize: 13

                        elide: Text.ElideRight
                    }

                    Text {

                        text: imageCount

                        color: "#B4C3D5"

                        font.pixelSize: 12
                    }
                }

                MouseArea {

                    anchors.fill: parent

                    cursorShape: Qt.PointingHandCursor

                    onClicked: {

                        root.selectedSeries = index

                        console.log(
                                    "Selected series:",
                                    seriesName)
                    }
                }
            }
        }

        // =====================================================
        // DIVIDER
        // =====================================================

        Rectangle {

            Layout.fillWidth: true
            Layout.preferredHeight: 1

            color: "#334155"
        }

        // =====================================================
        // PATIENT INFO TITLE
        // =====================================================

        Text {

            text: "PATIENT INFO"

            color: "#AEBFD2"

            font.pixelSize: 13
            font.bold: true

            Layout.fillWidth: true
        }

        // =====================================================
        // PATIENT INFORMATION
        // =====================================================

        GridLayout {

            Layout.fillWidth: true
            Layout.fillHeight: true

            columns: 2

            columnSpacing: 10
            rowSpacing: 10

            Text {

                text: "Patient Name"

                color: "#8FA1B5"

                font.pixelSize: 12
            }

            Text {

                Layout.fillWidth: true

                text: dicomLoader.patientName

                color: "#E2E8F0"

                font.pixelSize: 12

                elide: Text.ElideRight
            }

            Text {

                text: "Patient ID"

                color: "#8FA1B5"

                font.pixelSize: 12
            }

            Text {

                Layout.fillWidth: true

                text: dicomLoader.patientId

                color: "#E2E8F0"

                font.pixelSize: 12

                elide: Text.ElideRight
            }

            Text {

                text: "DoB / Gender"

                color: "#8FA1B5"

                font.pixelSize: 12
            }

            Text {

                Layout.fillWidth: true

                text: dicomLoader.patientBirthDate
                      + "  /  "
                      + dicomLoader.patientSex

                color: "#E2E8F0"

                font.pixelSize: 12

                elide: Text.ElideRight
            }

            Text {

                text: "Modality"

                color: "#8FA1B5"

                font.pixelSize: 12
            }

            Text {

                Layout.fillWidth: true

                text: dicomLoader.modality

                color: "#E2E8F0"

                font.pixelSize: 12

                elide: Text.ElideRight
            }

            Text {

                text: "Study Date"

                color: "#8FA1B5"

                font.pixelSize: 12
            }

            Text {

                Layout.fillWidth: true

                text: dicomLoader.studyDate

                color: "#E2E8F0"

                font.pixelSize: 12

                elide: Text.ElideRight
            }
        }

  }
}
