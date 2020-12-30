
import sys
import numpy as np
import networkx as nx
import matplotlib.pylab as pl
import time

n = 128
k = 2

P = np.arange(0,1,0.001)

for q in range(50):
	
	E= np.empty(0)
	C= np.empty(0)

	print(q)
	
	f = open("output/ec/128_2/ec_128_2(%d)" %(q),"w")

	for p in P:
        	eff = 0.0
        	while True:
                	G = nx.watts_strogatz_graph(n,k,p)
                	if(nx.is_connected(G)): break
        	for x in range(n):
                	for y in range(x):
                        	d = float(nx.shortest_path_length(G,x,y))
                        	eff = eff+ (1.0/d)
		
		eff = eff/(n*(n-1))
		f.write("%f %f %f\n" % (p,eff,nx.average_shortest_path_length(G)))

	f.close()




