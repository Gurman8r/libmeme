import libmeme          as ml       # structures
import libmeme_engine   as engine   # systems

# setup window
print("[i] loading window...")
assert(engine.window.open(ml.window_settings(
    "libmeme",
    ml.video_mode([ 1280, 720 ]),
    ml.context_settings(
        ml.window_client.opengl,        # renderer client
        4, 6,                           # client version
        ml.window_profile.compat,       # client profile
        24,                             # depth bits
        8,                              # stencil bits
        True,                           # multisample
        False),                         # sRGB capable
    ml.window_hints.default_maximized), # window hints
    True))                              # install callbacks

# setup gui
print("[i] loading gui...")
assert(engine.gui.initialize("#version 130"))
engine.gui.load_style(engine.fs.path2("assets/styles/obsidian.style"))

# setup plugins
print("[i] loading plugins...")
assert(engine.plugins.load("demo"))

# print messages
print(f"# {ml.project.libname} | {ml.build.arch!r}-bit | {ml.build.configuration}")
print(f"# {ml.project.url}")
print("# type \'help\' for a list of commands")
print("")