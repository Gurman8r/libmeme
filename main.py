import LIBMEME as ml

# print argv
for e in ml.config.argv():
   ml.io.info(e)

# load plugins
if ml.config.os() == "Windows":
    ml.engine.load_plugin("demo.dll")