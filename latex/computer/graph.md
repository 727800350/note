# graphviz
dot accepts input in the DOT language. 

This language describes three kinds of objects: graphs, nodes, and edges. 
The main (outermost) graph can be directed (digraph) or undirected graph. 
Because dot makes layouts of directed graphs, all the following examples use digraph.
(A separate layout utility, neato, draws undirected graphs.) 
Within a main graph, a subgraph defines a subset of nodes and edges.

	digraph G {
		main -> parse -> execute;
		main -> init;
		main -> cleanup;
		execute -> make_string;
		execute -> printf
		init -> make_string;
		main -> printf;
		execute -> compare;
	}

	$dot -Tps graph1.dot -o graph1.ps

Names of all these objects may be C identifiers, numbers, or quoted C
strings. Quotes protect punctuation and white space.

## Drawing Attributes
Nodes are drawn, by default, with `shape=ellipse, width=.75, height=.5`
and labeled by the node name.   
Other common shapes include box, circle, record and plaintext. 

