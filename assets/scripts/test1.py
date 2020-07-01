import libmeme          as ml       # data
import libmeme_engine   as engine   # systems

# setup window
print("[i] loading window...")
assert(engine.window.open(
    ml.window_settings(
        "libmeme",                      # title
        ml.video_mode(
            [ 1280, 720 ],              # resolution
            32),                        # color depth
        ml.context_settings(
            ml.client_api.opengl,       # client api
            4, 6,                       # api version
            ml.client_profile.compat,   # api profile
            24,                         # depth bits
            8,                          # stencil bits
            True,                       # multisample
            False),                     # sRGB capable
        ml.window_hints.default_max),   # hints
    True))                              # install callbacks

# setup gui
print("[i] loading gui...")
assert(engine.gui.initialize())
engine.gui.load_style(engine.fs.path2("assets/styles/obsidian.style"))

# setup plugins
print("[i] loading plugins...")
assert(engine.plugins.load("demo"))

# messages
print(f"# {ml.prj.libname} | {ml.cfg.arch!r}-bit | {ml.cfg.configuration}")
print(f"# {ml.prj.url}")
print("# type \'help\' for a list of commands")
print("")