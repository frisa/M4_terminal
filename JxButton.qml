import QtQuick 2.0

Rectangle {
    width: buttonText.width + 10
    height: 30
    color: "#61676a"

    signal clicked

    property alias text: buttonText.text
    property alias textColor: buttonText.color

    Text{
        id: buttonText
        anchors.centerIn: parent
        text: "jxButton"
        color: "white"
    }

    MouseArea{
        id: mouseArea
        anchors.fill: parent
        onClicked: {
            parent.clicked()
            console.log("clicked: " + buttonText.text)
        }
    }
}
