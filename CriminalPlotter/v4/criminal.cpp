// Definição dos pinos do teclado 3x3
#define ROW1 A0
#define ROW2 A1
#define ROW4 A2
#define COL1 A3
#define COL2 A4
#define COL3 A5

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
char primeira = 'L';
char segunda = 'L';


void setup()
{

	Serial.begin(9600);

	for (i = 0; i < 4; i++)
	{
		pinMode(motorX[i], OUTPUT); // Configura os pinos como saída
		pinMode(motorY[i], OUTPUT); // Configura os pinos como saída
		pinMode(motorZ[i], OUTPUT); // Configura os pinos como saída
	}

	// Configura as linhas e colunas como entradas com pull-up
	pinMode(ROW1, INPUT_PULLUP);
	pinMode(ROW2, INPUT_PULLUP);
	pinMode(ROW4, INPUT_PULLUP);
	pinMode(COL1, INPUT_PULLUP);
	pinMode(COL2, INPUT_PULLUP);
	pinMode(COL3, INPUT_PULLUP);
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


void limpar(bool priString){
	if(priString){
		primeira =  'L';
	}
	else{
		segunda = 'L';
	}
}

void desenhar(int prim, int seg)
{

	// while o python enviar valores para x e y
	Serial.print(prim);
	Serial.print(seg);
	Serial.print("Pronto");

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
	}
	// Fala para o python que está pronto para receber dados
	Serial.print("OK");
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

char teclado()
{

	char tecla; 
	bool pressionada = false;

	while (!pressionada) {
		// Verifica se a linha 1 foi pressionada
		if (digitalRead(ROW1) == LOW) {
			if (digitalRead(COL1) == LOW) {
				tecla = '1';
			}
			else if (digitalRead(COL2) == LOW) {
				tecla = '2';
			}
			else if (digitalRead(COL3) == LOW) {
				tecla = '3';
			}
			pressionada = true;
		}
		// Verifica se a linha 2 foi pressionada
		else if (digitalRead(ROW2) == LOW) {
			if (digitalRead(COL1) == LOW) {
				tecla = '4';
			}
			else if (digitalRead(COL2) == LOW) {
				tecla = '5';
			}
			else if (digitalRead(COL3) == LOW) {
				tecla = '6';
			}
			pressionada = true;
		}
		// Verifica se a linha 4 foi pressionada
		else if (digitalRead(ROW4) == LOW) {
			if (digitalRead(COL1) == LOW) {
				tecla = '7'; // Caso para a tecla 7, se necessário
			}
			else if (digitalRead(COL2) == LOW) {
				tecla = '0'; // Caso para a tecla 0
			}
			else if (digitalRead(COL3) == LOW) {
				tecla = '8'; // Caso para a tecla 8, se necessário
			}
			pressionada = true;
		}
	}

	return tecla;
}


void inicializador()
{
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

void encadeamento() {

	while (true) {

		bool proximo = false;
		char primeira, segunda;  // Declaração das variáveis de entrada

		// Espera a leitura do keypad
		primeira = teclado();

		if (primeira == '#') { // TERMINA A IMPRESSÃO
			return;
		}

		if (primeira == '*') { // USUARIO TECLOU *
			proximo = true;
		}

		if (proximo) { // Quando o usuário teclar *

			while (true) { // A pessoa pode querer voltar para a primeira característica

				segunda = teclado();

				if (segunda == '*') { // USUARIO TECLOU *
					desenhar(primeira, segunda);
					break;
				}
				else if (segunda == '#') { // USUARIO TECLOU #
					limpar(segunda); // LIMPA A TECLA DIGITADA
				}
			}

			// Limpa as teclas
			limpar(primeira);
			limpar(segunda);

		} else if (primeira == '#') {
			limpar(primeira);
		}
	}
}

void principal()
{
	inicializador();
	encadeamento();
}

void loop()
{

	principal();

}
