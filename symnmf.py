import numpy as np
import sys
import symnmf  # Importing the C extension

def read_input(file_name):
    return np.loadtxt(file_name, delimiter=',')

def convert_to_list(X):
    return X.tolist()

def print_matrix(H):
    for row in H:
        print(" ".join("{:.4f}".format(number) for number in row))


if __name__ == "__main__":
    k = int(sys.argv[1])
    goal = sys.argv[2]
    file_name = sys.argv[3]
    X = read_input(file_name)

    # Convert X to a list of lists
    x_list = convert_to_list(X)

    n = len(x_list)  # Get the number of data points
    d = len(x_list[0])  # Get the dimensionality of the data points

    if goal == "symnmf":
        W = symnmf.fit(x_list, d, n)  # Call the wrapped function with the list

        # Initialize H
        m = np.mean(W)
        np.random.seed(0)
        H = np.random.uniform(0, 2 * np.sqrt(m / k), size=(n, k))  


        W = np.array(W, dtype=np.float64)  # Convert W to a NumPy array
        H = np.array(H, dtype=np.float64)  # Convert H to a NumPy array
        
        # Constants for the update rule
        beta = 0.5
        epsilon = 1e-4  # Convergence threshold
        # Iterative update of H
        converged = False
        max_iterations = 300  # You can adjust this
        iteration = 0

        W = np.array(W)  # Convert W to a NumPy array   

        while not converged and iteration < max_iterations:
            WH = np.dot(W, H)
            H_new = H * (1-beta+beta*((WH) / (np.dot(np.dot(H, H.T), H))) )
            
            # Check for convergence
            if np.linalg.norm(H_new - H, 'fro') < epsilon:
                converged = True
            else:
                H =  H_new
            iteration += 1
    elif goal == "sym":
        symnmf.sym(x_list, d, n)  # Call the C function via the wrapper
    elif goal == "ddg":
        symnmf.ddg(x_list, d, n)  # Call the C function via the wrapper
    elif goal == "norm":
        symnmf.norm(x_list, d, n)
    
    # H_np = np.array(H)
    print_matrix(W)
    print ("shasi")
    print_matrix(H)

