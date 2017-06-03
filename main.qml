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

    signal loadClicked()

    background: Rectangle{
        gradient: Gradient{
            GradientStop {position: 0; color: "#6991a3"}
            GradientStop {position: 1; color: "#5c7884"}
        }
    }

    header: Rectangle
    {
        height: 30
        color: "#263b44"
        RowLayout
        {
            spacing: 1
            Rectangle{
                width: 80
                height: 29
                JxButton
                {
                    text: "Serial Port"
                    anchors.fill: parent
                    onClicked: {
                        mainStack.push(viewSerial)
                        console.log("show serial port view")
                        onClicked: applicationWindow.loadClicked()
                    }
                }
            }
            Rectangle{
                width: 80
                height: 29
                JxButton
                {
                    text: "Load"
                    anchors.fill: parent
                    onClicked: {
                        mainStack.push(viewLoad)
                        //onClicked: applicationWindow.solutionClicked()
                        console.log("show serial port view")
                    }
                }
            }
            Rectangle{
                width: 80
                height: 29
                JxButton
                {
                    text: "Unload"
                    anchors.fill: parent
                    onClicked: {
                        mainStack.push(viewUnload)
                        console.log("show serial port view")
                    }
                }
            }
        }
    }
    footer: Rectangle{
            height: 30
            color: "#bccad1"
        }

    StackView {
        id: mainStack
        initialItem: viewSerial
        anchors.fill: parent
    }

    ListModel{
        id: logModel
        ListElement{ name: "Tiva"}
        ListElement{ name: "noTiva"}
    }

    Component{
        id: viewSerial
        Rectangle{
            color: "#49575f"
            ColumnLayout{
                RowLayout{
                    spacing: 5
                    Text{text: "Serial Port"; color: "white"}
                    TextField{placeholderText: qsTr("Enter the serial port name"); height: 25}
                    JxButton
                    {
                        text: "Load Program"
                        onClicked: {
                            applicationWindow.loadClicked()
                            console.log("load the program")
                        }
                    }
                }
                Rectangle{
                    height: 250
                    width:  300

                    color: "white"
                    ListView{
                        id: slnView
                        model: slnModel
                        anchors.fill: parent
                        delegate: Component{
                            Rectangle{
                                width: slnView.width
                                implicitHeight: edit.implicitHeight + 2
                                color: "transparent"
                                border.color: "black"
                                border.width: 1
                                Text{
                                    id: edit
                                    anchors.margins: 1.5 * parent.border.width
                                    anchors.fill: parent
                                    text: model.display
                                }
                            }
                        }
                    }
                }
            }
        }
    }
    Component{
        id: viewLoad
        Rectangle{
            width: 100
            height: 80
            color: "blue"
        }
    }
    Component{
        id: viewUnload
        Rectangle{
            width: 100
            height: 80
            color: "red"
        }
    }
}
