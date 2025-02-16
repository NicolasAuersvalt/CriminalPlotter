import os

def remover_arquivos_txt(repositorio):
    if not os.path.exists(repositorio):
        print(f"O diretório '{repositorio}' não existe.")
        return
    
    print(f"Removendo arquivos .txt do diretório: {repositorio}")
    arquivos = [f for f in os.listdir(repositorio) if f.endswith(".txt")]
    
    if not arquivos:
        print("Nenhum arquivo .txt encontrado para remover.")
    else:
        for arquivo in arquivos:
            caminho_arquivo = os.path.join(repositorio, arquivo)
            try:
                os.remove(caminho_arquivo)
                print(f"Arquivo removido: {arquivo}")
            except Exception as e:
                print(f"Erro ao remover o arquivo {arquivo}: {e}")
    
    # Remover o arquivo matriz_resultante.txt, se existir
    caminho_matriz_resultante = os.path.join(repositorio, "matriz_resultante.txt")
    if os.path.exists(caminho_matriz_resultante):
        try:
            os.remove(caminho_matriz_resultante)
            print(f"Arquivo matriz_resultante.txt removido com sucesso.")
        except Exception as e:
            print(f"Erro ao remover matriz_resultante.txt: {e}")

    print("Processo de remoção concluído.")

# Exemplo de uso:
remover_arquivos_txt("impressao")
