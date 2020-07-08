import libmeme          as ml
import libmeme_engine   as engine

# open window
print("[i] opening window...")
assert(engine.window.open(ml.window_settings(
    "libmeme",                      # title
    ml.video_mode(
        [ 1280, 720 ],              # resolution
        [ 8, 8, 8, 8 ],             # bits per pixel
        -1),                        # refresh rate
    ml.context_settings(
        ml.context_api.opengl,      # api
        4, 6,                       # version
        ml.context_profile.compat,  # profile
        24,                         # depth bits
        8,                          # stencil bits
        True,                       # multisample
        False),                     # sRGB capable
    ml.window_hints.default_max     # hints
    )))

# initialize gui
print("[i] initializing gui...")
assert(engine.gui.initialize())
engine.gui.load_style(engine.fs.path2("assets/styles/obsidian.style"))

# load plugins
print("[i] loading plugins...")
assert(engine.plugins.load("demo"))

# print messages
print(f"# {ml.lib.name} | {ml.cfg.arch!r}-bit | {ml.cfg.configuration}")
print(f"# {ml.lib.url}")
print("# type \'help\' for a list of commands")
print("")