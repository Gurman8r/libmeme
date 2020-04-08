# * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * #

import libmeme_core as core
import libmeme_engine as engine

engine.load_plugin("demo")

print(f"# {core.lib.name} | {core.lib.arch!r}-bit | {core.lib.config}")
print(f"# {core.lib.url}")
print(f"# type \'help\' for a list of commands")
print("")

# * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * #