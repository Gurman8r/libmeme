# * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * #

import libmeme as ml

class my_script(ml.script_object):
    def __init__(self, **kwargs):
        super(my_script, self).__init__(self, **kwargs)
        return

    def on_activate(self):
        return

    def on_deactivate(self):
        return

    def on_tick(self):
        return

# * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * #

with ml.object.create("my_object") as obj:
    obj.add_script(my_script, enabled = True)

# * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * #