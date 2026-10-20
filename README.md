# SymNMF Clustering

An implementation of **Symmetric Non-negative Matrix Factorization (SymNMF)** for clustering, written in C with a Python C-API extension, and benchmarked against K-means using the silhouette score.

## How it works

Given `n` data points, SymNMF:

1. Builds the **similarity matrix** `A`, where `a_ij = exp(-‖x_i − x_j‖² / 2)` (and `a_ii = 0`)
2. Computes the **diagonal degree matrix** `D`, where `d_ii = Σ_j a_ij`
3. Computes the **normalized similarity matrix** `W = D^(-1/2) · A · D^(-1/2)`
4. Finds a non-negative `H` (`n × k`) minimizing `‖W − HHᵀ‖²_F` with multiplicative updates (β = 0.5), stopping when the change is below ε = 1e-4 or after 300 iterations
5. Assigns each point to the cluster with the largest value in its row of `H`

The numeric work is in C (`symnmf.c`). `symnmfmodule.c` exposes it to Python, and `symnmf.py` handles initialization and I/O.

## Project structure

| File | Description |
|------|-------------|
| `symnmf.c`, `symnmf.h` | Core C implementation and standalone CLI |
| `symnmfmodule.c` | Python C extension wrapping the C functions |
| `symnmf.py` | Python interface: initializes `H` and calls the extension |
| `analysis.py` | Compares SymNMF and K-means using the silhouette score |
| `kmeans.py` | Standalone K-means implementation |
| `setup.py`, `Makefile` | Build the Python extension and the C executable |
| `input_*.txt` | Sample datasets (comma-separated points, one per line) |

## Build

Requires `gcc` and Python 3 with the packages in `requirements.txt`.

```bash
pip install -r requirements.txt
python3 setup.py build_ext --inplace   # Python extension
make                                   # standalone C executable (ANSI C, -Wall -Werror)
```

## Usage

```bash
# Python: goal is one of symnmf | sym | ddg | norm
python3 symnmf.py <k> <goal> <file>
python3 symnmf.py 3 symnmf input_1.txt

# C: goal is one of sym | ddg | norm
./symnmf <goal> <file>
./symnmf sym input_1.txt

# Compare SymNMF vs. K-means (silhouette score)
python3 analysis.py <k> <file>
python3 analysis.py 3 input_1.txt
```

Every matrix is printed with one row per line and values formatted to 4 decimal places.

`make clean` removes all build output.
