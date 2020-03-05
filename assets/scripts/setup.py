import encodings
import sys
import LIBMEME as ml


# system setup
# * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * #
class ml_stdio(object):
    def __init__(self):
        sys.stdout  = self
        sys.stderr  = self
        sys.stdin   = None
    def write(self, args = None):
        if (not args == None): ml.io.print(str(args))
ml_stdio()
exit = ml.engine.close
sys.exit = ml.engine.close


# load plugins
# * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * #
plugins = [
   "demo",
  ]
for p in plugins:
    if ml.cfg.os() == "Windows":
        ml.engine.load_plugin(p + ".dll")
    else:
        ml.engine.load_plugin(p + ".so")


# messages
# * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * #
print("# " + ml.cfg.name()+ " | " + str(ml.cfg.arch()) + "-bit | " + ml.cfg.config())
print("# " + ml.cfg.url())
print("# type \'help\' for a list of commands")
