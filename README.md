# Superoptimization Experiments

---

Trying to build some experimental stuff with superoptimization, trying out some new techniques.

For example, a simple observation:

f(g(x)) != f(g(y)) => g(x) != g(y)

If we implement an exhaustive search as a depth-first search, we can reduce the cost of calculating test cases by calculating intermediate results during the DFS. If two test cases produce different results at the end, but have identical intermediate results, we can immediately assume that proceeding further down that path will be pointless.
