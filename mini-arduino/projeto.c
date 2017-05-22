#include <AFMotor.h>
#include <Servo.h>
#include <NewPing.h>
 
#define TRIG_PIN A0 // Conecta TRIG do sensor ultrassom à porta analógica A0
#define ECHO_PIN A5 // Conecta ECHO do sensor ultrassom à porta analógica A5
#define SERVO_PIN 10
#define MAX_DISTANCE 300         // Distância máxima de medição do sensor ultrassom. Retorna zero se passar desse valor.
#define MAX_SPEED 150            // Velocidade dos motores de tração.
#define MAX_SPEED_OFFSET 40      // Offset de velocidade utilizado para fazer curvas.
#define COLL_DIST 30            // Distância de colisão.
 
AF_DCMotor leftMotorI(1, MOTOR12_1KHZ);
AF_DCMotor leftMotorII(2, MOTOR12_1KHZ);
AF_DCMotor rightMotorI(3, MOTOR34_1KHZ);
AF_DCMotor rightMotorII(4, MOTOR34_1KHZ);
 
NewPing sonar(TRIG_PIN, ECHO_PIN, MAX_DISTANCE);
 
Servo servoMotor;
 
int speedSet = 0;
 
void setupServo(Servo servo)
{
    //Conecta o servo na porta 10.
    servo.attach(SERVO_PIN);
    //Aponta para frente.
    servo.write(90);
    delay(1000);
}
 
void setup()
{
    Serial.begin(9600);
    //Inicializa motor servo.
    setupServo(servoMotor);
    //Começa a se mover para frente.
    moveForward();
}
 
void loop()
{
    //No loop principal, lemos a distancia medida pelo sensor ultrassom constantemente,
    //e caso uma obstrução seja detectada em seu caminho, procura uma melhor alternativa
    //de trajeto.
    int curDist = readPing();
    Serial.println(curDist);
    if (curDist < COLL_DIST && curDist != 0) changePath();
}
 
long unsigned int moveServoLeft(){
  int moveTime = 500;
  //Direciona o sensor à esquerda.
  servoMotor.write(36);
  return moveTime;
}
 
long unsigned int moveServoRight(){
  int moveTime = 600;
  //Direciona o sensor à direita.
  servoMotor.write(144);
  return moveTime;
}
 
//Mede as distâncias à esquerda, e à direita,
//escolhendo o melhor caminho para evitar colisões.
void changePath()
{
    //Para os motores.
    moveStop();
    delay(moveServoLeft());
    //Mede a distância à direita.
    int rightDistance = readPing();
    Serial.print("Right: ");
    Serial.println(rightDistance);
    delay(moveServoRight());
    //Mede a distância à esquerda.
    int leftDistance = readPing();
    Serial.print("Left: ");
    Serial.println(leftDistance);
    //Retorna sensor ao centro.
    servoMotor.write(90);
    //Manobra para evitar colisão.
    chooseBestMovement(leftDistance, rightDistance);
}
 
//Determina o melhor plano de ação, dadas as duas distâncias medidas.
void chooseBestMovement(int leftDistance, int rightDistance)
{
    //Se a distancia medida for zero, passou dos limites de medição do sensor,
    //logo, é uma boa opção para evitar colisão.
    int moveTime;
    if (leftDistance > rightDistance) moveTime = turnLeft();
    else if (rightDistance > leftDistance) moveTime = turnRight();
    else moveTime = turnAround();
    //Delay passando o tempo da manobra, para evitar falsas colisões.
    delay(moveTime);
    //Move para frente novamente.
    moveForward();
}
 
//Lê 5 distâncias medidas pelo sensor e retorna a média.
int readPing()
{
  unsigned int uS = 0;
  unsigned int numMed = 5;
  for(int i = 0; i < numMed; i++){
    unsigned int med = sonar.ping();
    uS += med;
    delay(10);
  }
  return uS / (numMed*US_ROUNDTRIP_CM);
}
 
//Para todos os motores.
void moveStop()
{
    leftMotorI.run(RELEASE);
    leftMotorII.run(RELEASE);
    rightMotorI.run(RELEASE);
    rightMotorII.run(RELEASE);
}
 
//Movimento para frente
void moveForward()
{
    leftMotorI.run(FORWARD);                                // Move para frente.
    leftMotorII.run(FORWARD);                               // Move para frente.
    rightMotorI.run(FORWARD);                               // Move para frente.
    rightMotorII.run(FORWARD);                              // Move para frente.
    speedUp();
}
 
//Movimento para trás.
void moveBackward()
{
    leftMotorI.run(BACKWARD);                                // Muda para marcha ré.
    leftMotorII.run(BACKWARD);                               // Muda para marcha ré.
    rightMotorI.run(BACKWARD);                               // Muda para marcha ré.
    rightMotorII.run(BACKWARD);                              // Muda para marcha ré.
    speedUp();
}
 
//Aumenta velocidade gradativamente para evitar perda de bateria.
void speedUp() {
  for (speedSet = 0; speedSet < MAX_SPEED; speedSet += 2){
      leftMotorI.setSpeed(speedSet);
      leftMotorII.setSpeed(speedSet);
      rightMotorI.setSpeed(speedSet);
      rightMotorII.setSpeed(speedSet);
      delay(5);
  }
}
 
//Vira à direita.
int turnRight()
{
    Serial.println("Turned right!");
    int moveTime = 350;
    leftMotorI.run(FORWARD);   // turn motor 1 forward
    leftMotorII.run(FORWARD);  // turn motor 2 forward
    rightMotorI.run(BACKWARD);  // turn motor 3 FORWARD
    rightMotorII.run(BACKWARD); // turn motor 4 FORWARD
    rightMotorI.setSpeed(speedSet + MAX_SPEED_OFFSET);
    rightMotorII.setSpeed(speedSet + MAX_SPEED_OFFSET);
    return moveTime;
}
 
//Vira à esquerda.
int turnLeft()
{
    Serial.println("Turned left!");
    int moveTime = 350;
    leftMotorI.run(BACKWARD);  // turn motor 1 FORWARD
    leftMotorII.run(BACKWARD); // turn motor 2 FORWARD
    leftMotorI.setSpeed(speedSet + MAX_SPEED_OFFSET);
    leftMotorII.setSpeed(speedSet + MAX_SPEED_OFFSET);
    rightMotorI.run(FORWARD);  // turn motor 3 forward
    rightMotorII.run(FORWARD); // turn motor 4 forward
    return moveTime;
}
 
//Vira 180 graus.
int turnAround()
{
    Serial.println("Turned around!");
    int moveTime = 500;
    leftMotorI.run(FORWARD);   // turn motor 1 forward
    leftMotorII.run(FORWARD);  // turn motor 2 forward
    rightMotorI.run(FORWARD);  // turn motor 3 FORWARD
    rightMotorII.run(FORWARD); // turn motor 4 FORWARD
    rightMotorI.setSpeed(speedSet + MAX_SPEED_OFFSET);
    rightMotorII.setSpeed(speedSet + MAX_SPEED_OFFSET);
    return moveTime;
}