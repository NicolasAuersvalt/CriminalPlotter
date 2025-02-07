import serial
import os
import shutil
import time

from collections import deque

queue = deque()  # Criação da fila (queue)


# Configura a porta serial (ajuste a porta conforme seu sistema)
# portaArduino = 'COM9'
portaArduino = '/dev/ttyUSB0'
ser = serial.Serial(portaArduino, 9600)  # Substitua 'COM9' pela sua porta serial
print(f"Conectado à porta {portaArduino} com baudrate 9600")
time.sleep(2)  # Aguarda o Arduino inicializar
pathMatrix = 'impressao'

def principal():
    while True:
        print("Esperando a mensagem 'Pronto' do Arduino...")

        try:
            pronto = ser.readline().decode('ascii', errors='ignore').strip()
            print(f"Recebido do Arduino: {pronto}")
            if pronto == "Pronto":
                print(f"Mensagem 'Pronto' recebida: {pronto}")
                
                char1 = ser.read(1).decode('ascii', errors='ignore').strip()
                char2 = ser.read(1).decode('ascii', errors='ignore').strip()
                char3 = ser.read(1).decode('ascii', errors='ignore').strip()

                print(f"Lido do Arduino: char1 = {char1}, char2 = {char2}, char3 = {char3}")
                
                file_name = f"{char1}{char2}{char3}.txt"
                file_path = os.path.join("matrizes", file_name)

                print(f"Verificando se o arquivo {file_name} existe...")
                if os.path.exists(file_path):
                    print(f"Lendo a matriz de {file_path}...")
                    matriz = []
                    try:
                        with open(file_path, "r") as file:
                            for line in file:
                                row = [int(char) for char in line.strip()]
                                matriz.append(row)
                        print("Matriz lida com sucesso.")
                    except Exception as e:
                        print(f"Erro ao ler o arquivo: {e}")

                    # Fazendo a cópia do arquivo para o repositório
                    destino = os.path.join("impressao", file_name)
                    shutil.copy(file_path, destino)
                    print(f"Cópia da matriz salva em {destino}")
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
