# * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * #

import libmeme as ml

# * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * #

class my_script(ml.script.object):
    def __init__(self, *args, **kwargs):
        return super(my_script, self).__init__(self, *args, **kwargs)

    def __call__(self, *args):
        return

    def on_enable(self):
        return

    def on_disable(self):
        return

# * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * #

ml.engine.register_script(my_script)

test = my_script()

test()

# * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * #