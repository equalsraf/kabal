import QtQuick 2.3

Rectangle {
	id: button
	Style { id: style}

	property alias label: text.text
	signal clicked
	height: text.paintedHeight + 32
	width: text.paintedWidth + 32
	border.width: 1
	border.color: style.borderColor2
	color: style.backgroundColor

	Text {
		id: text
		color: style.textColor
		anchors.centerIn: button
	}

	MouseArea {
		id: mousearea
		hoverEnabled: true
		anchors.fill: parent
		onEntered: {
			button.color = style.highlight
		}
		onExited: {
			button.color = style.backgroundColor
		}
		onClicked: button.clicked()
	}
}


