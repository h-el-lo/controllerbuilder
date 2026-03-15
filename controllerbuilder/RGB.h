// #ifndef RGB_H
// #define RGB_B

// #include <Arduino.h>
// #include <Adafruit_NeoPixel.h>

// class Pixel {
// private:
//   uint8_t _RGB_PIN;
//   uint8_t _pixelPosition;
//   struct color {
//     uint8_t r, g, b;
//   };
//   enum ANIMATOR {
//     LINEAR_STEPPER_FORWARD_BACKWARD,
//     ON_OFF,
//     MONTAGE_MX8,
//   };

//   enum PRESS_MODE {
//     TOGGLE,
//     MOMENTARY
//   };

//   // Synthage default colors
//   uint8_t default_colors[12][3] = {
//     { 51, 86, 255 },   // Color 1 - LightBlue       // Done
//     { 0, 0, 100 },     // Color 2 - Blue            // Done
//     { 101, 0, 205 },   // Color 3 - Indigo/Purple   // Done
//     { 80, 0, 87 },     // Color 4 - Violet          // Done
//     { 195, 0, 60 },    // Color 5 - Magenta         // Done
//     { 50, 3, 0 },      // Color 6 - Red             // Done
//     { 245, 65, 2 },    // Color 7 - Orange          // Done
//     { 255, 190, 0 },   // Color 8 - Yellow          // Done
//     { 180, 255, 0 },   // Color 9 - Lawn Green      // Done
//     { 120, 255, 0 },   // Color 10 - Green          // Done
//     { 72, 255, 51 },   // Color 11 - Mint Green     // Done
//     { 75, 155, 214 },  // Color 12 - Cyan           // Done
//   };
//   // ================= ONBOARD RGB VARIABLES ==================
//   // The fade effect without the delay function (using "RGB_timer" and "micros()")
//   // Animation variables (Very similar to Synthage)
//   int timeon = 435;  // milliseconds
//   int timeoff = 0;   // milliseconds
//   int steps = 240;   // 240 frames in about a second
//   unsigned long RGB_timer = 0;
//   int threshold = (timeon * 1000) / steps;
//   int RGB_count = 0;
//   uint8_t stage = 1;  // Begin animation at stage 1
//   // ==========================================================

//   // Setup Code
//   uint8_t sel = 11;  // Color selection
//   uint8_t* color = colors[sel - 1];
//   rgb(color[0], color[1], color[2]);


// public:
//   // Constructors

//   // Getters

//   // Setters

//   // Methods
//   void linear_stepper_forward_backward(uint_t r, uint_t g, uint_t b, int steps, int timeon, int timeoff) {
//     // This pattern is in four stages, increment, hold, decrement, hold.

//     if (stage == 1) {

//       if (RGB_count <= steps) {
//         if (micros() - RGB_timer >= threshold) {
//           rgbLedWrite(RGB_BUILTIN, ((r * RGB_count) / steps), ((g * RGB_count) / steps), ((b * RGB_count) / steps));
//           RGB_timer = micros();
//           RGB_count += 1;
//         }
//       }

//       if (RGB_count == steps) {
//         RGB_count = 0;
//         rgbLedWrite(RGB_BUILTIN, r, g, b);
//         RGB_timer = micros();  // Reset the RGB_timer variable
//         stage = 2;
//       }


//     } else if (stage == 2) {
//       if (micros() - RGB_timer >= timeoff * 1000) {
//         stage = 3;
//       }

//     } else if (stage == 3) {

//       if (RGB_count <= steps) {
//         if (micros() - RGB_timer >= threshold) {
//           rgbLedWrite(RGB_BUILTIN, ((r * (steps - RGB_count)) / steps), ((g * (steps - RGB_count)) / steps), ((b * (steps - RGB_count)) / steps));
//           RGB_timer = micros();
//           RGB_count += 1;
//         }
//       }

//       if (RGB_count == steps) {
//         RGB_count = 0;
//         rgbLedWrite(RGB_BUILTIN, 0, 0, 0);
//         RGB_timer = micros();  // Reset the RGB_timer variable
//         stage = 4;
//       }

//     } else if (stage == 4) {
//       if (micros() - RGB_timer >= timeoff * 1000) {
//         stage = 1;
//       }
//     }
//   }

//   void ON_OFF() {
//     // Pass
//   }
// };

// class RGB_LED_STRIP : public Adafruit_NeoPixel {
// private:
//   uint8_t _RGB_PIN;
//   uint8_t _NUMPIXELS;

// public:
//   // Constructors
//   RGB_LED_STRIP(uint16_t NUMPIXELS, uint8_t RGB_PIN);
//   // Getters
//   // Setters
//   // Methods
//   void update();

//   // Initialize RGB LED
//   // pixels.begin();
//   // pixels.setPixelColor(0, pixels.Color(r, g, b));
//   // pixels.show();

//   //  for led in ledstrippixelnums:
//   //  pixels.addpixel

//   //  for pixel in pixels:
//   //  pixel.mode();
// }

// #endif