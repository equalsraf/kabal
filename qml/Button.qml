import QtQuick 1.1

Rectangle {
	property string text : ""

	id: button
	Style {id: style}

	color: 'transparent'

	Rectangle {
		anchors.fill: parent
		anchors.margins: 4
		color: style.backgroundColor2
		radius: 20
		smooth: true
		Text {
			anchors.fill: parent
			text: button.text
			color: style.textColor
			font.bold: true
			font.pointSize: 8
			horizontalAlignment: Text.AlignHCenter
			verticalAlignment: Text.AlignVCenter
		}

		MouseArea {
			hoverEnabled: true
			anchors.fill: parent
			onEntered: parent.color = style.highlight
			onExited: parent.color = style.backgroundColor2
		}

	}
}


