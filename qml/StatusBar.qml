import QtQuick 1.1


Rectangle {
	id: statusbar
	property string text: ""
	Style {id: style}

	Row {
		Text {
			text: statusbar.text
			color: style.textColor
		}
		Rectangle {
			color: "red"
		}
	}

}


