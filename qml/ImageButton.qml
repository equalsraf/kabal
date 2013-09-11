import QtQuick 1.1

Rectangle {
	id: button
	property alias source: img.source
	property alias mirror: img.mirror
	signal clicked

	Image {
		id: img
		anchors.fill: parent;
		fillMode: Image.PreserveAspectFit
	}

	MouseArea {
		id: mousearea
		hoverEnabled: true
		anchors.fill: parent
		onEntered: {
			button.color = style.highlight
		}
		onExited: {
			button.color = style.backgroundColor2
		}
		onClicked: button.clicked()
	}
}


