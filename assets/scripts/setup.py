import LIBMEME_CONFIG   as config
import LIBMEME_STDIO    as stdio
import LIBMEME_EDITOR   as editor
import LIBMEME_ENGINE   as engine
import sys

# system setup
# * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * #
class ml_system_wrapper(object):
    def __init__(self):
        sys.stdout  = self
        sys.stderr  = self
        sys.stdin   = None
        sys.exit    = engine.close
    def write(self, args):
        stdio.write(str(args))
ml_system_wrapper()
exit = engine.close


# load plugins
# * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * #
plugins = [
   "demo",
  ]
for p in plugins:
    if config.system() == "Windows":
       engine.load_plugin(p + ".dll")
    else:
       engine.load_plugin(p + ".so")


# messages
# * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * #
print("# " + config.proj_name()+ " | " + str(config.arch()) + "-bit | " + config.config())
print("# " + config.proj_url())
print("# type \'help\' for a list of commands")
