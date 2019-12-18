# * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * #

import MEMELIB as ml

# Debug Mode
if ml.config.is_debug():
    
    for e in ml.config.args():
        ml.io.printl(e)
    
    ml.io.pause()
    ml.io.exit()

# Load Plugins
if (ml.config.system_name() == "Windows"):
    ml.plugins.load("demo.dll")

# * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * #