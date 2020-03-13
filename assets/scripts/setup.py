import LIBMEME_ENGINE as engine
import LIBMEME_EDITOR as editor

# * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * #

class test():
    def awake():
        import builtins
        import sys
        builtins.exit   = engine.exit
        sys.exit        = engine.exit
        sys.stdout      = engine.cout
        sys.stderr      = engine.cout
        sys.stdin       = None
        return
    
    def load():
        plugins = [ "demo", ]
        for p in plugins: engine.load_plugin(p)
        return
    
    def unload():
        return
    
    def start():
        print(f"# {engine.lib_name} | {engine.architecture!r}-bit | {engine.configuration}")
        print(f"# {engine.lib_url}")
        print(f"# type \'help\' for a list of commands")
        return

# * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * #

engine.add_callback("awake",    test.awake)
engine.add_callback("load",     test.load)
engine.add_callback("unload",   test.unload)
engine.add_callback("start",    test.start)

# * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * #
