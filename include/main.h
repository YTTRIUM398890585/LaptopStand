uint8_t hue = 0;
uint8_t brightness = 80;      // Default brightness is 80
uint8_t brightness_min = 55;  // Min brightness is 55
uint8_t brightness_max = 255; // Max brightness is 255

uint8_t speed_mode = 3;
float delay_multiplier = 1;

int LED_ID = 0;
int LED_ID_1 = 0;
int LED_ID_2 = 0;
int LED_ID_3 = 0;

bool Tick_Tock = LOW;

unsigned long previousMillis = 0;
// unsigned long previousMillis_for_fading = 0;
unsigned long presentMillis = 0;

int Mode = 0;
int PreviousMode = 1;

CRGBArray<NUM_LEDS> leds; // CRGBArray<no of addresable nodes> name;

/*Modes*/
// 1)  RGB_Without_Fading();
// 2)  RGB_With_Fading();
// 3)  Random_Without_Fading();
// 4)  Random_With_Fading();
// 5)  Random_Contrasting_Collisions();
// 6)  RGB_Contrasting_Collisions();
// 7)  RGB_Twins();
// 8)  RGB_Contrasting_Double_Twins();
// 9)  RGB_Quartet();
// 10) Random_Twins();
// 11) RGB_Alternating();
// 12) Random_Alternating();

void RGB_Without_Fading();

void RGB_With_Fading();

void Random_Without_Fading();

void Random_With_Fading();

void Random_Contrasting_Collisions();

void RGB_Contrasting_Collisions();

// void RGB_Contrasting_Collisions();

void RGB_Twins();

void RGB_Contrasting_Double_Twins();

void RGB_Quartet();

void Random_Twins();

void RGB_Alternating();

void Random_Alternating();

void speed_mode_indicator();