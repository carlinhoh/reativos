#include "xtea.c.h"
 
#define LED 13
uint32_t key[] = { 1, 2, 3, 4 };
const int sv = 100; 
uint32_t v[sv];

int i, led = 0;

void setup () {
    pinMode(LED, OUTPUT);
    for (i = 0; i < sv; i++) {
      v[i] = i * 10000;
    }
    Serial.begin(9600);
}
 
void loop () {

    static unsigned long tic = micros();
    
    for (i = 0; i < sv; i+=2) {
        encipher(32, &v[i], key);
        decipher(32, &v[i], key);
    }

    digitalWrite(LED, led=!led);

    static unsigned long tac = micros();
    Serial.println(-tic+tac);
    
}
