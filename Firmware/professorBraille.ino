int bobsup[] = {8, 9, 10, 11, 12, 13};//Definição dos pinos para as bobinas superiores
int bobinf[] = {2, 3, 4, 5, 6, 7};//Definição dos pinos para as bobinas inferiores
int botao[] = {A0, A1, A2, A3, A4};//Definição do pinos para os botões:(incrementa,decrementa,Letras,Números,Simbolos)
int botaoPressionado[] = {0, 0, 0, 0, 0}; //Botão pressionado quando for 1 e não pressionado quando for 0(zero) e cada posição controla inc, dec, let, num e sim
int anterior[] = {0, 0, 0, 0, 0}; //Estado anterior do botão inc, dec, let, num e sim
boolean botLigado[] = {false, false, false}; //Se botão está ligado recebe true senão false cada posição controla let, num e sim
boolean zerou = false;//Abaixa todos os pinos
boolean mostrou = false;//Informa se o caractere foi mostrado para desligamento das bobinas
int cont = 0;//Contador para incrementar e decrementar os valores referents a cada caractere

void setup() {//Definição das entradas e saídas
  for (int i = 0; i < 6; i++) {//Laço define as bobinas como saídas e botões como entradas
    pinMode(bobsup[i], OUTPUT);//Bobina superior como saída
    pinMode(bobinf[i], OUTPUT);//Bobina inferior como saída
    if (i < 5) {
      pinMode(botao[i], INPUT);}//Botões como entradas
  }Serial.begin(9600); //Porta serial com leitura de 9600bauds/s
}

void loop() {//Looping de cada ciclo do processador
  for (int i = 0; i < 5; i++) {//Laço para a leitura dos botões
    botaoPressionado[i] = digitalRead(botao[i]);}//Se o botão i for pressionado a variável botaoPressionado[i] recebe valor 1
  ctrl_botoes();//Chama a função de controle dos botões de incremento e decremento
  ctrl_bot_char();//Chama a função de controle dos botões para escolher mostrar letra, número ou símbolo
  ctrl_letenum();//Chama a função de controle para mostrar letra ou número
  ctrl_sim();//Chama a função de controle para mostrar símbolo
}
void ctrl_botoes() {
//************controle do botao incrementa************
  if ((anterior[0] == LOW) && (botaoPressionado[0] == HIGH)) {
    cont++;//Incrementa o contador cont++
    zerou = false;//Zerou deve receber false para zerar ultimo caractere mostrado
    mostrou = false;//Mostrou recebe false para liberar ativação das bobinas superiores
    Serial.print(cont);}//Pode-se verificar o valor do contador via porta serial
  anterior[0] = botaoPressionado[0];//Passagem do valor atual do botão incremento
  delay(50);//Aguarda 50 milissegundos para evitar saltos na leitura e passagem de valores
//************controle do botao decrementa************
  if ((anterior[1] == LOW) && (botaoPressionado[1] == HIGH)) {
    cont--;//Decrementa o contador cont--
    zerou = false; //Zerou deve receber false para zerar ultimo caractere mostrado
    mostrou = false; //Mostrou recebe false para liberar ativação das bobinas superiores
    Serial.print(cont);} //Pode-se verificar o valor do contador via porta serial
  anterior[1] = botaoPressionado[1];
  delay(50); //Aguarda 50 milissegundos para evitar saltos na leitura e passagem de valores
}

void ctrl_bot_char() {
  if ((anterior[2] == LOW) && (botaoPressionado[2] == HIGH)) {//controle do botao letra
    cont = 1;//Se botão alfabeto foi pressionado então contador sempre deverá receber 1 que equivale a primeira letra do alfabeto
    zerou = false;//Deve-se zerar o ultimo caractere mostrado antes de mostrar o atual
    mostrou = false;//Deve-se ativar as bobinas superiores para mostrar o caractere 
    Serial.print(cont);//Informa via porta serial o valor de cont
    botLigado[0] = true;//Informa que botão alfabeto esta ligado
    botLigado[1] = false;//Informa que botão número está desligado
    botLigado[2] = false;}//Informa que botão símbolo está desligado
    anterior[2] = botaoPressionado[2];//Passa-se o valor atual do botaoPressionado para anterior para utilização posterior
  if ((anterior[3] == LOW) && (botaoPressionado[3] == HIGH)) {//controle do botao numero
    cont = 1;//cont deve receber 1 que é equivalente ao número 1 
    zerou = false;//Deve-se zerar o ultimo caractere mostrado antes de mostrar o atual
    mostrou = false;//Deve-se ativar as bobinas superiores para mostrar o caractere
    Serial.print(cont); //Informa via porta serial o valor de cont
    botLigado[0] = false;//Informa que o botão alfabeto está desligado
    botLigado[1] = true;//Informa que o botão número está ligado
    botLigado[2] = false;}//Informa que o botão símbolo está desligado
    anterior[3] = botaoPressionado[3];//Passa-se o valor atual do botaoPressionado para anterior para utilização posterior
  if ((anterior[4] == LOW) && (botaoPressionado[4] == HIGH)) {//controle do botao simbolo
    cont = 27;//cont recebe 27 que é equivalente ao ponto(.) que é o primeiro símbolo a ser mostrado
    zerou = false;//Deve-se zerar o ultimo caractere mostrado antes de mostrar o atual
    mostrou = false;//Deve-se ativar as bobinas superiores para mostrar o caractere
    Serial.print(cont);//Informa via porta serial o valor de cont
    botLigado[0] = false;//Informa que o botão alfabeto está desligado
    botLigado[1] = false;//Informa que o botão número está desligado 
    botLigado[2] = true;}//Informa que o botão símbolo esta ligado
  anterior[4] = botaoPressionado[4];//Passa-se o valor atual do botaoPressionado para anterior para utilização posterior
}

void ctrl_letenum() {//Controle de letra e numero
  if (botLigado[0] || botLigado[1]) {//Verifica condicional de letra ou numero
    if (cont < 1 && botLigado[0]) {//Gira da primeira para a ultima letra
      cont = 26;}
    if (cont > 26 && botLigado[0]) {//Gira da ultima para a primeira letra                    cont = 1 ;}
    if (cont < 1 && botLigado[1]) {//Gira do primeiro numero para o ultimo
      cont = 0;}
    if (cont > 0 && botLigado[1]) {//Gira do ultimo numero para o primeiro
      cont = 1 ;}
    if (!zerou) {//Sequência para abaixar todos os pinos
      for (int i = 0; i < 6; i++) {
        digitalWrite(bobinf[i], HIGH);}//Ativa as bobinas inferiores
      delay(100);//Agurada 100 milissegundos
      for (int i = 0; i < 6; i++) {
        digitalWrite(bobinf[i], LOW);}//Desativa as bobinas inferiores
      zerou = true;}//Confirma abaixamento dos pinos
    if (!mostrou) {//Sequência para mostrar o caractere
      switch (cont) {//Verifica valor de cont
        case 1://Mostra letra “a” ou número “1”
          digitalWrite(bobsup[0], HIGH);//Ativa bobina superior 1
          delay(100);//Aguarda 100 milissegundos
          digitalWrite(bobsup[0], LOW);//Desativa bobina superior 1
          break;
        case 2://Mostra letra “b” ou número “2”
          digitalWrite(bobsup[0], HIGH);
          digitalWrite(bobsup[1], HIGH);
          delay(100);
          digitalWrite(bobsup[0], LOW);
          digitalWrite(bobsup[1], LOW);
          break;
        case 3://Mostra letra “c” ou número “3”
          digitalWrite(bobsup[0], HIGH);//Ativa bobina superior 1
          digitalWrite(bobsup[3], HIGH);//Ativa bobina superior 2
          delay(100);//Aguarda 100 milissegundos
          digitalWrite(bobsup[0], LOW);//Desativa bobina superior 1
          digitalWrite(bobsup[3], LOW);//Desativa bobina superior 2
          break;
       //De maneira analoga os demais valores representarão uma sequência de caracteres a serem mostrados
        case 4://Mostra a letra “d” ou número “4”
          digitalWrite(bobsup[0], HIGH);
          digitalWrite(bobsup[3], HIGH);
          digitalWrite(bobsup[4], HIGH);
          delay(100);
          digitalWrite(bobsup[0], LOW);
          digitalWrite(bobsup[3], LOW);
          digitalWrite(bobsup[4], LOW);
          break;
        case 5://Mostra a letra “e” ou número “5”
          digitalWrite(bobsup[0], HIGH);
          digitalWrite(bobsup[4], HIGH);
          delay(100);
          digitalWrite(bobsup[0], LOW);
          digitalWrite(bobsup[4], LOW);
          break;
        case 6://Mostra a letra “f” ou número “6”
          digitalWrite(bobsup[0], HIGH);
          digitalWrite(bobsup[1], HIGH);
          digitalWrite(bobsup[3], HIGH);
          delay(100);
          digitalWrite(bobsup[0], LOW);
          digitalWrite(bobsup[1], LOW);
          digitalWrite(bobsup[3], LOW);
          break;
        case 7://Mostra letra “g” ou número “7”
          digitalWrite(bobsup[0], HIGH);
          digitalWrite(bobsup[1], HIGH);
          digitalWrite(bobsup[3], HIGH);
          digitalWrite(bobsup[4], HIGH);
          delay(100);
          digitalWrite(bobsup[0], LOW);
          digitalWrite(bobsup[1], LOW);
          digitalWrite(bobsup[3], LOW);
          digitalWrite(bobsup[4], LOW);
          break;
        case 8://Mostra a letra “h” ou número “8”
          digitalWrite(bobsup[0], HIGH);
          digitalWrite(bobsup[1], HIGH);
          digitalWrite(bobsup[4], HIGH);
          delay(100);
          digitalWrite(bobsup[0], LOW);
          digitalWrite(bobsup[1], LOW);
          digitalWrite(bobsup[4], LOW);
          break;
        case 9://Mostra a letra “i” ou número “9”
          digitalWrite(bobsup[1], HIGH);
          digitalWrite(bobsup[3], HIGH);
          delay(100);
          digitalWrite(bobsup[1], LOW);
          digitalWrite(bobsup[3], LOW);
          break;
        case 10://Mostra a letra “j” ou número “0”
          digitalWrite(bobsup[1], HIGH);
          digitalWrite(bobsup[3], HIGH);
          digitalWrite(bobsup[4], HIGH);
          delay(100);
          digitalWrite(bobsup[1], LOW);
          digitalWrite(bobsup[3], LOW);
          digitalWrite(bobsup[4], LOW);
          break;
        case 11://Mostra letra “k”
          digitalWrite(bobsup[0], HIGH);
          digitalWrite(bobsup[2], HIGH);
          delay(100);
          digitalWrite(bobsup[0], LOW);
          digitalWrite(bobsup[2], LOW);
          break;
        case 12://Mostra letra “l”
          digitalWrite(bobsup[0], HIGH);
          digitalWrite(bobsup[1], HIGH);
          digitalWrite(bobsup[2], HIGH);
          delay(100);
          digitalWrite(bobsup[0], LOW);
          digitalWrite(bobsup[1], LOW);
          digitalWrite(bobsup[2], LOW);
          break;
        case 13://Mostra letra “m”
          digitalWrite(bobsup[0], HIGH);
          digitalWrite(bobsup[2], HIGH);
          digitalWrite(bobsup[3], HIGH);
          delay(100);
          digitalWrite(bobsup[0], LOW);
          digitalWrite(bobsup[2], LOW);
          digitalWrite(bobsup[3], LOW);
          break;
        case 14://Mostra letra “n”
          digitalWrite(bobsup[0], HIGH);
          digitalWrite(bobsup[2], HIGH);
          digitalWrite(bobsup[3], HIGH);
          digitalWrite(bobsup[4], HIGH);
          delay(100);
          digitalWrite(bobsup[0], LOW);
          digitalWrite(bobsup[2], LOW);
          digitalWrite(bobsup[3], LOW);
          digitalWrite(bobsup[4], LOW);
          break;
        case 15://Mostra letra “o”
          digitalWrite(bobsup[0], HIGH);
          digitalWrite(bobsup[2], HIGH);
          digitalWrite(bobsup[4], HIGH);
          delay(100);
          digitalWrite(bobsup[0], LOW);
          digitalWrite(bobsup[2], LOW);
          digitalWrite(bobsup[4], LOW);
          break;
        case 16://Mostra letra “p”
          digitalWrite(bobsup[0], HIGH);
          digitalWrite(bobsup[1], HIGH);
          digitalWrite(bobsup[2], HIGH);
          digitalWrite(bobsup[3], HIGH);
          delay(100);
          digitalWrite(bobsup[0], LOW);
          digitalWrite(bobsup[1], LOW);
          digitalWrite(bobsup[2], LOW);
          digitalWrite(bobsup[3], LOW);
          break;
        case 17://Mostra letra “q”
          digitalWrite(bobsup[0], HIGH);
          digitalWrite(bobsup[1], HIGH);
          digitalWrite(bobsup[2], HIGH);
          digitalWrite(bobsup[3], HIGH);
          digitalWrite(bobsup[4], HIGH);
          delay(100);
          digitalWrite(bobsup[0], LOW);
          digitalWrite(bobsup[1], LOW);
          digitalWrite(bobsup[2], LOW);
          digitalWrite(bobsup[3], LOW);
          digitalWrite(bobsup[4], LOW);
          break;
        case 18://Mostra letra “r”
          digitalWrite(bobsup[0], HIGH);
          digitalWrite(bobsup[1], HIGH);
          digitalWrite(bobsup[2], HIGH);
          digitalWrite(bobsup[4], HIGH);
          delay(100);
          digitalWrite(bobsup[0], LOW);
          digitalWrite(bobsup[1], LOW);
          digitalWrite(bobsup[2], LOW);
          digitalWrite(bobsup[4], LOW);
          break;
        case 19://Mostra letra “s”
          digitalWrite(bobsup[1], HIGH);
          digitalWrite(bobsup[2], HIGH);
          digitalWrite(bobsup[3], HIGH);
          delay(100);
          digitalWrite(bobsup[0], LOW);
          digitalWrite(bobsup[2], LOW);
          digitalWrite(bobsup[3], LOW);
          break;
        case 20://Mostra letra “t”
          digitalWrite(bobsup[1], HIGH);
          digitalWrite(bobsup[2], HIGH);
          digitalWrite(bobsup[3], HIGH);
          digitalWrite(bobsup[4], HIGH);
          delay(100);
          digitalWrite(bobsup[1], LOW);
          digitalWrite(bobsup[2], LOW);
          digitalWrite(bobsup[3], LOW);
          digitalWrite(bobsup[4], LOW);
          break;
        case 21://Mostra letra “u”
          digitalWrite(bobsup[0], HIGH);
          digitalWrite(bobsup[2], HIGH);
          digitalWrite(bobsup[5], HIGH);
          delay(100);
          digitalWrite(bobsup[0], LOW);
          digitalWrite(bobsup[2], LOW);
          digitalWrite(bobsup[5], LOW);
          break;
        case 22://Mostra letra “v”
          digitalWrite(bobsup[0], HIGH);
          digitalWrite(bobsup[1], HIGH);
          digitalWrite(bobsup[2], HIGH);
          digitalWrite(bobsup[5], HIGH);
          delay(100);
          digitalWrite(bobsup[0], LOW);
          digitalWrite(bobsup[1], LOW);
          digitalWrite(bobsup[2], LOW);
          digitalWrite(bobsup[5], LOW);
          break;
        case 23://Mostra letra “w”
          digitalWrite(bobsup[1], HIGH);
          digitalWrite(bobsup[3], HIGH);
          digitalWrite(bobsup[4], HIGH);
          digitalWrite(bobsup[5], HIGH);
          delay(100);
          digitalWrite(bobsup[1], LOW);
          digitalWrite(bobsup[3], LOW);
          digitalWrite(bobsup[4], LOW);
          digitalWrite(bobsup[5], LOW);
          break;
        case 24://Mostra letra “x”
          digitalWrite(bobsup[0], HIGH);
          digitalWrite(bobsup[2], HIGH);
          digitalWrite(bobsup[3], HIGH);
          digitalWrite(bobsup[5], HIGH);
          delay(100);
          digitalWrite(bobsup[0], LOW);
          digitalWrite(bobsup[2], LOW);
          digitalWrite(bobsup[3], LOW);
          digitalWrite(bobsup[5], LOW);
          break;
        case 25://Mostra letra “y”
          digitalWrite(bobsup[0], HIGH);
          digitalWrite(bobsup[2], HIGH);
          digitalWrite(bobsup[3], HIGH);
          digitalWrite(bobsup[4], HIGH);
          digitalWrite(bobsup[5], HIGH);
          delay(100);
          digitalWrite(bobsup[0], LOW);
          digitalWrite(bobsup[2], LOW);
          digitalWrite(bobsup[3], LOW);
          digitalWrite(bobsup[4], LOW);
          digitalWrite(bobsup[5], LOW);
          break;
        case 26://Mostra letra “z”
          digitalWrite(bobsup[0], HIGH);
          digitalWrite(bobsup[2], HIGH);
          digitalWrite(bobsup[4], HIGH);
          digitalWrite(bobsup[5], HIGH);
          delay(100);
          digitalWrite(bobsup[0], LOW);
          digitalWrite(bobsup[2], LOW);
          digitalWrite(bobsup[4], LOW);
          digitalWrite(bobsup[5], LOW);
          break;
        default:
          cont = 0;//Para qualquer valor diferente do esperado cont recebe 0 e abaixa todos os piinos
      } mostrou = true;//Informa que foi mostrado o caractere esperado
    }
  }
}

void ctrl_sim() {//Controle de símbolo de pontuação textual
  if (botLigado[2] == true) {//Gira do primeiro simbolo para o último
    if (cont < 27) {
      cont = 50;}
    if (!zerou) {//Laço para zerar o último valor mostrado
      for (int i = 0; i < 6; i++) {
        digitalWrite(bobinf[i], HIGH);}//Ativa as bobinas inferiores
      delay(100);//Aguarda 100 milissegundos
      for (int i = 0; i < 6; i++) {
        digitalWrite(bobinf[i], LOW);}//Desativa as bobinas inferiores
      zerou = true;}//Informa que zerou os pinos
    switch (cont){//Lê variável cont
      case 27://Mostra o símbolo ponto “.”
        digitalWrite(bobsup[1], HIGH);    
        digitalWrite(bobsup[4], HIGH);
        digitalWrite(bobsup[5], HIGH);
        delay(100);
        digitalWrite(bobsup[1], LOW);
        digitalWrite(bobsup[4], LOW);
        digitalWrite(bobsup[5], LOW);
        break;
      case 28://Mostra o símbolo virgula “,”
        digitalWrite(bobsup[1], HIGH);
        delay(100);
        digitalWrite(bobsup[1], LOW);
        break;
      case 29://Mostra o símbolo ponto e virgule “;”
        digitalWrite(bobsup[1], HIGH);
        digitalWrite(bobsup[2], HIGH);
        delay(100);
        digitalWrite(bobsup[1], LOW);
        digitalWrite(bobsup[2], LOW);
        break;
      case 30://Mostra o símbolo dois pontos “:”
        digitalWrite(bobsup[1], HIGH);
        digitalWrite(bobsup[4], HIGH);
        delay(100);
        digitalWrite(bobsup[1], LOW);
        digitalWrite(bobsup[4], LOW);
        break;
      case 31://Mostra o símbolo barra “/”
        digitalWrite(bobsup[2], HIGH);
        digitalWrite(bobsup[3], HIGH);
        delay(100);
        digitalWrite(bobsup[2], LOW);
        digitalWrite(bobsup[3], LOW);
        break;
      case 32://Mostra o símbolo interrogação “?”
        digitalWrite(bobsup[1], HIGH);
        digitalWrite(bobsup[2], HIGH);
        digitalWrite(bobsup[5], HIGH);
        delay(100);
        digitalWrite(bobsup[1], LOW);
        digitalWrite(bobsup[2], LOW);
        digitalWrite(bobsup[5], LOW);
        break;
      case 33://Mostra o símbolo exclamação “!”
        digitalWrite(bobsup[1], HIGH);
        digitalWrite(bobsup[2], HIGH);
        digitalWrite(bobsup[4], HIGH);
        delay(100);
        digitalWrite(bobsup[1], LOW);
        digitalWrite(bobsup[2], LOW);
        digitalWrite(bobsup[4], LOW);
        break;
      case 34://Mostra o símbolo arroba “@”
        digitalWrite(bobsup[2], HIGH);
        digitalWrite(bobsup[3], HIGH);
        digitalWrite(bobsup[4], HIGH);
        delay(100);
        digitalWrite(bobsup[2], LOW);
        digitalWrite(bobsup[3], LOW);
        digitalWrite(bobsup[4], LOW);
        break;
      case 35://Mostra o símbolo hashtag “#”
        digitalWrite(bobsup[2], HIGH);
        digitalWrite(bobsup[3], HIGH);
        digitalWrite(bobsup[4], HIGH);
        digitalWrite(bobsup[5], HIGH);
        delay(100);
        digitalWrite(bobsup[2], LOW);
        digitalWrite(bobsup[3], LOW);
        digitalWrite(bobsup[4], LOW);
        digitalWrite(bobsup[5], LOW);
        break;
      case 36://Mostra o symbolo mais “+”
        digitalWrite(bobsup[1], HIGH);
        digitalWrite(bobsup[2], HIGH);
        digitalWrite(bobsup[4], HIGH);
        delay(100);
        digitalWrite(bobsup[1], LOW);
        digitalWrite(bobsup[2], LOW);
        digitalWrite(bobsup[4], LOW);
        break;
      case 37://Mostra o símbolo menos “-“
        digitalWrite(bobsup[1], HIGH);
        digitalWrite(bobsup[4], HIGH);
        delay(100);
        digitalWrite(bobsup[1], LOW);
        digitalWrite(bobsup[4], LOW);
        break;
      case 38://Mostra o símbolo asterisco “*”
        digitalWrite(bobsup[2], HIGH);
        digitalWrite(bobsup[4], HIGH);
        delay(100);
        digitalWrite(bobsup[2], LOW);
        digitalWrite(bobsup[4], LOW);
        break;
      case 39://Mostra o símbolo abre aspas ““”
        digitalWrite(bobsup[1], HIGH);
        digitalWrite(bobsup[2], HIGH);
        digitalWrite(bobsup[5], HIGH);
        delay(100);
        digitalWrite(bobsup[1], LOW);
        digitalWrite(bobsup[2], LOW);
        digitalWrite(bobsup[5], LOW);
        break;
      case 40://Mostra o símbolo fecha aspas “””
        digitalWrite(bobsup[2], HIGH);
        digitalWrite(bobsup[4], HIGH);
        digitalWrite(bobsup[5], HIGH);
        delay(100);
        digitalWrite(bobsup[2], LOW);
        digitalWrite(bobsup[4], LOW);
        digitalWrite(bobsup[5], LOW);
        break;
      case 41://Mostra o símbolo plica “'”
        digitalWrite(bobsup[2], HIGH);
        delay(100);
        digitalWrite(bobsup[2], LOW);
        break;
      case 42://Mostra o símbolo “<”
        digitalWrite(bobsup[0], HIGH);
        digitalWrite(bobsup[1], HIGH);
        digitalWrite(bobsup[5], HIGH);
        delay(100);
        digitalWrite(bobsup[0], LOW);
        digitalWrite(bobsup[1], LOW);
        digitalWrite(bobsup[5], LOW);
        break;
      case 43://Mostra o símbolo “>”
        digitalWrite(bobsup[2], HIGH);
        digitalWrite(bobsup[3], HIGH);
        digitalWrite(bobsup[4], HIGH);
        delay(100);
        digitalWrite(bobsup[2], LOW);
        digitalWrite(bobsup[3], LOW);
        digitalWrite(bobsup[4], LOW);
        break;
      case 44://Mostra o símbolo abre parênteses “(”
        digitalWrite(bobsup[1], HIGH);
        digitalWrite(bobsup[2], HIGH);
        digitalWrite(bobsup[4], HIGH);
        digitalWrite(bobsup[5], HIGH);
        delay(100);
        digitalWrite(bobsup[1], LOW);
        digitalWrite(bobsup[2], LOW);
        digitalWrite(bobsup[4], LOW);
        digitalWrite(bobsup[5], LOW);
        break;
      case 45://Mostra o símbolo fecha parênteses “)”
        digitalWrite(bobsup[1], HIGH);
        digitalWrite(bobsup[2], HIGH);
        digitalWrite(bobsup[4], HIGH);
        digitalWrite(bobsup[5], HIGH);
        delay(100);
        digitalWrite(bobsup[1], LOW);
        digitalWrite(bobsup[2], LOW);
        digitalWrite(bobsup[4], LOW);
        digitalWrite(bobsup[5], LOW);
        break;
      case 46://Mostra símbolo que representa sequência de letras maiúsculas “capital”
        digitalWrite(bobsup[5], HIGH);
        delay(1000);
        digitalWrite(bobsup[5], LOW);
        break;
      case 47://Mostra símbolo anderline “_”
        digitalWrite(bobsup[2], HIGH);
        digitalWrite(bobsup[5], HIGH);
        delay(100);
        digitalWrite(bobsup[2], LOW);
        digitalWrite(bobsup[5], LOW);
        break;
      case 48://Mostra o símbolo lógico  E “and”
        digitalWrite(bobsup[0], HIGH);
        digitalWrite(bobsup[1], HIGH);
        digitalWrite(bobsup[2], HIGH);
        digitalWrite(bobsup[3], HIGH);
        digitalWrite(bobsup[5], HIGH);
        delay(100);
        digitalWrite(bobsup[0], LOW);
        digitalWrite(bobsup[1], LOW);
        digitalWrite(bobsup[2], LOW);
        digitalWrite(bobsup[3], LOW);
        digitalWrite(bobsup[5], LOW);
        break;
      case 49://Mostra símbolo que representa sequência de letras “letra”
        digitalWrite(bobsup[4], HIGH);
        digitalWrite(bobsup[5], HIGH);
        delay(100);
        digitalWrite(bobsup[4], LOW);
        digitalWrite(bobsup[5], LOW);
        break;
      case 50://Mostra símbolo que representa sequência de números “numero”
        digitalWrite(bobsup[2], HIGH);
        digitalWrite(bobsup[3], HIGH);
        digitalWrite(bobsup[4], HIGH);
        digitalWrite(bobsup[5], HIGH);
        delay(100);
        digitalWrite(bobsup[2], LOW);
        digitalWrite(bobsup[3], LOW);
        digitalWrite(bobsup[4], LOW);
        digitalWrite(bobsup[5], LOW);
        break;
      default:
        cont = 27;//Para valores diferentes dos esperados mostra o primeiro valor dos símbolos
    }
  }
}

