# * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * #

import LIBMEME_EDITOR as editor
import LIBMEME_ENGINE as engine
import sys

# * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * #

# system setup
exit        = engine.close
sys.exit    = engine.close
sys.stdout  = engine.cout
sys.stderr  = engine.cout
sys.stdin   = None

# load style
editor.load_style(engine.path_to("assets/styles/obsidian.style"))

# load plugins
plugins = [ "demo", ]
for p in plugins: engine.load_plugin(p)

# messages
print("# %r | %r-bit | %r" % (engine.lib_name, engine.arch, engine.configuration))
print("# %r" % (engine.lib_url))
print("# type \'help\' for a list of commands")

# * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * #