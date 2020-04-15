# * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * #

import libmeme_config   as cfg
import libmeme_core     as core
import libmeme_platform as pfm
import libmeme_engine   as engine

# load plugins
assert(engine.plugins.load("demo"))

# open window
assert(engine.window.open(pfm.window_settings(
    "libmeme",              # title
    pfm.display_settings(
        [ 1280, 720 ],      # resolution
        32),                # color depth
    pfm.context_settings(
        pfm.api.opengl,     # api
        4,                  # major
        6,                  # minor
        pfm.profile.compat, # profile
        24,                 # depth bits
        8,                  # stencil bits
        False,              # multisample
        False),             # srgb capable
    93),                    # window hints
    True)                   # install callbacks
    )

# initialize gui
assert(engine.gui.init("#version 130"))

# load style
engine.gui.load_style(engine.path_to("assets/styles/obsidian.style"))

# print messages
print(f"# {cfg.project.name} | {cfg.build.arch!r}-bit | {cfg.build.configuration}")
print(f"# {cfg.project.url}")
print(f"# type \'help\' for a list of commands")
print("")

# * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * #