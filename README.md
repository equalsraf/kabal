
# About

Kabal is a notification server, like notify-osd, knotify and colibri.


# What makes it different from XYZ?

Hopefully its more sane by default than most other notification servers out there. My own
frustrations with notification servers falls into one of two categories:

1. They are too disruptive and annoy me (knotify, xfce)
2. They are too lenient and lose some important 
   notifications (colibri, notify-osd)

What does Kabal do differently?

1. It only shows the LATEST notification and no more
2. Notifications can be disabled!
3. Critical notifications are alway shown regardless
   of (2) i.e. battery warnings and so on
4. It stores a logfile of notifications during the
   past session

# How do I use it?

Whenever Kabal starts it will place an icon in system tray. From that icon
you can:

1. Right click to access more options (e.g. disable notifications)
1. ...

On the notification widget:

1. Single click a notification to close it


## Kog file

Kabal keeps a log file of past notifications in $HOME/.kabal/log.

# TODO 

* Configurable Widget position

