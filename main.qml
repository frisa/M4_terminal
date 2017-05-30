import QtQuick 2.6
import QtQuick.Controls 2.1
import QtQuick.Window 2.2
import QtQuick.Layouts 1.1

ApplicationWindow {
    id: applicationWindow
    visible: true
    width: 640
    height: 480
    title: qsTr("M4 Terminal")
    signal solutionClicked()
    header: ToolBar{
        RowLayout{
            layoutDirection: "LeftToRight"
            id: rawLayout00
        Button{

            id: buttonSolution
            text: qsTr("Solution")
            MouseArea{
                id:maSolution
                anchors.fill: parent
                onClicked: applicationWindow.solutionClicked()
            }
        }
        Button{
            id: buttonLoad
            text: qsTr("Load")
        }
        }

    }

    footer: TabBar{

    }

    StackView {
        anchors.fill: parent
    }
}
