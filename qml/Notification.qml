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
		width: parent.height
		fillMode: Image.PreserveAspectFit

		anchors.topMargin: 5
		anchors.top: parent.top
		anchors.left: parent.left
		anchors.bottom: parent.bottom
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
		anchors.bottomMargin: 4
	}
	Text {
		id: bodyText
		text: body
		textFormat: Text.PlainText
		color: style.textColor
		elide: Text.ElideRight
		wrapMode: Text.Wrap
		verticalAlignment: Text.AlignVCenter
		maximumLineCount: 2
		anchors.left: iconImage.right
		anchors.right: parent.right
		anchors.bottom: parent.bottom
		anchors.top: summaryText.bottom
	}

	MouseArea {
		anchors.fill: parent
		onClicked: {
			notificationModel.CloseNotification(uid)
			}
	}
	} // Container

}
