# * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * #

import LIBMEME_ENGINE as ml

ml.load_plugin("demo")

print(f"# {ml.lib.name()} | {ml.lib.arch()!r}-bit | {ml.lib.config()}")
print(f"# {ml.lib.url()}")
print(f"# type \'help\' for a list of commands")

# * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * #

class scr_test(ml.scriptable_object):
    def __init__(self, *args, **kwargs):
        return super(scr_test, self).__init__(self, *args, **kwargs)

    def awake(self):        return print(">> awake")
    def on_disable(self):   return print(">> on_disable")
    def on_enable(self):    return print(">> on_enable")
    def reset(self):        return print(">> reset")
    def start(self):        return print(">> start")
    def update(self):       return print(">> update")

with scr_test(enabled = True) as test:
    print(f"args    | {test.args}")
    print(f"kwargs  | {test.kwargs}")
    print(f"flags   | {test.flags}")
    print(f"enabled | {test.enabled}")
    print(f"active  | {test.active}")
    test("update")

# * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * #