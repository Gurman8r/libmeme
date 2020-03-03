import LIBMEME as ml
import sys

# system setup
# * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * #

class ml_stdio(object):
	def __init__(self):
		sys.stdout	= self
		sys.stderr	= self
		sys.stdin	= None
	def write(self, args = None):
		ml.io.print(str(args))
ml_stdio()
exit = ml.engine.close
sys.exit = ml.engine.close


# load plugins
# * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * #

plugins = [ "demo", ]

for p in plugins:
	if ml.io.os() == "Windows":
		ml.engine.load_plugin(p + ".dll")
	else:
		ml.engine.load_plugin(p + ".so")


# messages
# * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * #

print("# " + ml.io.name()+ " | " + str(ml.io.arch()) + "-bit | " + ml.io.conf())
print("# " + ml.io.url())
print("# type \'help\' for a list of commands")
