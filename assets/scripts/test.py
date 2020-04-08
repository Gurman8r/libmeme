# * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * #

from libmeme import ml_library  as lib
from libmeme import ml_stdio    as stdio
from libmeme import ml_engine   as engine

class my_script(engine.script_object):
    def __init__(self, **kwargs):
        super(my_script, self).__init__(self, **kwargs)
        return

    def on_activate(self):
        return

    def on_deactivate(self):
        return

    def on_tick(self):
        return

    def on_timer(self):
        return

# * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * #

test = my_script(enabled = True)
test.call("on_tick")

# * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * #