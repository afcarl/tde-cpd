import operator
import itertools
import numpy as np
import matplotlib.pyplot as plt
from matplotlib.colors import colorConverter
from matplotlib.collections import LineCollection
from matplotlib.colors import ListedColormap, BoundaryNorm

data = np.loadtxt('../datasets/gatech_honeybee/sequence1.dat', dtype=np.dtype({'names':['x', 'y', 'mode'], 
  'formats':[np.double, np.double, 'S100']}))

segments = [np.array(list(g)) for k, g in itertools.groupby(data, key=operator.itemgetter(2))]

colormap = {'waggle':'-r', 'turn_left':'-g', 'turn_right':'-b'}

for segment in segments:
    plt.plot(segment['x'], segment['y'], colormap[segment['mode'][0]], linewidth=2.0)

plt.show()
