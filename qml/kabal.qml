import QtQuick 1.1
import Qt 4.7

Rectangle {
	property int notificationHeight: 64

	id: root
	Style { id: style}
	width: 360
	// We hold one or two notifications

	border.color: style.borderColor
	border.width: 2
	color: style.backgroundColor2

	Column {
		Notification {
			id: notification
			visible: false
			height: root.notificationHeight
			width: root.width
		}
		
		Notification {
			id: critical
			visible: false
			height: root.notificationHeight
			width: root.width
		}
		StatusBar {
			height: 16
		}
	} // Column


	function updateNotification(obj, summary, body, icon) {
		if ( Window.visible ) {
			obj.animateUpdates = true
		} else {
			obj.animateUpdates = false
		}
		obj.updateNotification(summary, body, icon);
	}

	Connections {
		target: notificationModel
		onLatestNotificationChanged: {
			updateNotification(notification, summary, body, icon);
			if ( state === '' ) {
				state = 'notification';
			} else if ( state == 'critical' ) {
				state = 'both'
			}
		}

		onCloseNotification: {
			if ( state === 'notification' ) {
				state = '';
			} else if (state === 'both') {
				state = 'critical';
			}
		}

		onLatestCriticalChanged: {
			updateNotification(critical, summary, body, icon);
			if ( state === '' ) {
				state = 'critical';
			} else if ( state == 'notification' ) {
				state = 'both'
			}
		}

		onCloseCritical: {
			if ( state === 'critical' ) {
				state = '';
			} else if (state === 'both') {
				state = 'notification';
			}
		}
	}


	MouseArea {
		anchors.fill: parent
		onClicked: root.state = ''
	}
	
	
	/*
	Button {
		id: count
		visible: (notificationModel.count > 1)
		text: notificationModel.count - 1
		width: parent.height
		anchors.top: parent.top
		anchors.bottom: parent.bottom
		anchors.right: parent.right
	}*/
	

	states: [
		State { name: ""
			PropertyChanges {target: Window; visible: false}
		},
		State { name: "visible"
			PropertyChanges {target: Window; visible: 'visible'}
		},
		State { 
			name: "single"
			extend: 'visible'
			PropertyChanges {target: root; height: root.notificationHeight}
			},
		State { name: "notification"; extend: 'single'
			PropertyChanges {target: notification; visible: true}
			},
		State { 
			name: "critical" 
			extend: 'single'
			PropertyChanges {target: critical; visible: true}
			},
		State { 
			name: "both" 
			extend: 'visible'
			PropertyChanges {target: root; height: root.notificationHeight*2}
			PropertyChanges {target: notification; visible: true}
			PropertyChanges {target: critical; visible: true}
			}

	]
		

}

