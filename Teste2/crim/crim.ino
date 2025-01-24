
#include <deque>
#include <utility>

const int pot = A0;
int i;
int motorX[] = {2,3,4,5};
int motorY[] = {6,7,8,9};
int motorZ[] = {10,11,12,13};
int horario = 0;
int xAtual, yAtual;
bool horario = false;

const int velocidade = 97;
const int passo = 20;
const int passoZ = 50;
const int tmp = 100;
const int giros_inicioZ = 50;
const int motorDelay = 100 - velocidade;
const int giros_inicio = 1300;

// Cria a fila de posições
std::queue<std::pair<int, int>> filaPosicoes;

void setup() {

  
  Serial.begin(9600);

  for( i=0 ; i < 4 ; i++ ){
    pinMode(motorX[i],OUTPUT); // Configura os pinos como saída
    pinMode(motorY[i],OUTPUT); // Configura os pinos como saída
    pinMode(motorZ[i],OUTPUT); // Configura os pinos como saída
  }
}

void limparFila(){

  filaPosicoes.clear();

}

void mover(int giro, bool horario, int motor[]){

  while(giro--){
  
    if(horario == true){ // Aciona o motor no sentido Horário
      
      for(i = 0 ; i < 4 ; i++ ){  // Intercala o as bobinas acionadas
        
         digitalWrite(motor[i],HIGH); // Envia um pulso de um passo
         delay(motorDelay); 
         digitalWrite(motor[i],LOW);
         //delay(motorDelay);
      }
    }

    else{ // Aciona o motor no sentido Anti-Horário
      
      for(i = 3 ; i >= 0 ; i--){ // Intercala o as bobinas acionadas
        
         digitalWrite(motor[i],HIGH); // Envia um pulso de um passo
         delay(motorDelay);
         digitalWrite(motor[i],LOW); 
         //delay(motorDelay);
      }
    }
    
  }

}

void inicializador(){

    mover(giros_inicio, true, motorX); // Desce
    delay(tmp);
    mover(giros_inicio, false, motorY); // Sobe
    delay(tmp);
    mover(100, true, motorZ); // Desce
    delay(tmp);

    limparFila();
    
    xAtual = 0;
    yAtual = 0;
  
  }

void canetar(int giros, int motorZ[]){
    delay(tmp);
    mover(giros, true, motorZ); // Desce
    delay(tmp);
    mover(giros, false, motorZ); // Sobe
    delay(tmp);
}

void printarPos(){

    // Enquanto a fila não estiver vazia
    while(!filaPosicoes.empty()){

      std::pair<int, int> frente = filaDePares.front(); // Acessa o primeiro par

      filaDePares.pop(); // Remove o primeiro par


      // VERIFICAÇÃO NO EIXO X

      // Horário
      if(xAtual < filaDePares.first){
        horario = true;
        mover(passo * (filaDePares.first - xAtual), horario, motorX); // em x

      }
      // AntiHorário
      else if (xAtual > filaDePares.first){
        horario = false;
        mover(passo * (xAtual - filaDePares.first), horario, motorX); // em x

      }

      // VERIFICAÇÃO NO EIXO Y

      if(yAtual > filaDePares.second){

        horario = true;
        mover(passo * (filaDePares.second - yAtual), horario, motorY); // em x

      }

      xAtual = filaDePares.first;
      yAtual = filaDePares.second;
      // Antihorário

      // Sempre mover eixo z
      canetar();

    }

}


void loop() {

    inicializador();

    int matriz[10][10] = {
    {1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
    {1, 0, 0, 0, 0, 0, 0, 0, 0, 1},
    {1, 0, 0, 0, 0, 0, 0, 0, 0, 1},
    {1, 0, 0, 0, 0, 0, 0, 0, 0, 1},
    {1, 0, 0, 0, 0, 0, 0, 0, 0, 1},
    {1, 0, 0, 0, 0, 0, 0, 0, 0, 1},
    {1, 0, 0, 0, 0, 0, 0, 0, 0, 1},
    {1, 0, 0, 0, 0, 0, 0, 0, 0, 1},
    {1, 0, 0, 0, 0, 0, 0, 0, 0, 1},
    {1, 1, 1, 1, 1, 1, 1, 1, 1, 1}
  };

  

  horario = false;
  int sizeMat = 10;


  for (int i = 0; i < sizeMat; i++) {
    for (int j = 0; j < sizeMat; j++) {
      horario = false;
          
      if(matriz[i][j]==1){
        //canetar(passoZ, motorZ);
        filaDePares.push(std::make_pair(i, j));

      }

    //horario = false;
    //mover(passo, horario, motorX); // em x
  }

  printarPos();

  /*
  horario = true;
  mover(passo, horario, motorY); 

  horario = true;
  mover(passo * sizeMat, horario, motorX); 
  }

  horario = false;
  mover(passo * sizeMat, horario, motorY); 

  */

}
