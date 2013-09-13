import QtQuick 1.1

Rectangle {
	id: notification

	Style { id: style}
	color: 'transparent'

	Rectangle {

	id: container
	radius: 5
	anchors.margins: 5
	anchors.fill: parent
	color: style.backgroundColor

	Image {
		id: iconImage
        	source: icon
		width: 48
		height: 48
		fillMode: Image.PreserveAspectFit

		anchors.leftMargin: 4
		anchors.top: parent.top
		anchors.left: parent.left
	}

	Text {
		id: summaryText
		text: summary
		font.bold: true
		color: style.textColor
		elide: Text.ElideRight
		wrapMode: Text.Wrap

		anchors.left: iconImage.right
		anchors.right: parent.right
		anchors.top: parent.top
		anchors.topMargin: 4
		anchors.leftMargin: 8
		anchors.bottomMargin: 4
	}
	Text {
		id: bodyText
		text: body
		textFormat: Text.PlainText
		color: style.textColor
		elide: Text.ElideRight
		wrapMode: Text.Wrap
		maximumLineCount: 2
		anchors.leftMargin: 8
		anchors.left: iconImage.right
		anchors.right: parent.right
		anchors.bottom: actionbox.top
		anchors.top: summaryText.bottom
	}

	MouseArea {
		anchors.fill: parent
		onClicked: {
			var dflidx = actionkeys.indexOf('default')
			if (dflidx == -1 ) {
				notificationModel.CloseNotification(uid)
			} else {
				notificationModel.invokeAction(uid, actionkeys[dflidx])
			}
		}
	}

	Row {
		id: actionbox
		height: 16
		anchors.left: parent.left
		anchors.right: parent.right
		anchors.bottom: parent.bottom
		anchors.margins: 2
		spacing: 4

		Repeater {

		model: actionkeys
		Button { 
			label: actionnames[index]
			height: parent.height
			onClicked: notificationModel.invokeAction(uid, actionkeys[index])
		}
		}
	}


	} // Container

}
