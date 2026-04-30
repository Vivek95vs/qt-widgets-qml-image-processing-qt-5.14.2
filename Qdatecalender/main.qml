import QtQuick 2.14
import QtQuick.Window 2.14
import QtQuick.Controls 2.14
import QtQuick.Controls 1.4 as OldControls

Window {
    width: 640
    height: 480
    visible: true
    title: "QML Calendar Example"

    OldControls.Calendar {
        id: myCalendar
        anchors.centerIn: parent

        // Set initial selected date to today
        selectedDate: new Date()

        onSelectedDateChanged: {
            console.log("Selected date:", selectedDate.toDateString())
        }
    }
}
