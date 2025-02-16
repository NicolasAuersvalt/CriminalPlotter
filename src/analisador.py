import os
import numpy as np

def ler_matrizes(diretorio):
    """Lê todos os arquivos .txt em um diretório e retorna as matrizes como listas de listas."""
    matrizes = []
    for arquivo in os.listdir(diretorio):
        if arquivo.endswith('.txt'):
            caminho_arquivo = os.path.join(diretorio, arquivo)
            with open(caminho_arquivo, 'r') as f:
                # Lê a matriz do arquivo
                matriz = []
                for linha in f:
                    # Cada linha contém 180 caracteres '0' ou '1', sem espaçamento
                    matriz.append([int(x) for x in linha.strip()])
                matrizes.append(np.array(matriz))  # Adiciona a matriz ao conjunto
    return matrizes

def criar_matriz_resultante(matrizes):
    """Cria uma matriz resultante com o valor mais frequente em cada posição."""
    # Inicializa uma matriz de contagem com o mesmo tamanho das matrizes de entrada
    contagem = np.zeros_like(matrizes[0], dtype=int)
    
    # Contabiliza a quantidade de 0s e 1s em cada posição
    for matriz in matrizes:
        contagem += matriz
    
    # A matriz resultante será 1 onde os 1s aparecem mais, e 0 caso contrário
    resultante = (contagem >= len(matrizes) / 2).astype(int)
    
    return resultante

def salvar_matriz_resultante(resultante, caminho_saida):
    """Salva a matriz resultante em um arquivo .txt."""
    with open(caminho_saida, 'w') as f:
        for linha in resultante:
            f.write(''.join(map(str, linha)) + '\n')

# Caminho para o diretório onde estão as matrizes e onde o arquivo final será salvo
diretorio_matrizes = 'impressao/resultado'  # Modifique para o diretório correto
caminho_saida = 'impressao/resultado/matriz_resultante.txt'

# Passos principais
matrizes = ler_matrizes(diretorio_matrizes)
matriz_resultante = criar_matriz_resultante(matrizes)
salvar_matriz_resultante(matriz_resultante, caminho_saida)

print(f'Matriz resultante salva em: {caminho_saida}')
