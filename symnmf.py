import numpy as np
import sys
import symnmf  # Importing the C extension

def read_input(file_name):
    return np.loadtxt(file_name, delimiter=',')

def convert_to_list(X):
    return X.tolist()

def print_matrix(H):
    for row in H:
        print(",".join("{:.4f}".format(number) for number in row))

if __name__ == "__main__":
    # האם צריך לבדוק אם k קטן מ N
    k = int(sys.argv[1])
    goal = sys.argv[2]
    file_name = sys.argv[3]
    X = read_input(file_name)

    # Convert X to a list of lists
    x_list = convert_to_list(X)

    n = len(x_list)  # Get the number of data points (lines)
    d = len(x_list[0])  # Get the dimensionality of the data points (columns)

    if goal == "symnmf":
        W = symnmf.norm(x_list, d, n)  # Call the wrapped function with the list
        
        # Initialize H
        m = np.mean(W)
        np.random.seed(0)
        H = np.random.uniform(0, 2 * np.sqrt(m / k), size=(n, k)) 
        H = H.tolist() 

        H = symnmf.symnmf(W, H, d, n, k)
        print_matrix(H)

    elif goal == "sym":
        sym = symnmf.sym(x_list, d, n)  # Call the C function via the wrapper
        print_matrix(sym)
    elif goal == "ddg":
        ddg = symnmf.ddg(x_list, d, n)  # Call the C function via the wrapper
        print_matrix(ddg)
    elif goal == "norm":
        W = symnmf.norm(x_list, d, n)
        print_matrix(W)
    
