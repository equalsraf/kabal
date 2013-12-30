

Kabal is a notification server, like notify-osd, knotify and colibri. It is written
using Qt and QML. It should work well with any desktop environment.

To build and run

    $ qmake
    $ make
    $ ./kabal

If you are already running a notification server you will need to disable it
first.

# What makes Kabal different from XYZ?

Hopefully it is more sane by default than most other notification servers out there. My own
frustrations with notification servers fall into one of the following:

1. They are too disruptive and annoy me (knotify, xfce)
2. They are too lenient and lose some important 
   notifications (colibri, notify-osd)
3. They are too light in features - applications 
   assume the notification server has some features
   (html, actions, etc) and won't work properly without those

What does Kabal do differently?

1. It only shows the LATEST notification and no more
2. Non critical notifications can be disabled!
3. It stores a logfile of notifications
4. It has multi screen support, i.e. one notification
   will be displayed in each screen

# How do I use it?

Whenever Kabal starts it will place an icon in system tray. From that icon
you can:

1. Right click to access more options (e.g. disable notifications)
1. ...

On the notification widget:

1. Single click a notification to close it, or invoke the default action
   for the notification
2. There should be action buttons inside each notification
3. A status bar is available under the notifications, with
   buttons to switch and disable notifications
4. The mouse wheel can be used to scroll through notifications

# Configuration options

Kabal will read settings from $HOME/.kabal/config. Check the docs
for more details.

# TODO 

* Configurable Widget position [Works, but we need a GUI for this]
* Settings in the notification widget
* Fix animations when enabling/disabling/removing notifications

# License

This code available under the New (3-clause) BSD license

