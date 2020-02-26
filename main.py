import LIBMEME as ml
import sys

# init
# * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * #
class __ml_py_init__(object):
    def __init__(self):
        sys.exit    = self.exit
        sys.stdout  = self
        sys.stderr  = self
    def exit(self, args = None) : ml.engine.close()
    def write(self, data = None): ml.io.print(str(data))
g_init = __ml_py_init__()
exit = g_init.exit


# load plugins
# * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * #
def load_plugin(path):
    if ml.config.os() == "Windows":
       ml.engine.load_plugin(path + ".dll")
    else:
       ml.engine.load_plugin(path + ".so")

load_plugin("demo")


# messages
# * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * #
for e in ml.config.argv(): print("# " + e)

print("# type \"help\" for a list of commands")
