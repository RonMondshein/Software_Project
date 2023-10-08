import mysymnmf as symnmf
import sys
import pandas as pd
import numpy as np
import math

###############################################
#global variables
###############################################
args = sys.argv
np.random.seed(0)
k = 0
goal = ''
file_name = ''
N = 0
df = []
###############################################
#functions
###############################################
# function that print error message and exit
def printError():
    print("An Error Has Occurred")
    exit(1)

# function that check the inputs
def initialization():
    if len(args) != 4:
        printError()

    # recieve command line arguments
    k = args[1]
    # check if k is a positive integer if not print error message and exit
    try:
        k = int(k)
        if k < 1:
            printError()
    except ValueError:
        printError() 

    goal = args[2]
    # check if goal is either 'symnmf' or 'sym' or 'ddg' or 'norm', if not print error message and exit
    if goal != 'symnmf' and goal != 'sym' and goal != 'ddg' and goal != 'norm':
        printError()

    file_name = args[3]
    # check if file_name is end with .txt, if not print error message and exit
    if file_name[-4:] != '.txt':
        printError()

    # Initialize an empty list to store the data
    df = pd.read_csv(file_name, header=None)
    #number of line in the file
    N = len(df)
    #check if k is greater than N, if so print error message and exit
    if k >= N:
        printError() 
    return k, goal, df

# Initialize H
def initializeH(W, k, N):
    m = np.mean(W)
    init_H = np.random.uniform(0, 2 * math.sqrt(m/k), size=(N, k))
    return init_H

# Print the result
def printResult(arr):
    for row in arr:
        row_str = ",".join("{:.4f}".format(value) for value in row)
        print(row_str, end='\n')
        
def printResultForAnalysis(arr):
    for row in arr:
        row_str = ",".join("{}".format(value) for value in row)
        print(row_str, end='\n')

def analysis(k, filename):
    try:
        k = int(k)
        if k < 1:
            printError()
    except ValueError:
        printError() 

    if filename[-4:] != '.txt':
        printError()

    df = pd.read_csv(filename, header=None)
    N = len(df)
    if k >= N:
        printError()
    df_C = df.to_numpy().tolist() 
    df_rows, df_columns = df.shape
    W = symnmf.fit_norm(df_C)
    init_H = initializeH(W, k, df_rows)
    H = symnmf.fit_symnmf(init_H.tolist(), W)
    return H

    
###############################################
#main
###############################################
if __name__ == "__main__":
    k, goal, df = initialization()
    df_rows, df_columns = df.shape

    df_C = df.to_numpy().tolist() 
    if goal == 'symnmf':
        W = symnmf.fit_norm(df_C)
        init_H = initializeH(W, k, df_rows)
        H = symnmf.fit_symnmf(init_H.tolist(), W)
        printResult(H)
    elif goal == 'sym':
        A = symnmf.fit_sym(df_C)
        printResult(A)
    elif goal == 'ddg':
        D = symnmf.fit_ddg(df_C)
        printResult(D)  
    elif goal == 'norm':
        W = symnmf.fit_norm(df_C)
        printResult(W)
