import numpy as np
import sys
import symnmf  # Importing the C extension

def read_input(file_name):
    return np.loadtxt(file_name, delimiter=',')

def convert_to_list(X):
    return X.tolist()

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
        symnmf.fit(x_list, d, n)  # Call the wrapped function with the list
    elif goal == "sym":
        symnmf.sym(x_list, d, n)  # Call the C function via the wrapper
    elif goal == "ddg":
        symnmf.ddg(x_list, d, n)  # Call the C function via the wrapper
    elif goal == "norm":
        symnmf.norm(x_list, d, n)

    # Free the C array memory if necessary
    # if goal == "symnmf":
    #     symnmf.free_c_array(c_array)
