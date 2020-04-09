# * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * #

import libmeme_core as core
import libmeme_engine as engine

engine.load_plugin("demo")

print(f"# {core.library.name} | {core.build.arch!r}-bit | {core.build.config}")
print(f"# {core.library.url}")
print(f"# type \'help\' for a list of commands")
print("")

# * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * #