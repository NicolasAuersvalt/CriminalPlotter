#include <Keypad.h>

const byte LINHAS = 3; // Número de linhas
const byte COLUNAS = 3; // Número de colunas

const int pot = A0;
int i;
int motorX[] = {2, 3, 4, 5};
int motorY[] = {6, 7, 8, 9};
int motorZ[] = {10, 11, 12, 13};

int xAtual, yAtual;
bool horario = false;

const int velocidade = 97;
const int passo = 20; // 2mm de distância entre os passos
const int passoZ = 50;
const int tmp = 100;
const int giros_inicioZ = 50;
const int motorDelay = 100 - velocidade;
const int giros_inicio = 1300;

// Teclas -> COLOCAR VETOR
char combinacao[4];



char teclas[LINHAS][COLUNAS] = {
  {'1', '2', '3'},
  {'4', '5', '6'},
  {'*', '0', '#'}
};

byte pinosLinhas[LINHAS] = {A0, A1, A2}; // Conecte as linhas do teclado aqui
byte pinosColunas[COLUNAS] = {A3, A4, A5};   // Conecte as colunas do teclado aqui

Keypad teclado = Keypad(makeKeymap(teclas), pinosLinhas, pinosColunas, LINHAS, COLUNAS);

void setup()
{

  Serial.begin(9600);

  for (i = 0; i < 4; i++)
  {
    pinMode(motorX[i], OUTPUT); // Configura os pinos como saída
    pinMode(motorY[i], OUTPUT); // Configura os pinos como saída
    pinMode(motorZ[i], OUTPUT); // Configura os pinos como saída
  }

}

void canetar(int giros)
{

  delay(tmp);

  horario = true;
  mover(giros, motorZ); // Desce
  delay(tmp);
  horario = false;
  mover(giros, motorZ); // Sobe
  delay(tmp);
}

void desenhar()
{

  // while o python enviar valores para x e y
  Serial.print(combinacao[0]);
  Serial.println(combinacao[2]);
  Serial.println("Pronto");

  // Enquanto houver dados
  while (Serial.available() >= 8)
  {

    int x = Serial.parseInt();
    int y = Serial.parseInt();

    // Horário
    if (xAtual < x)
    {
      horario = true;
      mover(passo * (x - xAtual), motorX); // em x
    }
    // AntiHorário
    else if (xAtual > x)
    {
      horario = false;
      mover(passo * (xAtual - x), motorX); // em x
    }

    // VERIFICAÇÃO NO EIXO Y

    if (yAtual > y)
    {

      horario = true;
      mover(passo * (y - yAtual), motorY); // em x
    }

    xAtual = x;
    yAtual = y;
    // Antihorário

    // Sempre mover eixo z
    canetar(passoZ);
    
    // Fala para o python que está pronto para receber dados
    Serial.println("OK");
  }
  Serial.println("Terminou o desenho");
}

void mover(int giro, int motor[])
{
  Serial.println("Mover");
  while (giro--)
  {

    if (horario == true)
    { // Aciona o motor no sentido Horário

      for (i = 0; i < 4; i++)
      { // Intercala o as bobinas acionadas

        digitalWrite(motor[i], HIGH); // Envia um pulso de um passo
        delay(motorDelay);
        digitalWrite(motor[i], LOW);
        // delay(motorDelay);
      }
    }

    else
    { // Aciona o motor no sentido Anti-Horário

      for (i = 3; i >= 0; i--)
      { // Intercala o as bobinas acionadas

        digitalWrite(motor[i], HIGH); // Envia um pulso de um passo
        delay(motorDelay);
        digitalWrite(motor[i], LOW);
        // delay(motorDelay);
      }
    }
  }
  Serial.println("Mover Concluido");
}

void teclar(int etapa) {
  Serial.println("Digite...");

  bool pressionada = false;

  while (!pressionada) {

    char tecla = teclado.getKey();

    if (tecla) {
      Serial.println(tecla);

      combinacao[etapa] = tecla;
      pressionada = true;
    }
  }
}



void inicializador()
{
  Serial.println("Inicializador");
  horario = true;
  mover(giros_inicio, motorX); // Desce
  delay(tmp);
  horario = false;
  mover(giros_inicio, motorY); // Sobe
  delay(tmp);
  horario = true;
  mover(100, motorZ); // Desce
  delay(tmp);

  xAtual = 0;
  yAtual = 0;

  horario = false;

  for (int i=0; i<3; i++){
    combinacao[i] = 'L';
  }

  Serial.println("Inicializador OK");
}

void canetar(int giros, int motorZ[])
{

  delay(tmp);
  horario = true;
  mover(giros, motorZ); // Desce
  delay(tmp);
  horario = false;
  mover(giros, motorZ); // Sobe
  delay(tmp);
}

void encadeamento(){
  prim_caract();
}

void prim_caract() {
  while (true) {
    Serial.println("Caracteristica");
    teclar(0);

    if (combinacao[0] != '#' && combinacao[0] != '*' && combinacao[0] != 'L') {

      if (confirmacao(1)) {

        seg_caract();
        return;

      }
    }
  }
}
bool confirmacao(int etapa) {
  while (true) {

    Serial.println("Confimacao: | * SIM | # NAO |");
    teclar(etapa);

    if (combinacao[etapa] == '*' && combinacao[etapa] != 'L') {

      return true;

    } 
    else if (combinacao[etapa] == '#') {

      combinacao[etapa] = 'L';
      combinacao[etapa - 1] = 'L';
      return false;

    }
  }
}

void seg_caract() {
  while (true) {

    Serial.println("Formato");
    teclar(2);

    if (combinacao[2] != '#' && combinacao[2] != '*' && combinacao[2] != 'L') {

      if (confirmacao(3)) {

        desenhar();
        return;

      }
    }
  }
}


void principal()
{
  inicializador();
  prim_caract();
  
}

void loop()
{

  principal();

}