import LIBMEME as ml

# cli args
for e in ml.config.args():
    ml.io.info(e)

# load plugins
#if (ml.config.os() == "Windows"):
#    ml.engine.load_plugin("demo.dll")