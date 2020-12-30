
import sys
import networkx as nx
import matplotlib.pylab as pl
import time
import numpy as np



n = int(sys.argv[1])
k = int(sys.argv[2])

for p in np.arange(0,1.01,0.01):

	ldata =[]
	r = 0

	while True:
		G = nx.watts_strogatz_graph(n,k,p,time.time())
		if(nx.is_connected(G)): break

	
	f = open("networks/ring_1024_2_hr/ring_"+str(n)+"_"+str(k)+"_"+"%3.2f" % (p)+".dat","w")
	f.write("#agents\nn=%d\n#adjacency\n" % (n))
	for line in nx.generate_adjlist(G,","):
		li = line.split(",")
		for l in nx.generate_adjlist(G,","):
			litemp = l.split(",")
			if(litemp[0] != li[0]):
				if(li[0] in litemp): li.append(litemp[0])
		del li[0]
		string =",".join(li)
		f.write("%s\n" %(string))
		
	f.close()



