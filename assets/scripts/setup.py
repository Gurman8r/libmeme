# * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * #

import libmeme_core as core
import libmeme_engine as engine

engine.plugins.load("demo")

print(f"# {core.project.name} | {core.build.arch!r}-bit | {core.build.config}")
print(f"# {core.project.url}")
print(f"# type \'help\' for a list of commands")
print("")

# * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * #