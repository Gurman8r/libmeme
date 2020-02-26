# imports
# * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * #
import LIBMEME as ml
import sys


# setup library
# * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * #
class __ml_py_lib__(object):
    def __init__(self):
        sys.exit    = self.exit
        sys.stdout  = self
        sys.stderr  = self
    def exit(self, args = None) : ml.engine.close()
    def write(self, args = None): ml.io.print(str(args))
g_lib = __ml_py_lib__()
exit = g_lib.exit


# load plugins
# * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * #
plugins = [ "demo", ]
for p in plugins:
    if ml.io.os() == "Windows":
       ml.engine.load_plugin(p + ".dll")
    else:
       ml.engine.load_plugin(p + ".so")
	

# messages
# * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * #
print("# " + ml.io.name() + " | " + str(ml.io.arch()) + "-bit | " + ml.io.conf())
print("# " + ml.io.url())
print("# type \'help\' for a list of commands")
