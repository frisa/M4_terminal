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
    signal loadElfClicked()

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
                    text: "Load ELF file"
                    anchors.fill: parent
                    onClicked: {
                        mainStack.push(viewElf)
                        console.log("show load the elf file")
                    }
                }
            }
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
                    text: "Ethernet"
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
                    text: "Help"
                    anchors.fill: parent
                    onClicked: {
                        mainStack.push(viewUnload)
                        console.log("show help")
                    }
                }
            }
            Rectangle{
                width: 80
                height: 29
                JxButton
                {
                    text: "Quit"
                    anchors.fill: parent
                    onClicked: {
                        mainStack.push(viewUnload)
                        console.log("show quit")
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
        initialItem: viewElf
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
                    ComboBox {
                            textRole: "display"
                            width: 200
                            model: comModel
                        }
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
                    height: mainStack.height
                    width:  applicationWindow.width

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
        id: viewElf
        Rectangle{
            color: "#49575f"
            ColumnLayout{
                RowLayout{
                    Text{text: "ELF File"; color: "white"}
                    ComboBox{model: elfModel; displayText: name}
                    Button{text: "Browse"}
                    Button{text: "Load"
                    onClicked: applicationWindow.loadElfClicked()
                    }
                }
                Rectangle
                {
                    width: parent.width
                    height: 300
                    ListView{
                        width: parent.width
                        height: 300
                        model: elfModel
                        delegate: Component{
                            Rectangle{
                                Text{
                                    color: "white"
                                    text: "|" + name + "|" + abi + "|" + sclass + "|" + dformat
                                }
                            }
                        }
                }
               }
            }
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
