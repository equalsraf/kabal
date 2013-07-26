import QtQuick 1.1

Rectangle {
	property int notificationHeight: 64

	id: root
	Style { id: style}
	width: 380
	height: 64

	border.color: style.borderColor
	border.width: 2
	color: style.backgroundColor2

	function updateWidget() {
		var count = notificationModel.notificationCount;

		if ( count === 0 ) {
			// Nothing to show
			Window.hide();
		} else {
			Window.show();
			view.positionViewAtEnd();
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
	}

	ListView {
		id: view
		anchors.top: parent.top
		anchors.left: parent.left
		anchors.right: parent.right
		height: 64

		orientation: ListView.Horizontal
		model: notificationModel
		snapMode: ListView.SnapToItem
	        delegate: Notification { height: 64; width: root.width}
	}

}

