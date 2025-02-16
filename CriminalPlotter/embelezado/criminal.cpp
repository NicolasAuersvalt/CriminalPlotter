#include <Keypad.h>

// Configurações de hardware
const byte LINHAS = 3;
const byte COLUNAS = 3;

int motorX[] = {2, 3, 4, 5};
int motorY[] = {6, 7, 8, 9};
int motorZ[] = {10, 11, 12, 13};

byte pinosLinhas[LINHAS] = {A5, A4, A3};
byte pinosColunas[COLUNAS] = {A2, A1, A0};

// Parâmetros de controle
const int VELOCIDADE = 97; // Velocidade padrão do Plotter (Ideal testada)
const int PASSO = 7;
const int PASSO_EVENTO = 7;
const int PASSO_Z = 20; 
const int TMP = 10; 
const int GIROS_INICIO_Z = 50; // Inicialização do motor Z
const int MOTOR_DELAY = 100 - VELOCIDADE; // delay entre os motores na função mover (Fundamental)
const int GIROS_INICIO = 1300; // Giros para inicializar os motores X e Y
const int TMP_EVENTO = 10; // Tempo de sinalização de evento
const int GIROS_SUBIDA = 10; // Giros de subida para cada canetada do motor Z

// Estado global
int xAtual = 0, yAtual = 0; // X e Y atual para a comparação com os recebidos pelo python
bool horario = false; // Define se deve girar horário ou não
char combinacao[6]; // vetor de combinações de teclas
// Exemplo: [1 * 2 * 0 *] -> Será procurado o arquivo 120.txt

// Mapeamento do teclado
char teclas[LINHAS][COLUNAS] = {
    {'1', '2', '3'},
    {'4', '5', '6'},
    {'*', '0', '#'}
};
Keypad teclado = Keypad(makeKeymap(teclas), pinosLinhas, pinosColunas, LINHAS, COLUNAS);

// Funções principais
void setup() {
    Serial.begin(9600);
    Serial.setTimeout(5000);

    for (int i = 0; i < 4; i++) {
        pinMode(motorX[i], OUTPUT);
        pinMode(motorY[i], OUTPUT);
        pinMode(motorZ[i], OUTPUT);
    }
}

void loop() {
    // Primeiramente inicializa algumas variáveis e o vetor combinação
    inicializador();

    // Emite um sinal positivo
    sinalEvento(1);

    // Inicia o encadeamento
    prim_caract();
}

// Funções auxiliares
void desenhar() {

    // Sinalizao ao python que está pronto para receber dados
    Serial.println("Pronto");
    Serial.print(combinacao[0]);
    Serial.print(combinacao[2]);
    Serial.print(combinacao[4]);

    sinalEvento(1);

    while (Serial.available() >= 4) {
        Serial.println("Pronto");

        byte x_bytes[2] = {Serial.read(), Serial.read()};
        byte y_bytes[2] = {Serial.read(), Serial.read()};

        int xPython = (x_bytes[0] << 8) | x_bytes[1];
        int yPython = (y_bytes[0] << 8) | y_bytes[1];

        moverEixo(xPython, motorX, xAtual);
        moverEixo(yPython, motorY, yAtual);

        xAtual = xPython;
        yAtual = yPython;

        canetar(PASSO_Z);
    }
}

void moverEixo(int destino, int motor[], int &posAtual) {
    if (posAtual < destino) {
        horario = false;
        mover(PASSO * (destino - posAtual), motor);
    } else if (posAtual > destino) {
        horario = true;
        mover(PASSO * (posAtual - destino), motor);
    }
    posAtual = destino;
}

void mover(int giro, int motor[]) {
    while (giro--) {
        for (int i = (horario ? 0 : 3); (horario ? i < 4 : i >= 0); i += (horario ? 1 : -1)) {
            digitalWrite(motor[i], HIGH);
            delay(MOTOR_DELAY);
            digitalWrite(motor[i], LOW);
        }
    }
}

void teclar(int etapa) {
    while (true) {
        char tecla = teclado.getKey();
        if (tecla) {
            combinacao[etapa] = tecla;
            Serial.print("Digitado: ");
            Serial.println(tecla);
            return;
        }
    }
}

void inicializador() {
    xAtual = 0;
    yAtual = 0;
    horario = false;
    memset(combinacao, 'L', sizeof(combinacao));
}

void canetar(int giros) {
    delay(TMP);
    horario = true;
    mover(giros, motorZ);
    delay(TMP);
    horario = false;
    mover(GIROS_SUBIDA, motorZ);
    delay(TMP);
}

void inicializarMotores() {
    mover(GIROS_INICIO, motorX);
    mover(GIROS_INICIO, motorY);
    mover(100, motorZ);
    horario = false;
    mover(100, motorZ);
}

void prim_caract() {
    while (true) {

        // Inicialmente, vai esperar um dígito numérico, # ou * para prosseguir
        Serial.println("Primeiro Digito");
        teclar(0);

        // Caso o usuário teclou numérico
        if (combinacao[0] != '#' && combinacao[0] != '*' && combinacao[0] != 'L') {

            // Sinaliza positivamente
            sinalEvento(1);

            // Usuário confirmou com *
            if (confirmacao(1)) {

                // Segunda parte do encadeamento
                seg_caract();
                return;
            }
        
        // Inicializa os motores
        } else if (combinacao[0] == '#') {
            inicializarMotores();
            inicializador();
        }
    }
}

bool confirmacao(int etapa) {
    while (true) {

        // Espera o usuário digitar * ou #
        teclar(etapa);
        if (combinacao[etapa] == '*') { // Confirmação
            sinalEvento(1);
            return true;
        } else if (combinacao[etapa] == '#') { // Cancelamento

            // Limpa as posições do vetor combinação para reiniciar tal etapa
            combinacao[etapa] = 'L';
            combinacao[etapa - 1] = 'L';

            // Sinaliza negativamente
            sinalEvento(2);
            return false;
        }
    }
}

void seg_caract() {
    Serial.println("Segundo Digito");
    while (true) {
        teclar(2);
        if (combinacao[2] != '#' && combinacao[2] != '*' && combinacao[2] != 'L') {
            sinalEvento(1);
            if (confirmacao(3)) {
                ter_caract();
                return;
            }
        }
    }
}

void ter_caract() {
    Serial.println("Terceiro Digito");
    while (true) {
        teclar(4);
        if (combinacao[4] != '#' && combinacao[4] != '*' && combinacao[4] != 'L') {
            sinalEvento(1);
            if (confirmacao(5)) {

                // Chama a função desenhar
                desenhar();
                return;
            }
        }
    }
}

void sinalEvento(int evento) {
    const int sequencia[][3] = {
        {motorX[0], motorY[0], motorZ[0]}, // Evento 1
        {motorZ[0], motorY[0], motorX[0]}  // Evento 2
    };

    for (int i = 0; i < 3; i++) {
        horario = (evento == 1);
        mover(PASSO_EVENTO, sequencia[evento - 1][i]);
        delay(TMP_EVENTO);
    }
}
