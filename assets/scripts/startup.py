# * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * #

import libmeme          as ml
import libmeme_engine   as engine

# setup window
assert(engine.window.open(ml.window_settings(
    "libmeme",              # title
    ml.display_settings(
        [ 1280, 720 ],      # resolution
        32),                # color depth
    ml.context_settings(
        ml.platform.opengl, # api
        4,                  # major
        6,                  # minor
        ml.platform.compat, # profile
        24,                 # depth bits
        8,                  # stencil bits
        False,              # multisample
        False),             # srgb capable
    93),                    # window hints
    True)                   # install callbacks
    )

# setup gui
assert(engine.gui.init("#version 130"))

# load style
engine.gui.load_style(ml.fs.path_to("assets/styles/obsidian.style"))

# load plugins
assert(engine.plugins.load("demo"))

# print messages
print(f"# {ml.project.name} | {ml.build.arch!r}-bit | {ml.build.configuration}")
print(f"# {ml.project.url}")
print(f"# type \'help\' for a list of commands")
print("")

# testing
engine.assets.load("image", "my_image", "../foo.png")
engine.assets.load("texture", "my_texture", "my_image")

# * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * #