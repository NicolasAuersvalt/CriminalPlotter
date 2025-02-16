import os
import numpy as np
from PIL import Image

def matriz_para_png_sequencial(caminho_arquivo, diretorio_pai):
    # Diretório de saída
    subdiretorio = os.path.join(diretorio_pai, "Banco de Retratos")
    os.makedirs(subdiretorio, exist_ok=True)  # Cria o subdiretório se não existir

    # Lista os arquivos existentes no subdiretório
    arquivos_existentes = [f for f in os.listdir(subdiretorio) if f.endswith(".png")]

    # Determina o próximo número da sequência
    numeros_existentes = []
    for arquivo in arquivos_existentes:
        nome, _ = os.path.splitext(arquivo)
        if nome.isdigit():  # Verifica se o nome do arquivo é numérico
            numeros_existentes.append(int(nome))
    proximo_numero = max(numeros_existentes, default=0) + 1  # Próximo número disponível

    # Define o caminho do próximo arquivo
    caminho_saida = os.path.join(subdiretorio, f"{proximo_numero}.png")

    # Lê a matriz do arquivo de texto
    matriz = []
    with open(caminho_arquivo, "r") as f:
        for linha in f:
            matriz.append([int(char) for char in linha.strip()])

    # Converte a matriz para imagem
    matriz = np.array(matriz, dtype=np.uint8) * 255  # Converte 1 -> 255 (branco) e 0 -> 0 (preto)
    matriz = 255 - matriz  # Inverte preto e branco
    imagem = Image.fromarray(matriz, mode='L')  # 'L' para escala de cinza
    imagem.save(caminho_saida)

    print(f"Imagem salva em {caminho_saida}")

# Exemplo de uso:
matriz_para_png_sequencial("impressao/resultado/matriz_resultante.txt", "impressao")
