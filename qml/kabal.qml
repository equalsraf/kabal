import QtQuick 1.1

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
				Window.show();
			} else if (mouse.containsMouse === false) {
				view.currentIndex = view.count-1;
			}
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

		ImageButton {
			source: 'qrc:///icons/go-previous-view.png'
			id:buttonPrev
			width: 16
			anchors.top: parent.top
			anchors.left: parent.left
			anchors.bottom: parent.bottom
			onClicked: view.decrementCurrentIndex()
		}

		Text {
			id: statusCount
			color: style.textColor
			text: view.currentIndex + 1
			anchors.top: parent.top
			anchors.left: buttonPrev.right
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
		ImageButton {
			id:buttonNext
			source: 'qrc:///icons/go-next-view.png'
			anchors.leftMargin: 5
			anchors.top: parent.top
			anchors.left: statusCountTotal.right
			anchors.bottom: parent.bottom
			width: 16
			onClicked: view.incrementCurrentIndex()
		}
		
		ImageButton { // disable notification
			id: buttonToggleNotifications
			checkable: true
			source: "qrc:///icons/user-online.png"
			sourceChecked: "qrc:///icons/user-busy.png"
			anchors.top: parent.top
			anchors.left: buttonNext.right
			anchors.bottom: parent.bottom
			onClicked: { 
				notificationModel.notificationsDisabled = !notificationModel.notificationsDisabled
			}
		}
	}

}

