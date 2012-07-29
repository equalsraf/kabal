import Qt 4.7
import QtQuick 1.1

Rectangle {
	id: notification
	property string summary : ""
	property string body : ""
	property string icon : ""
	property bool animateUpdates : true

	//visible: summary || icon || body

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
		width: parent.height
		fillMode: Image.PreserveAspectFit

		anchors.topMargin: 5
		anchors.top: parent.top
		anchors.left: parent.left
		anchors.bottom: parent.bottom
	}

	Text {
		id: summaryText
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

	} // Container

	SequentialAnimation {
		id: animation
		NumberAnimation { target: notification; properties: "opacity"; from: 1; to : 0 }
		ScriptAction { script: {
				summaryText.text = notification.summary;
				bodyText.text = notification.body;
				iconImage.source = notification.icon;
			}}
		NumberAnimation { target: notification; properties: "opacity"; from: 0; to : 1 }
	}

	function updateNotification(summary, body, icon) {
		notification.summary = summary;
		notification.icon = icon;
		notification.body = body;

		if ( notification.animateUpdates ) {
			animation.restart();
		} else {
			summaryText.text = notification.summary;
			bodyText.text = notification.body;
			iconImage.source = notification.icon;
		}
	}
}
