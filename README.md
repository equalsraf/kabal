
# About

Kabal is a notification server, like notify-osd, knotify and colibri. It is written
using Qt and QML. It should work with any desktop environment.


# What makes Kabal different from XYZ?

Hopefully its more sane by default than most other notification servers out there. My own
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

Run kabal, e.g.

    ./kabal

Whenever Kabal starts it will place an icon in system tray. From that icon
you can:

1. Right click to access more options (e.g. disable notifications)
1. ...

On the notification widget:

1. Single click a notification to close it, or invoke the default action
   for the notification
2. There should be action buttons inside each notification
3. A status bar with is available under the notifications, with
   buttons to switch and disable notifications
4. The mouse wheel can be used to scroll through notifications

# TODO 

* Configurable Widget position [Works, but we need a GUI for this]
* Settings in the notification widget

# Configuration options

Kabal will read settings from $HOME/.kabal/config - but there aren't
many options yet, other than the widget position. Here are some configuration 
the default settings

    log = /home/user/.kabal/log
    trunclog = true
    x = 10
    y = 10
    corner = topleft
    mintime = 7000


