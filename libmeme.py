import LIBMEME as ml

# print command-line args
for e in ml.config.args(): ml.io.info(e)

# load plugins
ml.engine.load_plugin("demo")