
// int pots[] = {
//   A1, // Rotary 2
//   A0, // Rotary 1
// };
// int potcc[] = {
//   11, // Expression Pedal CC 02
//   1, // Modulation Wheel CC 01
// };

// int potstate[] = { 0 };
// int potpstate[] = { 0 };
// const byte potthreshold = 1;

// void setup() {
//   // put your setup code here, to run once:
//   Serial.begin(9600);
// }

// void loop() {
//   // put your main code here, to run repeatedly:
//   for (int pot = 0; pot < sizeof(pots); pot++) {
//     potstate[pot] = map(analogRead(pots[pot]),3 0, 1023, 0, 127);
//     int potvar = abs(potstate[pot] - potpstate[pot]);
//     if (potstate[pot] != potpstate[pot]) {
//       if (potvar > potthreshold) {
//         Serial.print("Pot ");
//         Serial.print(pot);
//         Serial.print("  |  ");
//         Serial.print("Potstate: ");
//         Serial.print(potstate[pot]);
//         Serial.print("  |  ");
//         Serial.print("Potprevtate: ");
//         Serial.print(potpstate[pot]);
//         Serial.print("  |  ");
        
//         potpstate[pot] = potstate[pot];
//         delay(1200);
//       }
//     }
//   }

//   Serial.println();
// }





// This is a file for trying out those random ideas.