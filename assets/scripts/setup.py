import libmeme          as ml
import libmeme_engine   as engine

# open window
assert(engine.window.open(ml.window_settings(
    "libmeme",                  # title
    ml.video_mode(
        [ 1280, 720 ],          # resolution
        32),                    # color depth
    ml.context_settings(
        ml.window_api.opengl,   # api
        4,                      # major
        6,                      # minor
        ml.window_api.compat,   # profile
        24,                     # depth bits
        8,                      # stencil bits
        False,                  # multisample
        False),                 # srgb capable
    ml.window_hints.default_max # hints
    )))

# setup gui
assert(engine.gui.startup("#version 130"))

# load style
engine.gui.load_style(engine.fs.path_to("assets/styles/obsidian.style"))

# load plugins
assert(engine.plugins.load("demo"))

# print messages
print(f"# {ml.project.name} | {ml.build.arch!r}-bit | {ml.build.configuration}")
print(f"# {ml.project.url}")
print(f"# type \'help\' for a list of commands")
print("")