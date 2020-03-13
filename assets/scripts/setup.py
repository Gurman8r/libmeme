import LIBMEME_ENGINE as engine
import LIBMEME_EDITOR as editor
import sys

# awake
# * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * #
def on_awake():

    # system setup
    __builtins__.exit   = engine.exit
    sys.exit            = engine.exit
    sys.stdout          = engine.cout
    sys.stderr          = engine.cout
    sys.stdin           = None
    
    return 0

# load
# * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * #
def on_load():

    # load plugins
    engine.load_plugin("demo")

    # messages
    print(f"# {engine.lib_name} | {engine.architecture!r}-bit | {engine.configuration}")
    print(f"# {engine.lib_url}")
    print(f"# type \'help\' for a list of commands")

    return 0

# unload
# * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * #
def on_unload():
    return 0


# install callbacks
# * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * #
engine.add_callback("awake", on_awake)
engine.add_callback("load", on_load)
engine.add_callback("unload", on_unload)
