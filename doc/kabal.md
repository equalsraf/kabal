# KABAL(1) -- Kabal notification server


## SYNOPSIS

kabal [--persistence]

## DESCRIPTION

kabal is a notification server for X11. It receives, manages, and displays
desktop notifications.

## OPTIONS

--persistence
  Run kabal in persitent mode. In this mode, all notifications are persistent,
  meaning that by default they are never removed by timeout, unless the user or
  an application removes them.

## FILES

*~/.kabal/config*
  User config file for kabal

*~/.kabal/log*
  Log file for kabal notifications

## CONFIG FILE

The configuration file is a sectionless ini file

*log*
  The path of your log file (default: /home/username/.kabal/log)

*trunclog*
  Truncate the log file on start (default: true)

*corner*
  Place the notification widget at a given corner - topleft, topright,
  bottomleft, bottomright (default: topleft)

*x*
  x-axis distance from the *corner* (default:10)

*y*
  y-axis distance from the *corner* (default:10)

*mintime*
  The minimum time(ms) a notification should be visible for (default: 7000)

*persistence*
  If true notifications never timeout this can be overriden from the command
  line (default: false)

Here is an example config file

    x = 10
    y = 25
    corner = topright


## AUTHORS

Written by Rui Abreu Ferreira, after he got tired of whining about all other
notification servers.


