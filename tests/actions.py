"""
Sends a notification with two actions and quits when one of the
actions is invoked
"""
import pynotify
import gobject


def onclicked(n, action):
    print("-----><", action)
    global loop
    loop.quit()

loop = gobject.MainLoop()
pynotify.init ("Test")
test=pynotify.Notification ("Testing actions","This is an example notification.","dialog-information")
test.set_timeout(60000)
test.add_action('actionkey', 'Click here', onclicked)
test.add_action('actionkey2', 'Click now', onclicked)

test.show ()
loop.run()

