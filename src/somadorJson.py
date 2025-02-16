import os
import numpy as np
import json

def somar_matrizes_com_json(diretorio_matrizes, diretorio_banco, arquivo_json):
    """
    Lê as matrizes indicadas no arquivo JSON, busca essas matrizes no diretório,
    soma-as e salva a matriz resultante no diretório de destino.
    
    Args:
        diretorio_matrizes (str): Diretório onde as matrizes .txt estão localizadas.
        diretorio_banco (str): Diretório onde a matriz resultante será salva.
        arquivo_json (str): Caminho para o arquivo JSON com os nomes das matrizes.
    """
    if not os.path.exists(diretorio_matrizes):
        print(f"O diretório '{diretorio_matrizes}' não existe.")
        return
    
    if not os.path.exists(diretorio_banco):
        os.makedirs(diretorio_banco)  # Cria o diretório caso não exista

    # Carrega os nomes das matrizes do JSON
    if not os.path.exists(arquivo_json):
        print(f"O arquivo JSON '{arquivo_json}' não foi encontrado.")
        return

    with open(arquivo_json, "r") as f:
        matrices_metadata = json.load(f)

    print(f"Lendo matrizes do JSON: {arquivo_json}")
    
    # Matriz resultante para somar todas as matrizes
    matriz_soma = None
    tamanho_base = None

    for matrix_name in matrices_metadata.keys():
        arquivo_matriz = f"{matrix_name}.txt"
        caminho_arquivo = os.path.join(diretorio_matrizes, arquivo_matriz)

        if not os.path.exists(caminho_arquivo):
            print(f"Matriz {arquivo_matriz} não encontrada no diretório.")
            continue
        
        # Lê a matriz do arquivo .txt
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
            print(f"Matriz {arquivo_matriz} ignorada devido a tamanho diferente: {matriz.shape}")
    
    # Se a soma foi realizada, salva a matriz resultante
    if matriz_soma is not None:
        caminho_resultado = os.path.join(diretorio_banco, "matriz_resultante.txt")
        with open(caminho_resultado, "w") as f:
            for linha in matriz_soma:
                f.write("".join(map(str, linha)) + "\n")

        print(f"Matriz resultante salva com sucesso em {caminho_resultado}")
    else:
        print("Nenhuma matriz foi somada. Verifique os arquivos.")

# Exemplo de uso
diretorio_matrizes = "matrizes"  # Diretório onde as matrizes .txt estão
diretorio_banco = "impressao/bancoRetratos"  # Diretório onde a matriz resultante será salva
arquivo_json = "impressao/matrices_metadata.json"  # Caminho para o arquivo JSON com os nomes das matrizes

somar_matrizes_com_json(diretorio_matrizes, diretorio_banco, arquivo_json)
