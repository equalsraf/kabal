
import pynotify
import gobject

body = """
this the <b>body</b> of a 
notification <unk>:D</unk>
<font>some awesome stuff</font>
"""

loop = gobject.MainLoop()
pynotify.init ("Test")
test=pynotify.Notification ("Testing html", body)
test.set_timeout(10000)

test.show ()
