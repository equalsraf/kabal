import QtQuick 2.3

Rectangle {
	id: button
	
	property url source: ""
	property int size: 32
	property alias mirror: img.mirror

	signal clicked

	property bool checkable: false
	property bool checked: false
	property url sourceChecked: ""

	width: size
	height: size
	color: "transparent"
	Style { id: style}

	Image {
		id: img
		anchors.fill: parent;
		fillMode: Image.PreserveAspectFit
		source: button.source
	}

	MouseArea {
		id: mousearea
		hoverEnabled: true
		anchors.fill: parent
		onClicked: {
			if ( button.checkable ) {
				if ( button.state === 'checked' ) {
					button.state = 'unchecked';
				} else {
					button.state = 'checked';
				}
			}
			button.clicked()
		}
	}

	states: [
		State {
			name: "unchecked"
			PropertyChanges {target: img; source: button.source}
			PropertyChanges {target: button; checked: false}
		},
		State {
			name: "checked"
			PropertyChanges {target: img; source: button.sourceChecked}
			PropertyChanges {target: button; checked: true}
		}
	]
}


