import LIBMEME_PROJECT as project
import LIBMEME_ENGINE as engine

# * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * #

engine.load_plugin("demo")

def on_start():
    print(f"# {project.name} | {project.arch!r}-bit | {project.config}")
    print(f"# {project.url}")
    print(f"# type \'help\' for a list of commands")
    return

engine.hook("start", on_start)

# * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * #

class scr_test(engine.script):
    def __init__(self, *args, **kwargs):
        return super(scr_test, self).__init__(self, *args, **kwargs)
    
    def invoke(self, *args, **kwargs):
        print(f"# args:   {args}")
        print(f"# kwargs: {kwargs}")
        return 0

scr_test('Hello')('Welcome')

# * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * #