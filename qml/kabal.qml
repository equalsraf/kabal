import QtQuick 2.3

Rectangle {
	property int notificationHeight: 84

	id: root
	Style { id: style}
	width: 380
	height: notificationHeight + 16

	border.color: style.borderColor
	border.width: 2
	color: style.backgroundColor2

	Connections {
		target: notificationModel
		onNotificationCountChanged: {
			if ( count === 0 ) {
				// Nothing to show
				Window.hide();
			} else if (Window.visible === false) {
				view.positionViewAtIndex(view.count-1, ListView.Contain);
				Window.show();
			} else if (mouse.containsMouse === false) {
				// Animate notification switch
				view.currentIndex = view.count-1;
			}
		}
		onNotificationsToggled: {
			// On toggle we want to quickly jump to the last position
			view.positionViewAtIndex(view.count-1, ListView.Contain);
		}
	}

	Connections {
		target: Window
		onMouseWheelUp: view.incrementCurrentIndex()
		onMouseWheelDown: view.decrementCurrentIndex()
	}

	MouseArea {
		id: mouse
		hoverEnabled: true
		anchors.fill: parent
	}

	ListView {
		id: view
		anchors.top: parent.top
		anchors.left: parent.left
		anchors.right: parent.right
		height: notificationHeight
		interactive: false

		orientation: ListView.Horizontal
		model: notificationModel
		snapMode: ListView.SnapToItem
	        delegate: Notification { height: notificationHeight; width: root.width}
	}

	// statusbar
	Rectangle {
		id: statusbar
		anchors.top: view.bottom
		anchors.left: parent.left
		anchors.right: parent.right
		anchors.bottom: parent.bottom
		anchors.leftMargin: 5

		color: style.backgroundColor2

		Text {
			id: statusCount
			color: style.textColor
			text: view.currentIndex + 1
			anchors.top: parent.top
			anchors.left: parent.left
			anchors.bottom: parent.bottom
			anchors.leftMargin: 5
		}
		Text {
			id: statusCountSplit
			color: style.textColor
			anchors.top: parent.top
			anchors.left: statusCount.right
			anchors.bottom: parent.bottom
			text:'/' 
		}
		Text {
			id: statusCountTotal
			color: style.textColor
			anchors.top: parent.top
			anchors.left: statusCountSplit.right
			anchors.bottom: parent.bottom
			text: view.count
		}
		
		ImageButton { // disable notification
			id: buttonToggleNotifications
			checkable: true
			source: "qrc:///icons/user-online.png"
			sourceChecked: "qrc:///icons/user-busy.png"
			anchors.top: parent.top
			anchors.left: statusCountTotal.right
			anchors.bottom: parent.bottom
			onClicked: { 
				notificationModel.notificationsDisabled = !notificationModel.notificationsDisabled
			}
		}
	}

}

