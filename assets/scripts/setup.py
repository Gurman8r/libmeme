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

class ml_test(engine.script):
    def awake(self):
        return

    def start(self):
        return

# * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * #