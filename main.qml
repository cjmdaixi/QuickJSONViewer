import QtQuick 2.7
import QtQuick.Window 2.3
import QtQuick.Controls 2.3

Window {
    visible: true
    width: 1000
    height: 600
    title: qsTr("JSON Visualizer")

    Rectangle{
        id: jsonEditorFrame
        anchors{
            left: parent.left
            top: parent.top
            bottom: parent.bottom
            margins: 20
        }
        width: 300
        border.color: "black"
        TextArea{
            id: jsonEditor
            anchors.fill: parent
            text: $hub.demoJson
            selectByMouse: true
        }
    }

    Button{
        id: visualizeButton
        text: qsTr("Visualize")
        anchors{
            left: jsonEditorFrame.right
            verticalCenter: parent.verticalCenter
            margins: 10
        }
        onClicked: {
            var jsonObj;
            try {
                jsonObj = JSON.parse(jsonEditor.text);
            }catch(e){
                console.log(e.name, e.message);
                return;
            }
            if(visualizer.rootNode !== null){
                $hub.destroyNode(visualizer.rootNode);
            }

            visualizer.rootNode = $hub.visualize(visualizer, jsonObj);
        }
    }

    Rectangle{
        id: visualizeFrame
        anchors{
            left: visualizeButton.right
            top: parent.top
            bottom: parent.bottom
            right: parent.right
            margins: 20
        }
        border.color: "black"
        //Item{
        ScrollView{
            id: visualizeScrollView
            anchors.centerIn: parent
            clip: true
            width: Math.min(parent.width, visualizer.width)
            height: Math.min(parent.height, visualizer.height)
            contentWidth: visualizer.width; contentHeight: visualizer.height
            //            ScrollBar.horizontal.policy: ScrollBar.AlwaysOn
            //                  ScrollBar.vertical.policy: ScrollBar.AlwaysOn
            Visualizer{
                id: visualizer
                anchors.centerIn: parent
            }
        }
    }
}
