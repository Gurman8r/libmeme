import LIBMEME_SYSTEM as system
import LIBMEME_EDITOR as editor
import LIBMEME_ENGINE as engine
import sys

# * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * #

# system setup
exit        = system.exit
sys.exit    = system.exit
sys.stdout  = system.cout
sys.stderr  = system.cout
sys.stdin   = None

# * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * #

def on_startup():
    system.load_plugin("demo")
    return 0

def on_load():
    print(f"# {system.lib_name} | {system.architecture!r}-bit | {system.configuration}")
    print(f"# {system.lib_url}")
    print(f"# type \'help\' for a list of commands")
    return 0

def on_unload():
    return 0

def on_shutdown():
    return 0

# * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * #

# install callbacks
engine.add_callback("startup",   on_startup)
engine.add_callback("load",      on_load)
engine.add_callback("unload",    on_unload)
engine.add_callback("shutdown",  on_shutdown)

# * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * #
