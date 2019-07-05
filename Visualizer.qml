import QtQuick 2.7

Rectangle {
    id: root

    width: childrenRect.width
    height: childrenRect.height

    property Item rootNode: null

    //color: Qt.rgba(0.3, 0.5, 0.7, 1.0)

    function clear(){
        if(rootNode !== null){
            rootNode.destroy();
            rootNode = null;
        }
    }
}
