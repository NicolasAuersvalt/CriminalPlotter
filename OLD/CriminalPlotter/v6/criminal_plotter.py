import serial
import os
import time

from collections import deque

queue = deque()  # Criação da fila (queue)


# Configura a porta serial (ajuste a porta conforme seu sistema)
# portaArduino = 'COM9'
portaArduino = '/dev/ttyUSB0'
ser = serial.Serial(portaArduino, 9600)  # Substitua 'COM9' pela sua porta serial
print(f"Conectado à porta {portaArduino} com baudrate 9600")
time.sleep(2)  # Aguarda o Arduino inicializar

def principal():
    while True:
        print("Esperando a mensagem 'Pronto' do Arduino...")

        try:
            # Lê a linha completa enviada pelo Arduino (espera "Pronto\n")
            pronto = ser.readline().decode('ascii', errors='ignore').strip()  # Remove espaços e '\n'
            print(f"Recebido do Arduino: {pronto}")
            if pronto == "Pronto":
                print(f"Mensagem 'Pronto' recebida: {pronto}")
                
                # Lê os dois caracteres enviados após "Pronto"
                char1 = ser.read(1).decode('ascii', errors='ignore').strip()  # Lê o primeiro caractere
                char2 = ser.read(1).decode('ascii', errors='ignore').strip()  # Lê o segundo caractere
                print(f"Lido do Arduino: char1 = {char1}, char2 = {char2}")
                
                # Converte os caracteres para inteiros
                int1 = int(char1)
                int2 = int(char2)
                print(f"Convertido para inteiros: int1 = {int1}, int2 = {int2}")
                
                # Acessa a matriz correspondente ao arquivo char1char2.txt
                file_name = f"{char1}{char2}.txt"
                file_path = os.path.join("matrizes", file_name)

                print(f"Verificando se o arquivo {file_name} existe...")
                if os.path.exists(file_path):
                    print(f"Lendo a matriz de {file_path}...")
                    matriz = []
                    try:
                        with open(file_path, "r") as file:
                            for line in file:
                                row = [int(char) for char in line.strip()]  # Cada caractere se torna um inteiro
                                matriz.append(row)
                        print(f"Matriz lida com sucesso.")
                    except Exception as e:
                        print(f"Erro ao ler o arquivo: {e}")

                    #Exibe a matriz lida
                    print("Matriz lida:")
                    for row in matriz:
                        print(row)

                    # Depuração
                    print("Conteúdo atual da matriz para depuração:")
                    for idx, row in enumerate(matriz):
                        print(f"Linha {idx}: {row}")

                    # Criação da stack de coordenadas (x, y)

                    for i in range(len(matriz)):  # Itera sobre as linhas
                        for j in range(len(matriz[i])):  # Itera sobre as colunas da linha atual
                            if matriz[i][j] == 0:
                                queue.append((j, i))
                    # print(f"Coordenadas na stack: {queue}")

                    size = len(queue)  # Obtém o tamanho inicial da fila
                    processed = 0  # Contador de itens processados


                    # Envia os pares da stack para o Arduino
                    while queue:
                        x, y = queue.popleft()

                        comando = f"{x} {y}\n"

                        processed += 1

                        progress = (processed / size) * 100


                        print(f"{progress:.2f}% concluído")

                        # Exibe a mensagem de depuração
                        print(f"Enviando coordenadas para o Arduino: {comando}")

                        # Envia dois inteiros como bytes (2 bytes para cada inteiro)
                        ser.write(x.to_bytes(2, 'big'))  # Converte x para 2 bytes e envia
                        ser.write(y.to_bytes(2, 'big'))  # Converte y para 2 bytes e envia

                        time.sleep(0.1)  # Pequeno delay

                        # Espera receber "OK" do Arduino (indica que terminou de imprimir)
                        ok = 'L'
                        while ok != 'Pronto':
                            time.sleep(0.1)  # Pequeno delay
                            ok = ser.readline().decode('ascii', errors='ignore').strip()  # Espera a linha completa de "OK"
                            print(f"Recebido do Arduino: {ok}")
                    print("Impressão Concluída")
                else:
                    print(f"Arquivo {file_name} não encontrado no diretório matrizes.")
        
        except Exception as e:
            print(f"Erro ao ler dados do Arduino: {e}")

if __name__ == "__main__":
    try:
        principal()
    except KeyboardInterrupt:
        print("Encerrando...")
    finally:
        ser.close()  # Fecha a conexão serial
        print("Conexão serial fechada.")
