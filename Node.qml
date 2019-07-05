import QtQuick 2.7
import QtQuick.Layouts 1.3

Item{
    id: root
    implicitWidth: childrenRect.width
    implicitHeight: childrenRect.height
    property alias nodeRect: nodeContentRect
    property alias text: nodeText.text
    property alias childrenLayout: childrenLayout
    property string color
    RowLayout {
        id: rowLayout

        spacing: 50

        Rectangle{
            id: nodeContentRect
            implicitWidth: nodeText.contentWidth + 15
            implicitHeight: nodeText.contentHeight + 15
            color: root.color
            radius: 5
            Text{
                id: nodeText
                anchors.fill: parent
                horizontalAlignment: Text.AlignHCenter
                verticalAlignment: Text.AlignVCenter
                font{
                    family: "Arial"
                    pointSize: 15
                }
                color: "white"
            }
        }

        ColumnLayout{
            id: childrenLayout
            spacing: 10
            onHeightChanged: lineCanvas.requestPaint();
            onWidthChanged: lineCanvas.requestPaint();
        }
    }
    Canvas{
        id: lineCanvas
        anchors.fill: parent
        onPaint: {
            var ctx = getContext("2d");
            ctx.reset();
            var pt1 = mapFromItem(nodeContentRect, nodeContentRect.width, nodeContentRect.height / 2);
            for(var i = 0; i !== childrenLayout.children.length; ++i){
                var item = childrenLayout.children[i];
                var pt2 = mapFromItem(item.nodeRect, 0, item.nodeRect.height / 2);
                ctx.moveTo(pt1.x, pt1.y);
                //ctx.lineTo(pt2.x, pt2.y);
                ctx.bezierCurveTo(pt1.x + 20, pt1.y, pt2.x - 20, pt2.y, pt2.x, pt2.y);
            }
            ctx.strokeStyle = "#969f95";
            ctx.lineWidth = 2;
            ctx.stroke();
        }
    }
}


