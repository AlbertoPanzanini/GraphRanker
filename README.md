# GraphRanker

This is the final project for the course **"Algoritmi e Principi dell'Informatica"** (Algorithms and Data Structures), held at [Politecnico di Milano](https://www.polimi.it/).

---

### Project Description

You are to implement in C (C11, VLA) a program that **ranks weighted directed graphs**.

---

### Requirements

The program receives in input:
- two **parameters** (only once, at the beginning of the input, separated by a space):
    - `d` the **number** of **nodes** in the graph.
    - `k` the **length** of the **ranking**.
- a sequence of the **commands**:
    - `AggiungiGrafo [adjacency_matrix]` \
      **Adds a graph** to those considered for the ranking.\
      The adjacency matrix is given with elements separated by a comma and each input line corresponds to a row of the matrix.\
      The weights on the line 'n' of the adjacency matrix are those of the outgoing arcs of node 'n'.\
      The weights of the arcs are in the range: \[0 , 2<sup>32</sup> -1 \].
      
      ![AggiungiGrafo example](https://github.com/AlbertoPanzanini/GraphRanker/blob/main/GraphExample.png)
      
    - `TopK` \
      **Returns** the integer **indexes** of the **k graphs**, given in input until that command, with the **lowest score**.\
      The graphs are indexed with an increasing integer count, representing the number of graphs read before that one. Indeed, the first graph given should be indexed as 0.\
      The score of a graph is the total cost of the shortest paths from node 0 to all the other reachable nodes.\
      The cost of a path from 0 to a node that is not reachable, is 0.\
      Tie-breaker rule: precedence in rank according to input order.\
      The k indexes are printed on a single line, separated by a space, in any order.

Short example:\
Given the following input

>3 2\
>AggiungiGrafo\
>0,4,3\
>0,2,0\
>2,0,0\
>AggiungiGrafo\
>0,0,2\
>7,0,4\
>0,1,0\
>AggiungiGrafo\
>3,1,8\
>0,0,5\
>0,9,0\
>TopK

The expected output is either

> 0 1

or

> 1 0



For further details, consult the [full specifics](https://github.com/AlbertoPanzanini/GraphRanker/blob/main/ProvaFinale2021.pdf).

---

### Assumptions

`d`, `k` and the `number of graphs` can be represented with 32-bit integers.

---

### Implementation details

The shortest paths are calculated using Dijkstra Algorithm.

---
