# * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * #

import libmeme as ml

ml.plugins.load("demo")

ml.window.create(ml.window_settings(
    "libmeme",
    ml.display_settings([ 1280, 720 ], 32),
    ml.context_settings(1, 4, 6, 2, 24, 8, False, False),
    93
    ))

print(f"# {ml.project.name} | {ml.build.arch!r}-bit | {ml.build.config}")
print(f"# {ml.project.url}")
print(f"# type \'help\' for a list of commands")
print("")


# * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * #