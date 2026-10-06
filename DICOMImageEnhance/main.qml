import QtQuick 2.12
import QtQuick.Controls 2.12
import QtQuick.Dialogs 1.3
import com.bonesuppression 1.0

ApplicationWindow {
    visible: true
    width: 1024
    height: 768
    title: "Bone Suppression Tool"

    BoneSuppressionProcessor {
        id: processor
        onProcessingFinished: {
            statusText.text = "Processing completed!"
            progressBar.visible = false
        }
        onProcessingError: {
            statusText.text = "Error: " + error
            progressBar.visible = false
        }
    }

    Column {
        anchors.centerIn: parent
        spacing: 20

        Button {
            text: "Select Raw Image"
            onClicked: fileDialog.open()
        }

        Button {
            text: "Process Image"
            enabled: processor.hasImage
            onClicked: {
                statusText.text = "Processing..."
                progressBar.visible = true
                processor.process()
            }
        }

        Text {
            id: statusText
            text: "Ready"
        }

        ProgressBar {
            id: progressBar
            visible: false
            indeterminate: true
        }
    }

    FileDialog {
        id: fileDialog
        title: "Select 16-bit RAW Image"
        nameFilters: ["Raw files (*.raw)", "All files (*.*)"]
        onAccepted: {
            processor.loadImage(fileUrl.toString().replace("file://", ""))
            statusText.text = "Image loaded: " + fileUrl
        }
    }
}
