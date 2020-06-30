import libmeme          as ml       # structures
import libmeme_engine   as engine   # systems

ws = ml.window_settings(
    "libmeme",                      # title
    ml.video_mode
    (
        [ 1280, 720 ],              # resolution
        32                          # color depth
    ),
    ml.context_settings
    (
        ml.client_api.opengl,       # client api
        4, 6,                       # api version
        ml.client_profile.compat,   # api profile
        24,                         # depth bits
        8,                          # stencil bits
        True,                       # multisample
        False                       # sRGB capable
    ),
    ml.window_hints.default_max     # hints
    )

# open window
print("[i] opening window...")
assert(engine.window.open(ws, True))

# initialize gui
print("[i] initializing gui...")
assert(engine.gui.initialize())
engine.gui.load_style(engine.fs.path2("assets/styles/obsidian.style"))

# load plugins
print("[i] loading plugins...")
assert(engine.plugins.load("demo"))

# print messages
print(f"# {ml.prj.libname} | {ml.cfg.arch!r}-bit | {ml.cfg.configuration}")
print(f"# {ml.prj.url}")
print("# type \'help\' for a list of commands")
print("")