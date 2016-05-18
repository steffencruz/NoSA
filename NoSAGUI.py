"""NoSA GUI"""

from consys import consys
import numpy as np



#test system
cs = consys()

aa = 2.0 #mass spacing
mm = 1.0 #mass
kk = 3.0 #spring const
nmass = 10

#add masses
for ii in range(nmass):
    cs.AddMass(ii*aa, 0., mm)


for ii in range(nmass-1):
    cs.AddSpring(ii, ii+1, kk)

#show initialized system
cs.Print()
