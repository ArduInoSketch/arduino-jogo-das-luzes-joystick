#include <Arduino.h>

#define azul 8
#define amarelo 9
#define verde 10
#define branco 11
//pinos referente ao joystick
#define eixo_x A1
#define eixo_y A2
#define botam_sw 6

int array_led[] = {branco, azul, amarelo, verde}, registra_seq_led[4];
int esquerda = 4, direita = 3, descer = 2, subir = 1, parado = 5;
int controle = 0, posicaoControleAnterior = 0, cursorControle = 0, posicao = 0;

void setup() {
  for(int i = 0; i < 4; i++) {
      pinMode(array_led[i],OUTPUT);
  }

  pinMode(eixo_x, INPUT);
  pinMode(eixo_x, INPUT);
  pinMode(botam_sw, INPUT_PULLUP);

  randomSeed(analogRead(A0));

  Serial.begin(9600);
}

void sorteiaLed() {
    int cont = 0, led = 0, ledSorteada = 0;

    while(cont < 4) {
        led = random(8, 12);

        if(led != ledSorteada) {
            digitalWrite(led, HIGH);
            registra_seq_led[cont] = led;
            cont++;
            delay(800);
            digitalWrite(led, LOW);
            delay(200);
        }

        ledSorteada = led;
    }
}

int posicaoJoystick() {
    int posicaoJoy = 0;
    int ler_eixoX = analogRead(eixo_x);
    int ler_eixoY = analogRead(eixo_y);

    if ((ler_eixoX >= 460) && (ler_eixoX <= 510) && (ler_eixoY >= 100) && (ler_eixoY <= 400)) {
        posicaoJoy = parado; 
        } else if ((ler_eixoX >= 0) && (ler_eixoX <=480) && (ler_eixoY >= 0)) {
            posicaoJoy = subir; 
            } else if ((ler_eixoY >= 400) && (ler_eixoY < 500) && (ler_eixoX >= 900)) {
                posicaoJoy = descer; 
                } else if ((ler_eixoY >= 940) && (ler_eixoY <= 1025) && (ler_eixoX >= 400)) {
                    posicaoJoy = esquerda;
                    } else if ((ler_eixoX <= 520) && (ler_eixoX >= 400) && (ler_eixoY < 100)) {
                        posicaoJoy = direita;
                    }

    return posicaoJoy;
}

bool controleCursorjoystick() {
    int contCursor = 0, vida = 5, acertos = 0;

    Serial.println("--- JOGO DAS LUZES --- ");
    Serial.print("--- VIDA: ");
    Serial.println(vida);

    do {
    posicao = posicaoJoystick();

    if((posicao == subir) || (posicao == direita)) {
        //valor do controle sera acrescido, de acordo como o cursor segue em "reta"
        if(controle <= 3) {
            digitalWrite(array_led[controle], HIGH);

            if (controle != posicaoControleAnterior) {
                digitalWrite(array_led[posicaoControleAnterior], LOW);
                delay(200);
            }

            posicaoControleAnterior = controle;
            controle++;

            if(controle == 4) {
                controle = 0;
            }
        }
    } else if((posicao == descer) || (posicao == esquerda)) {
        //valor do controle sera diminuido, de acordo com o movimento de "recuamento"
        if(controle >= 0) {
            digitalWrite(array_led[controle], HIGH);

            if (controle != posicaoControleAnterior) {
                digitalWrite(array_led[posicaoControleAnterior], LOW);
                delay(200);
            }

            posicaoControleAnterior = controle;
            controle--;

            if(controle < 0) {
                controle = 3;
            }
        }
    } 

    //verifica se botão foi pressionado e caso não esteja parado (5), analiza se teve acerto
    if (!digitalRead(botam_sw) && posicao != 5) {
            if(array_led[posicaoControleAnterior] == registra_seq_led[contCursor]) {
                acertos++;
                Serial.print("--- ACERTOU! (1P): ");
                Serial.println(acertos);
                delay(500);
                contCursor++;
                if(contCursor == 4) {
                    contCursor = 0;
                }
            } else {
                vida--;
                Serial.print("--- ERROU! (-1V): ");
                Serial.println(vida);
                delay(500);
                if(vida == 0) {
                    return false;
            }
        }
    }
    } while (acertos != 4);

    return true;
}

void loop() {
    sorteiaLed();

    if(controleCursorjoystick()) {
        Serial.println("-- PARABENS! GANHOU O JOGO");
    } else {
        Serial.println("GAME OVER!");
    }

    for (int i = 0; i < 4; i++) {
        //desliga todas leds
        digitalWrite(array_led[i], LOW);
    }

    Serial.println("--- JOGO ENCERRADO! ---");
    Serial.println();
    delay(5000);
}