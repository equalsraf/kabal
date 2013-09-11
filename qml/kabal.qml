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

	function updateWidget() {
		var count = notificationModel.notificationCount;

		if ( count === 0 ) {
			// Nothing to show
			Window.hide();
		} else if (Window.visible === false) {
			Window.show();
		} else if (mouse.containsMouse === false) {
			view.currentIndex = view.count-1;
		}
	}

	Connections {
		target: notificationModel
		onRowsRemoved: {
			updateWidget();
		}
		onRowsInserted: {
			updateWidget();
		}
		onNotificationsToggled: {
			updateWidget();
		}
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
			source: 'qrc:///icons/arrow_r.png'

			id:buttonPrev
			mirror: true
			width: 8
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
			source: 'qrc:///icons/arrow_r.png'
			anchors.leftMargin: 5
			anchors.top: parent.top
			anchors.left: statusCountTotal.right
			anchors.bottom: parent.bottom
			width: 8
			onClicked: view.incrementCurrentIndex()
		}
	}

}

