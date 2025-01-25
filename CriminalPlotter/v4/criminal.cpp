#include <Keypad.h>

const byte LINHAS = 3; // Número de linhas
const byte COLUNAS = 3; // Número de colunas

int i;
int motorX[] = {2, 3, 4, 5};
int motorY[] = {6, 7, 8, 9};
int motorZ[] = {10, 11, 12, 13};

int xAtual = 0, yAtual = 0;
bool horario = false;

const int velocidade = 97;
const int passo = 20; // 2mm de distância entre os passos
const int passoEvento = 7;
const int passoZ = 50;
const int tmp = 100;
const int giros_inicioZ = 50;
const int motorDelay = 100 - velocidade;
const int giros_inicio = 1300;
const int tmpEvento = 10;

// Teclas -> COLOCAR VETOR
char combinacao[4];



char teclas[LINHAS][COLUNAS] = {
  {'1', '2', '3'},
  {'4', '5', '6'},
  {'*', '0', '#'}
};

byte pinosLinhas[LINHAS] = {A5, A4, A3}; // Conecte as linhas do teclado aqui
byte pinosColunas[COLUNAS] = {A2, A1, A0};   // Conecte as colunas do teclado aqui

Keypad teclado = Keypad(makeKeymap(teclas), pinosLinhas, pinosColunas, LINHAS, COLUNAS);

void setup()
{

  Serial.begin(9600);
  Serial.setTimeout(5000);  // Configura o timeout de leitura para 10 segundos (10000 ms)

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
  bool primBuffer = true;
  Serial.println("Pronto");
  // while o python enviar valores para x e y
  Serial.print(combinacao[0]);
  Serial.print(combinacao[2]);

  sinalEvento(1);
  bool primeiroEvento = true;
  
  // Enquanto houver dados
  while (Serial.available() >= 4)
  {
    if(primeiroEvento == false){
      Serial.println("Pronto");
    }
    
    sinalEvento(1);
    
    byte x_bytes[2];
    byte y_bytes[2];

    // Lê os 2 bytes para x e 2 bytes para y
    x_bytes[0] = Serial.read();
    x_bytes[1] = Serial.read();
    y_bytes[0] = Serial.read();
    y_bytes[1] = Serial.read();

    // Converte os bytes para inteiros (considerando a ordem big-endian)
    int x = (x_bytes[0] << 8) | x_bytes[1];  // Concatena os dois bytes de x
    int y = (y_bytes[0] << 8) | y_bytes[1];  // Concatena os dois bytes de y

    // Horário
    if (xAtual <= x)
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

    if (yAtual >= y)
    {

      horario = true;
      mover(passo * (y - yAtual), motorY); // em x
    }

    xAtual = x;
    yAtual = y;
    // Antihorário

    // Sempre mover eixo z
    canetar(passoZ);
    primeiroEvento = false;
    
  }
  //Serial.println("Terminou o desenho");
}

void mover(int giro, int motor[])
{
  
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
}

void teclar(int etapa) {
  //Serial.println("Digite...");

  bool pressionada = false;

  while (!pressionada) {

    char tecla = teclado.getKey();

    if (tecla) {
      //Serial.println(tecla);

      combinacao[etapa] = tecla;
      pressionada = true;
    }
  }
}



void inicializador()
{
  //Serial.println("Inicializador");
  xAtual = 0;
  yAtual = 0;

  horario = false;

  for (int i=0; i<3; i++){
    combinacao[i] = 'L';
  }

  //Serial.println("Inicializador OK");
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

void inicializarMotores(){
  horario = true;
  mover(giros_inicio, motorX); // Desce
  delay(tmp);
  horario = false;
  mover(giros_inicio, motorY); // Sobe
  delay(tmp);
  horario = true;
  mover(100, motorZ); // Desce
  delay(tmp);
  }

void prim_caract() {
  while (true) {
    //Serial.println("Caracteristica");
    teclar(0);

    if (combinacao[0] != '#' && combinacao[0] != '*' && combinacao[0] != 'L') {
      sinalEvento(1);

      if (confirmacao(1)) {

        seg_caract();
        return;

      }
    }
    if(combinacao[0] == '#'){
      inicializarMotores();  
     }
  }
}
bool confirmacao(int etapa) {
  while (true) {

    //Serial.println("Confimacao: | * SIM | # NAO |");
    teclar(etapa);

    if (combinacao[etapa] == '*' && combinacao[etapa] != 'L') {
      sinalEvento(1);

      return true;

    } 
    else if (combinacao[etapa] == '#') {

      combinacao[etapa] = 'L';
      combinacao[etapa - 1] = 'L';
      
      sinalEvento(2);
      return false;

    }
  }
}

void seg_caract() {
  while (true) {

    //Serial.println("Formato");
    teclar(2);

    if (combinacao[2] != '#' && combinacao[2] != '*' && combinacao[2] != 'L') {
      sinalEvento(1);
      if (confirmacao(3)) {

        desenhar();
        return;

      }
    }
  }
}

void sinalEvento(int evento){

  if(evento == 1){
    horario = true;
    mover(passoEvento, motorX); // Desce
    delay(tmpEvento);
    
    horario = false;
    mover(passoEvento, motorY); // Sobe
    delay(tmpEvento);
    
    horario = true;
    mover(passoEvento, motorZ); // Desce
    delay(tmpEvento);
  }
  else if (evento == 2){
    horario = true;
    mover(passoEvento, motorZ); // Desce
    delay(tmpEvento);

    horario = false;
    mover(passoEvento, motorY); // Sobe
    delay(tmpEvento);

    horario = true;
    mover(passoEvento, motorX); // Desce
    delay(tmpEvento);
   }
  
  horario = false;
}

void loop()
{

  inicializador();
  sinalEvento(1);
  prim_caract();

}