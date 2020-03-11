import LIBMEME_SYSTEM as system
import LIBMEME_EDITOR as editor
import LIBMEME_ENGINE as engine
import sys

# system setup
exit        = system.exit
sys.exit    = system.exit
sys.stdout  = system.cout
sys.stderr  = system.cout
sys.stdin   = None

# load plugins
plugins = [ "demo", ]
for p in plugins: system.load_plugin(p)

# messages
print(f"# {system.lib_name} | {system.architecture!r}-bit | {system.configuration}")
print(f"# {system.lib_url}")
print("# type \'help\' for a list of commands")
