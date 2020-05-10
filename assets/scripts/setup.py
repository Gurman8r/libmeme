import libmeme          as ml
import libmeme_engine   as engine

# setup window
print("[ info ] opening window...")
assert(engine.window.open(ml.window_settings(
    "libmeme",
    ml.video_mode([ 1280, 720 ]),
    ml.context_settings(
        ml.window_renderer.opengl, 4, 6,
        ml.window_profile.compat,
        24,     # depth bits
        8,      # stencil bits
        True,   # multisample
        False), # sRGB capable
    ml.window_hints.default_maximized
    )))
engine.window.install_default_callbacks()
engine.window.set_cursor_mode(ml.cursor_mode.normal)

# setup gui
print("[ info ] initializing gui...")
assert(engine.gui.initialize("#version 130"))
engine.gui.load_style(engine.fs.path2("assets/styles/obsidian.style"))

# setup plugins
print("[ info ] loading plugins...")
assert(engine.plugins.load("demo"))

# print messages
print(f"# {ml.project.name} | {ml.build.arch!r}-bit | {ml.build.configuration}")
print(f"# {ml.project.url}")
print("# type \'help\' for a list of commands")
print("")