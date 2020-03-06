# * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * #

import sys
import LIBMEME_CONFIG   as config
import LIBMEME_STDIO    as stdio
import LIBMEME_EDITOR   as editor
import LIBMEME_ENGINE   as engine

# * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * #

# system setup
class ml_sys_wrapper(object):
    def __init__(self):
        sys.stdout  = self
        sys.stderr  = self
        sys.stdin   = None
        sys.exit    = engine.close
    def write(self, args):
        stdio.write(str(args))
ml_sys_wrapper()
exit = engine.close

# * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * #

# editor setup
editor.load_style(engine.path_to("assets/styles/obsidian.style"))

# load plugins
plugins = [
   "demo",
  ]
for p in plugins:
    if config.os() == "Windows": engine.load_plugin(p + ".dll")
    else: engine.load_plugin(p + ".so")

# messages
print("# " + config.proj_name()+ " | " + str(config.arch()) + "-bit | " + config.config())
print("# " + config.proj_url())
print("# type \'help\' for a list of commands")

# * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * #