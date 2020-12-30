
import sys
import numpy as np
import networkx as nx
import matplotlib.pylab as pl
import time

n = 16384

E= np.empty(0)
C= np.empty(0)

P = np.arange(0,1,0.001)
for p in P:
	
	print(p)
        eff = 0.0
        while True:
                G = nx.watts_strogatz_graph(n,2,p)
                if(nx.is_connected(G)): break
        for x in range(n):
                for y in range(x):
                        d = nx.shortest_path_length(G,x,y)
                        eff = eff+ (1.0/d)
        E = np.append(E,eff/(n*(n-1)))
        C = np.append(C,nx.average_shortest_path_length(G))

c_max = np.max(C)
e_max = np.max(E)

C = C/c_max
E = E/e_max

pl.xlabel("$p$")
pl.ylabel("$E,C$")
pl.title("$C_{max} = %f \;\; E_{max} = %f$" %(c_max,e_max))
pl.plot(P,C,label = "C")
pl.plot(P,E,label = "E")
pl.legend(loc = "best")
pl.show()



