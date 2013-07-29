import QtQuick 1.1

Rectangle {
	id: button
	property alias label: text.text
	signal clicked
	width: text.width
	height: text.height
	Style { id: style}
	color: style.backgroundColor2
	radius: 3

	Text {
		id: text
		color: style.textColor
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


