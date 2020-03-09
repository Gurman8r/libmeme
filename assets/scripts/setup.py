# * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * #

import LIBMEME_SYSTEM as system
import LIBMEME_EDITOR as editor
import LIBMEME_ENGINE as engine
import sys

# * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * #

# system setup
exit        = engine.close
sys.exit    = engine.close
sys.stdout  = system.cout()
sys.stderr  = system.cout()
sys.stdin   = None

# * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * #

# editor setup
editor.load_style(engine.path_to("assets/styles/obsidian.style"))

# plugin setup
plugins = [ "demo", ]
for p in plugins: engine.load_plugin(p)

# messages
print("# " + system.prj.name()+ " | " + str(system.cfg.arch()) + "-bit | " + system.cfg.config())
print("# " + system.prj.url())
print("# type \'help\' for a list of commands")

# * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * #