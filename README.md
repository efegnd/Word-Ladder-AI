# Word Ladder AI Graph Search Engine

## 📌 Project Overview
Welcome to my Word Ladder AI project! This repository contains the search engine I developed for my *Introduction to Artificial Intelligence* course to solve the classic combinatorial "Word Ladder" puzzle. 

Instead of just writing a basic script, I modeled the English vocabulary as a complex graph search environment. Every valid word is a node, and every single-character transformation is a directed edge. The goal of this project was to explore how different search algorithms handle massive state spaces and exponentially growing branching factors.

## 🚀 Algorithms Implemented
To compare performance, execution speed, and path optimality, I integrated several algorithms:
* **Uninformed (Blind) Search:** Breadth-First Search (BFS), Depth-First Search (DFS), and Uniform-Cost Search (UCS).
* **Informed (Heuristic) Search:** Greedy Best-First Search and A* Search.
* **Generalized A* Model:** I also implemented a parameterized A* model using an $\alpha$ weight. By tweaking $\alpha$ between 0.0 and 1.0, the algorithm dynamically shifts from pure Uniform-Cost Search to standard A*, and finally to a fast but sub-optimal Greedy search.

## 🧠 The Challenge & Architectural Optimizations
While testing 3-letter words was straightforward, scaling up to the 6-letter word domain caused severe memory exhaustion. The search frontier grew too fast. To solve these bottlenecks, I had to engineer a few specific optimizations:

* **Prime-Sized Custom Hash Table:** Tracking explored states with a standard array was taking $O(N)$ linear time and causing huge delays. To fix this, I designed a custom modular Hash Table (Size: 503, specifically chosen as a prime number to reduce collisions). This simple mathematical decision dropped our state-lookup latency to a constant $O(1)$ time and completely prevented cyclical infinite loops.
* **Admissible Heuristic (Hamming Distance):** To give the A* algorithm directional guidance, I used the Hamming Distance to count positional character mismatches. As I mathematically proved in the project report, this heuristic is strictly **admissible** and **consistent**, meaning it aggressively prunes bad paths without ever missing the shortest possible route.

## 👨‍💻 My Role in the Project
While this was officially submitted as a group academic project, I took the initiative to lead the core technical architecture and coding. My specific contributions include:
1. **Algorithm Integration:** Writing the core logic for the BFS, DFS, and A* search frameworks.
2. **Memory Optimization:** Designing and integrating the 503-sized Hash Table to solve our memory overflow issues.
3. **Heuristic Engineering:** Formulating the Hamming Distance heuristic and proving its consistency for the $f(n) = g(n) + h(n)$ evaluation function.
4. **Performance Analysis:** Running the final stress tests. In our hardest test case (`WINTER` --> `TENDER`), my optimized A* model successfully found the target by expanding only 14 nodes out of an immense operational space!

## 📄 Documentation & Proofs
If you are interested in the mathematical proofs of the heuristic function, detailed algorithm complexity analysis, or real-time terminal execution logs, please check out the `WordLadder_Project_Report.pdf` included in this repository.
