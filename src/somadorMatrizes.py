import os
import numpy as np

def somar_matrizes(repositorio):
    if not os.path.exists(repositorio):
        print(f"O diretório '{repositorio}' não existe.")
        return
    
    print(f"Lendo matrizes do diretório: {repositorio}")
    arquivos = [f for f in os.listdir(repositorio) if f.endswith(".txt")]
    
    if not arquivos:
        print("Nenhuma matriz encontrada.")
        return
    
    matriz_soma = None
    tamanho_base = None
    
    for arquivo in arquivos:
        caminho_arquivo = os.path.join(repositorio, arquivo)
        matriz = []
        
        with open(caminho_arquivo, "r") as f:
            for linha in f:
                matriz.append([int(char) for char in linha.strip()])
        
        matriz = np.array(matriz)
        
        if matriz_soma is None:
            matriz_soma = matriz
            tamanho_base = matriz.shape
        elif matriz.shape == tamanho_base:
            matriz_soma = np.minimum(matriz_soma + matriz, 1)  # Garante que valores não ultrapassem 1
        else:
            print(f"Matriz {arquivo} ignorada devido a tamanho diferente: {matriz.shape}")
    
    # Removido o trecho que exclui os arquivos:
    # for arquivo in arquivos:
    #     os.remove(os.path.join(repositorio, arquivo))
    
    caminho_resultado = os.path.join(repositorio, "matriz_resultante.txt")
    with open(caminho_resultado, "w") as f:
        for linha in matriz_soma:
            f.write("".join(map(str, linha)) + "\n")
    
    if os.path.exists(caminho_resultado):
        print(f"Matriz resultante salva com sucesso em {caminho_resultado}")
    else:
        print("Erro ao salvar a matriz resultante.")

# Exemplo de uso:
somar_matrizes("impressao")
