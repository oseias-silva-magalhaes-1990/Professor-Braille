//ESP-8266 NodeMCU 1.0
#include <Bounce2.h>//Leitura dos botões incremento e decremento
#include "FirebaseESP8266.h"//Conexão com banco de dados firebase
#include <ESP8266WiFi.h>//Conexão wifi
//lib necessária para conectar o wifi
#include <WiFiUdp.h>//Biblioteca do UDP.
#include <WiFiManager.h>

#define FIREBASE_HOST "https://professorbrai-6a82f.firebaseio.com" //databaseURL fornecido pelo Firebase
#define FIREBASE_AUTH "HyY2lpqPVITY94sxJK***********************"

#define botao A0  //Botões de escolha Alfabeto, Números e Símbolos
#define inc 15   //Botão Incremento
#define dec 13   //Botão Decremento

Bounce debouncer1 = Bounce();//Botão incremento
Bounce debouncer2 = Bounce();//Botão decremento

int pino[] = {2, 16, 5, 4, 14, 12};//Pinos: 1,2,3,4,5,6

int opcao;//1-Alfabeto, 2-Numero, 3-Simbolo
int valor;//Armazena o valor lido na entrada analógica
int cont=0;//Contador para incremento e decremento
//Caracteres utilizados
char letras[50] = {'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H', 'I', 'J', 'K', 'L', 'M', 'N', 'O', 'P', 'Q', 'R', 'S', 'T', 'U', 'V', 'W', 'X', 'Y', 'Z', '.', ',', ';', ':', '/', '?', '!', '@', '#', '+', '-', '*', 'a', 'f', 'p', '<', '>', '(', ')', 'C', '_', 'E', 'L', 'N'};
//Define FirebaseESP8266 data objects
FirebaseData firebaseDado;

void conectaWifi() {
  //Conecta wifi
  //Serial.println("");
  //Serial.println("Conectando...");
  WiFi.mode(WIFI_STA);
  //WiFiManager
  //Depois de configurado a primeira vez o acesso ao WiFi e automatica
  WiFiManager gerenciadorWiFi;
  gerenciadorWiFi.setDebugOutput(false);
  if (!gerenciadorWiFi.autoConnect("Professor Braille")) {
    Serial.println("Falha na conexão com a WiFi");
    //reseta o ESP e tenta novamente
    ESP.restart();
    delay(1000);
  }
  //Mostra IP do servidor
  Serial.println();
  Serial.println("WiFi conectado");
  Serial.print("Endereço IP: ");
  Serial.println(WiFi.localIP());
  Serial.println("Use este endereço para conectar ao dispositivo");
  Serial.println();
}


void testaPinos(){
  for(int i=0; i<6; i++){
    digitalWrite(pino[i], LOW);
    delay(100);
    digitalWrite(pino[i], HIGH);
    delay(100);
  }
}

void setup() {
  Serial.begin(115200);

  conectaWiFi();

  Serial.println("CONECTANDO FIREBASE...");
  //Inicia a conexão com o Firebase
  Firebase.begin(FIREBASE_HOST, FIREBASE_AUTH);
  Firebase.reconnectWiFi(true);
  Serial.println("FIREBASE CONECTADO COM SUCESSO");
  
  debouncer1.attach(inc, INPUT_PULLUP); // Attach the debouncer to a BOB with INPUT_PULLUP mode
  debouncer1.interval(25); // Use a debounce interval of 25 milliseconds
  debouncer2.attach(dec, INPUT_PULLUP);
  debouncer2.interval(25); // Use a debounce interval of 25 milliseconds
  
  pinMode(botao, INPUT);   //Botão para a escolha de alfabeto número e símbolo
  //Pino para o controle lógico dos atuadores
  for(int i=0; i<6; i++){
    pinMode(pino[i], OUTPUT);   //Definindo pinos como saída
    digitalWrite(pino[i], HIGH);//Definindo pinos em nível alto como desligado
  }
  Firebase.setString(firebaseDado,"opcao", "");
  Firebase.setString(firebaseDado,"caractere", "");
  Firebase.setInt(firebaseDado,"contador", 0);
  testaPinos();
}

void loop() {
  valor = analogRead(botao);
  delay(25);
  verificaBotao();
  debouncer1.update(); // Realiza leitura do botão incrmento
  debouncer2.update(); // Realiza leitura do botão decremento
  controleIncDec();    //Incrementa os valores dentro da opção escolhida
}

void verificaOpcao(){
  String op = lerValorFirebase("opcao");
  for(int i=0; i<3; i++){
    if (op.charAt(i) == 'a'){
      opcao = 1;
    }
    if (op.charAt(i) == 'n'){
      opcao = 2;
    }
    if (op.charAt(i) == 's'){
      opcao = 3;
    }
  }
}

void verificaBotao(){
  if(valor > 100 and valor < 380){
    opcao = 1;    //Botão Alfabeto
    cont = 0;     //Caso o botao alfabeto seja pressionado cont recebe 1 equivalente a letra "a"
    Serial.println("OPÇÃO ALFABETO SELECIONADA");
    //isereValorFirebase("opcao", "a");
    Firebase.setString(firebaseDado,"opcao", "alf");
    //isereValorFirebase("caractere", "");
    Firebase.setString(firebaseDado,"caractere", "");
  }
  if(valor > 380 and valor < 700){
    opcao = 2;   //Botão Número
    cont = 0;    //Caso o botao alfabeto seja pressionado cont recebe 1 equivalente ao numero "1"
    Serial.println("OPÇÃO NÚMEROS SELECIONADA");
    //isereValorFirebase("opcao", "n");
    Firebase.setString(firebaseDado,"opcao", "num");
    //isereValorFirebase("caractere", "");
    Firebase.setString(firebaseDado,"caractere", "");
  }
  if(valor > 700 and valor <= 1024){
    opcao = 3;   //Botão Símbolo
    cont = 26;   //Caso o botao alfabeto seja pressionado cont recebe 27 equivalente ao símbolo "." ponto.
    Serial.println("OPÇÃO SÍMBOLOS SELECIONADA");
    //isereValorFirebase("opcao", "s");
    Firebase.setString(firebaseDado,"opcao", "sim");
    //isereValorFirebase("caractere", "");
    Firebase.setString(firebaseDado,"caractere", "");
  }
}

void controleIncDec() {//Controle de incremento e decremento
  if (debouncer1.fell() or debouncer2.fell()){
    verificaOpcao();
    Serial.println("opcao: " + opcao);
  }
  if (opcao == 1) {//Opcao Alfabeto
    if (debouncer1.fell()) {
      cont += 1;
      if (cont > 26)
        cont = 1;
      Serial.println(letras[cont - 1]);
      //isereValorFirebase("caractere", String(letras[cont - 1]));
      Firebase.setString(firebaseDado,"caractere", String(letras[cont - 1]));
      Firebase.setInt(firebaseDado,"contador", cont);
      mostraCaractere();   //Mostra o caractere correspondente
    }

    if (debouncer2.fell()) {
      cont -= 1;
      if (cont < 1)
        cont = 26;
      Serial.println(letras[cont - 1]);
      //isereValorFirebase("caractere", String(letras[cont - 1]));
      Firebase.setString(firebaseDado, "caractere", String(letras[cont - 1]));
      Firebase.setInt(firebaseDado,"contador", cont);
      mostraCaractere();   //Mostra o caractere correspondente
    }
  }

  if (opcao == 2) {//Opcao Numeros
    if (debouncer1.fell()) {
      cont += 1;
      if (cont > 10)
        cont = 1;
      if (cont < 10){
        Serial.println(cont);
        //isereValorFirebase("caractere", String(cont));
        Firebase.setInt(firebaseDado,"contador", cont);
        Firebase.setString(firebaseDado,"caractere", String(cont));
      }else{
        Serial.println('0');
        //isereValorFirebase("caractere", "0");
        Firebase.setInt(firebaseDado,"contador", cont);
        Firebase.setString(firebaseDado,"caractere", "0");
      }
      mostraCaractere();   //Mostra o caractere correspondente
    }

    if (debouncer2.fell()) {
      cont -= 1;
      if (cont < 1)
        cont = 10;
      if (cont < 10){
        Serial.println(cont);
        //isereValorFirebase("caractere", String(cont));
        Firebase.setInt(firebaseDado,"contador", cont);
        Firebase.setString(firebaseDado,"caractere", String(cont));
      }else{
        Serial.println('0');
        //isereValorFirebase("caractere", "0");
        Firebase.setInt(firebaseDado,"contador", cont);
        Firebase.setString(firebaseDado,"caractere", "0");
      }
      mostraCaractere();   //Mostra o caractere correspondente
    }
  }

  if (opcao == 3) {//Opcao Simbolos
    if (debouncer1.fell()) {
      cont += 1;
      if (cont > 50)
        cont = 27;
      Serial.println(letras[cont - 1]);
	    gravaSimbolo(letras[cont - 1]);
      mostraCaractere();   //Mostra o caractere correspondente
    }

    if (debouncer2.fell()) {
      cont -= 1;
      if (cont < 27)
        cont = 50;
      Serial.println(letras[cont - 1]);
	    gravaSimbolo(letras[cont - 1]);
      mostraCaractere();   //Mostra o caractere correspondente
    }
  }
}

void gravaSimbolo(char valor){
  Firebase.setInt(firebaseDado,"contador", cont);
  switch(valor){
	case '.':
		Firebase.setString(firebaseDado,"caractere", "ponto");
		break;
	case ',':
		Firebase.setString(firebaseDado,"caractere", "vírgula");
		break;
	case ';':
		Firebase.setString(firebaseDado,"caractere", "ponto_e_vírgula");
		break;
	case ':':
		Firebase.setString(firebaseDado,"caractere", "dois_pontos");
		break;
	case '/':
		Firebase.setString(firebaseDado,"caractere", "barra");
		break;
	case '?':
		Firebase.setString(firebaseDado,"caractere", "interrogação");
		break;
	case '!':
		Firebase.setString(firebaseDado,"caractere", "exclamação");
		break;
	case '@':
		Firebase.setString(firebaseDado,"caractere", "arroba");
		break;
	case '#':
		Firebase.setString(firebaseDado,"caractere", "cerquilha");
		break;
	case '+':
		Firebase.setString(firebaseDado,"caractere", "mais");
		break;
	case '-':
		Firebase.setString(firebaseDado,"caractere", "menos");
		break;
	case '*':
		Firebase.setString(firebaseDado,"caractere", "asterisco");
		break;
	case 'a':
		Firebase.setString(firebaseDado,"caractere", "abre_aspas");
		break;
	case 'f':
		Firebase.setString(firebaseDado,"caractere", "fecha_aspas");
		break;
	case 'p':
		Firebase.setString(firebaseDado,"caractere", "plicas");
		break;
	case '<':
		Firebase.setString(firebaseDado,"caractere", "menor");
		break;
	case '>':
		Firebase.setString(firebaseDado,"caractere", "maior");
		break;
	case '(':
		Firebase.setString(firebaseDado,"caractere", "abre_parêntese");
		break;
	case ')':
		Firebase.setString(firebaseDado,"caractere", "fecha_parêntese");
		break;
	case 'C':
		Firebase.setString(firebaseDado,"caractere", "capitular");
		break;
	case '_':
		Firebase.setString(firebaseDado,"caractere", "underline");
		break;
	case 'E':
		Firebase.setString(firebaseDado,"caractere", "e_lógico");
		break;
	case 'L':
		Firebase.setString(firebaseDado,"caractere", "letra");
		break;
	case 'N':
		Firebase.setString(firebaseDado,"caractere", "número");
		break;
	default:
		return;
  }
}
void mostraCaractere() {
  switch (cont) {
    case 1://A e 1
      ativacaoPinos(1, 0, 0, 0, 0, 0);//Liga apenas o pino 1
      break;

    case 2://B e 2
      ativacaoPinos(1, 1, 0, 0, 0, 0);//Liga apenas o pino 1 e 2
      break;

    case 3://C e 3
      ativacaoPinos(1, 0, 0, 1, 0, 0);//Liga apenas o pino 1 e 4
      break;

    case 4://D e 4
      ativacaoPinos(1, 0, 0, 1, 1, 0);//Liga apenas o pino 1, 4 e 5
      break;

    case 5://E e 5
      ativacaoPinos(1, 0, 0, 0, 1, 0);//Liga apenas o pino 1 e 5
      break;

    case 6://F e 6
      ativacaoPinos(1, 1, 0, 1, 0, 0);//Liga apenas o pino 1, 2 e 4
      break;

    case 7://G e 7
      ativacaoPinos(1, 1, 0, 1, 1, 0);//Liga apenas o pino 1, 2, 4 e 5
      break;

    case 8://H e 8
      ativacaoPinos(1, 1, 0, 0, 1, 0);//Liga apenas o pino 1, 2 e 4
      break;

    case 9://I e 9
      ativacaoPinos(0, 1, 0, 1, 0, 0);//Liga apenas o pino 2 e 4
      break;

    case 10://J e 0
      ativacaoPinos(0, 1, 0, 1, 1, 0);//Liga apenas o pino 2, 4 e 5
      break;

    case 11://K
      ativacaoPinos(1, 0, 1, 0, 0, 0);//Liga apenas o pino 1 e 3
      break;

    case 12://L
      ativacaoPinos(1, 1, 1, 0, 0, 0);//Liga apenas o pino 1, 2 e 3
      break;

    case 13://M
      ativacaoPinos(1, 0, 1, 1, 0, 0);//Liga apenas o pino 1, 3 e 4
      break;

    case 14://N
      ativacaoPinos(1, 0, 1, 1, 1, 0);//Liga apenas o pino 1, 3, 4 e 5
      break;

    case 15://O
      ativacaoPinos(1, 0, 1, 0, 1, 0);//Liga apenas o pino 1, 3 e 5
      break;

    case 16://P
      ativacaoPinos(1, 1, 1, 1, 0, 0);//Liga apenas o pino 1, 2, 3 e 4
      break;

    case 17://Q
      ativacaoPinos(1, 1, 1, 1, 1, 0);//Liga apenas o pino 1, 2, 3, 4 e 5
      break;

    case 18://R
      ativacaoPinos(1, 1, 1, 0, 1, 0);//Liga apenas o pino 1, 2, 3 e 5
      break;

    case 19://S
      ativacaoPinos(0, 1, 1, 1, 0, 0);//Liga apenas o pino 1 e 4
      break;

    case 20://T
      ativacaoPinos(0, 1, 1, 1, 1, 0);//Liga apenas o pino 2, 3, 4 e 5
      break;

    case 21://U
      ativacaoPinos(1, 0, 1, 0, 0, 1);//Liga apenas o pino 1, 3 e 6
      break;

    case 22://V
      ativacaoPinos(1, 1, 1, 0, 0, 1);//Liga apenas o pino 1, 2, 3 e 6
      break;

    case 23://W
      ativacaoPinos(0, 1, 0, 1, 1, 1);//Liga apenas o pino 2, 4, 5 e 6
      break;

    case 24://X
      ativacaoPinos(1, 0, 1, 1, 0, 1);//Liga apenas o pino 1, 3, 4 e 6
      break;

    case 25://Y
      ativacaoPinos(1, 0, 1, 1, 1, 1);//Liga apenas o pino 1, 3, 4, 5 e 6
      break;

    case 26://Z
      ativacaoPinos(1, 0, 1, 0, 1, 1);//Liga apenas o pino 3 e 6
      break;

    case 27://.
      ativacaoPinos(0, 1, 0, 0, 1, 1);//Liga apenas o pino 1, 3, 5 e 6
      break;

    case 28://,
      ativacaoPinos(0, 1, 0, 0, 0, 0);//Liga apenas o pino 2
      break;

    case 29://;
      ativacaoPinos(0, 1, 1, 0, 0, 0);//Liga apenas o pino 2 e 3
      break;

    case 30://:
      ativacaoPinos(0, 1, 0, 0, 1, 0);//Liga apenas o pino 2 e 5
      break;

    case 31:///
      ativacaoPinos(0, 0, 1, 1, 0, 0);//Liga apenas o pino 3 e 4
      break;

    case 32://?
      ativacaoPinos(0, 1, 1, 0, 0, 1);//Liga apenas o pino 2, 3 e 6
      break;

    case 33://!
      ativacaoPinos(0, 1, 1, 0, 1, 0);//Liga apenas o pino 2, 3 e 5
      break;

    case 34://@
      ativacaoPinos(0, 0, 1, 1, 1, 0);//Liga apenas o pino 3, 4 e 5
      break;

    case 35://#
      ativacaoPinos(0, 0, 1, 1, 1, 1);//Liga apenas o pino 3, 4, 5 e 6
      break;

    case 36://+
      ativacaoPinos(0, 1, 1, 0, 1, 0);//Liga apenas o pino 2, 3 e 5
      break;

    case 37://-
      ativacaoPinos(0, 1, 0, 0, 1, 0);//Liga apenas o pino 2 e 5
      break;

    case 38://*
      ativacaoPinos(0, 0, 1, 0, 1, 0);//Liga apenas o pino 3 e 5
      break;

    case 39://" Abre aspas
      ativacaoPinos(0, 1, 1, 0, 0, 1);//Liga apenas o pino 2, 3 e 6
      break;

    case 40://" Fecha aspas
      ativacaoPinos(0, 0, 1, 0, 1, 1);//Liga apenas o pino 3, 5 e 6
      break;

    case 41://´ plicas
      ativacaoPinos(0, 0, 1, 0, 0, 0);//Liga apenas o pino 3
      break;

    case 42://<
      ativacaoPinos(1, 1, 0, 0, 0, 1);//Liga apenas o pino 1, 2 e 6
      break;

    case 43://>
      ativacaoPinos(0, 0, 1, 1, 1, 0);//Liga apenas o pino 3, 4 e 5
      break;

    case 44://(
      ativacaoPinos(0, 1, 1, 0, 1, 1);//Liga apenas o pino 2, 3, 5 e 6
      break;

    case 45://)
      ativacaoPinos(0, 1, 1, 0, 1, 1);//Liga apenas o pino 2, 3, 5 e 6
      break;

    case 46://C capitular
      ativacaoPinos(0, 0, 0, 0, 0, 1);//Liga apenas o pino 6
      break;

    case 47://_
      ativacaoPinos(0, 0, 1, 0, 0, 1);//Liga apenas o pino 3 e 6
      break;

    case 48://E
      ativacaoPinos(1, 1, 1, 1, 0, 1);//Liga apenas o pino 1, 2, 3, 4 e 6
      break;

    case 49://L
      ativacaoPinos(0, 0, 0, 0, 1, 1);//Liga apenas o pino 5 e 6
      break;

    case 50://N
      ativacaoPinos(0, 0, 1, 1, 1, 1);//Liga apenas o pino 3, 4, 5 e 6
      break;
  }
}

void ativacaoPinos(int pino1, int pino2, int pino3, int pino4, int pino5, int pino6) {
  int estadoPino[] = {pino1, pino2, pino3, pino4, pino5, pino6};
  for (int i = 0; i < 6; i++) {
    if (estadoPino[i] == 1)
      digitalWrite(pino[i], LOW);
      delay(100);
      digitalWrite(pino[i], HIGH);
      delay(100);
  }
}

void isereValorFirebase(String campo, String valor) {
  //Muda o valor do campo no firebase
  if (Firebase.setString(firebaseDado, campo, valor))
  {
    //Success
    Serial.println("Valor do campo " + campo + " inserido com sucesso");
  } else {
    //Falha, obtém a razão do erro no firebaseData
    Serial.print("Erro no campo " + campo + ", ");
    Serial.println(firebaseDado.errorReason());
  }
}

String lerValorFirebase(String campo) {
  if (Firebase.get(firebaseDado, campo))
  {
    return firebaseDado.stringData();
  }
}
