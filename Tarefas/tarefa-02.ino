#define BT1 2
#define BT2 7
#define LED 13

int before, now;
int dt = 1000;
int tStop = -1;
int ledState, bt1State, bt2State;

void setup() {
  before = millis();
  pinMode(LED,OUTPUT);
  pinMode(BT1,INPUT);
  pinMode(BT2,INPUT);
  ledState = 0; bt1State = 0; bt2State = 0;
}

void loop() {
  //Salva tempo atual utilizando a chamada de função millis() para timing (não bloqueante).
  now = millis();
  //Muda estado do LED a cada 'dt' milissegundos.
  if(now >= before + dt){
    ledState = !ledState;
    before = now;
  }
  //Acende ou apaga LED.
  if(ledState) digitalWrite(LED,HIGH);
  else digitalWrite(LED,LOW);
  
  /*Reação ao clique dos botões.*/
  //Se os dois forem pressionados ao mesmo tempo, salva tempo atual na variavel tStop (inicializada como -1).
  if(digitalRead(BT1) && digitalRead(BT2)) tStop = millis();
  //Desacelera (apenas na transição de LOW para HIGH).
  if(digitalRead(BT1) && !bt1State && dt<2000) dt+=100;
  //Acelera (apenas na transição de LOW para HIGH).
  if(digitalRead(BT2) && !bt2State && dt>100) dt-=100;

  //Se os dois botões foram pressionados, tStop != -1. Desliga o LED permanentemente em 500ms.
  if(tStop != -1 && tStop + 500 <= now) {
    digitalWrite(LED,LOW);
    while(1);
  }
  //Atualiza estado atual dos botões.
  bt1State = digitalRead(BT1);
  bt2State = digitalRead(BT2);
}