# * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * #

import LIBMEME_CONFIG as config
import LIBMEME_SYSTEM as system
import LIBMEME_EDITOR as editor
import LIBMEME_ENGINE as engine

# * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * #

# system setup
class ml_setup(object):
    def __init__(self):
        import sys
        sys.stdout  = self
        sys.stderr  = self
        sys.stdin   = None
        sys.exit    = engine.close
    def write(self, args):
        system.write(args)
ml_setup()
exit = engine.close

# * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * #

# editor settings
editor.load_style(engine.path_to("assets/styles/obsidian.style"))

# load plugins
plugins = [
   "demo",
  ]
for p in plugins: engine.load_plugin(p)

# messages
print("# " + config.proj_name()+ " | " + str(config.arch()) + "-bit | " + config.config())
print("# " + config.proj_url())
print("# type \'help\' for a list of commands")

# * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * #