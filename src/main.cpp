#include <Arduino.h>
#include "FastLED.h"
#include "define.h"
#include "main.h"

void setup()
{
	// FastLED.addLeds<chip type, pin on Arduino, RGB
	// sequence thats dependant on strip>(name, no of
	// addresable nodes);
	FastLED.addLeds<WS2812, DataPin, RGB>(leds, NUM_LEDS); // WS2815 is used but WS2812 works, but need RGB

	pinMode(Mode_Button, INPUT_PULLUP);
	pinMode(Up_Button, INPUT_PULLUP);
	pinMode(Down_Button, INPUT_PULLUP);

	pinMode(RandomPin, INPUT);
	randomSeed(analogRead(RandomPin));

	leds.fadeToBlackBy(255); // to clear away previous colours
	FastLED.show();
}

void loop()
{
	bool Mode_Button_State = digitalRead(Mode_Button);
	bool Up_Button_State = digitalRead(Up_Button);
	bool Down_Button_State = digitalRead(Down_Button);

	presentMillis = millis();

	/*To vary brightness*/
	if (Up_Button_State == LOW && Mode != 0 && brightness < brightness_max) {
		brightness = brightness + 1;
		delay(5);
	}

	if (Down_Button_State == LOW && Mode != 0 && brightness > brightness_min) {
		brightness = brightness - 1;
		delay(5);
	}

	/*To vary speed*/
	if (Up_Button_State == LOW && Mode == 0 && speed_mode < 5) {
		speed_mode = speed_mode + 1;

		speed_mode_indicator();

		delay_multiplier = 1 + 0.20 * (3 - speed_mode);

		delay(200);

		leds[0] = CRGB::Black;
		FastLED.show();
	}

	if (Down_Button_State == LOW && Mode == 0 && speed_mode > 1) {
		speed_mode = speed_mode - 1;

		speed_mode_indicator();

		delay_multiplier = 1 + 0.20 * (3 - speed_mode);

		delay(200);

		leds[0] = CRGB::Black;
		FastLED.show();
	}

	/*To change modes*/
	if (Mode_Button_State == LOW) {
		if (Mode == 0) {
			Mode = PreviousMode;
		}
		else {
			Mode++;
		}

		LED_ID = 0;
		LED_ID_1 = 0;
		LED_ID_2 = 0;
		LED_ID_3 = 0;

		hue = 0;

		delay(200);

		Mode_Button_State = digitalRead(Mode_Button);

		if (Mode_Button_State == LOW) {
			leds.fadeToBlackBy(255);
			FastLED.show();
			PreviousMode = Mode - 1; // to compensate for the Mode++; above
			Mode = 0;
			delay(300);
		}
	}

	switch (Mode) {
	case 0:
		break;
	case 1:
		RGB_Without_Fading();
		break;
	case 2:
		RGB_With_Fading();
		break;
	case 3:
		Random_Without_Fading();
		break;
	case 4:
		RGB_Alternating();
		break;
	case 5:
		Random_Alternating();
		break;
	case 6:
		RGB_Contrasting_Collisions();
		break;
	case 7:
		RGB_Twins();
		break;
	case 8:
		RGB_Contrasting_Double_Twins();
		break;
	case 9:
		RGB_Quartet();
		break;
	case 10:
		Random_Twins();
		break;
	default:
		Mode = 1;
		break;
	}
}

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

void RGB_Without_Fading()
{
	if (presentMillis - previousMillis >= 10 * delay_multiplier) {
		for (int i = 0; i < NUM_LEDS; i++) {
			leds[i] = CHSV(hue - i * 255 / NUM_LEDS, 255, brightness);
		}
		FastLED.show();
		hue++;
		previousMillis = presentMillis;
	}
}

void RGB_With_Fading()
{
	if (presentMillis - previousMillis >= 20 * delay_multiplier) {
		uint8_t brightness_for_fade = map(brightness, brightness_min, brightness_max, 1, 10);
		leds.fadeToBlackBy(brightness_for_fade);

		leds[LED_ID] = CHSV(hue, 255, brightness);
		FastLED.show();

		LED_ID++;
		hue = hue + 255 / (NUM_LEDS * 2);

		if (LED_ID >= NUM_LEDS) {
			LED_ID = 0;
		}

		previousMillis = presentMillis;
	}
}

void Random_Without_Fading()
{
	if (presentMillis - previousMillis >= 100 * delay_multiplier) {
		for (int i = 0; i < NUM_LEDS; i++) {
			hue = random(0, 255);
			leds[i] = CHSV(hue, 255, brightness);
		}
		FastLED.show();
		previousMillis = presentMillis;
	}
}

void Random_With_Fading()
{
	if (presentMillis - previousMillis >= 20 * delay_multiplier) {
		uint8_t brightness_for_fade = map(brightness, brightness_min, brightness_max, 1, 25);
		leds.fadeToBlackBy(brightness_for_fade);

		hue = random(0, 255);
		leds[LED_ID] = CHSV(hue, 255, brightness);
		FastLED.show();

		LED_ID++;

		if (LED_ID >= NUM_LEDS) {
			LED_ID = 0;
		}

		previousMillis = presentMillis;
	}
}

void Random_Contrasting_Collisions()
{
	if (presentMillis - previousMillis >= 50 * delay_multiplier) {
		uint8_t brightness_for_fade = map(brightness, brightness_min, brightness_max, 100, 200);
		leds.fadeToBlackBy(brightness_for_fade);

		if (LED_ID == 0) {
			hue = random(0, 255);
		}

		leds[LED_ID] = CHSV(hue, 255, brightness);
		leds[NUM_LEDS - 1 - LED_ID] = CHSV(hue + 128, 255, brightness);
		FastLED.show();

		LED_ID++;

		if (LED_ID >= NUM_LEDS) {
			LED_ID = 0;
		}

		previousMillis = presentMillis;
	}
}

void RGB_Contrasting_Collisions()
{
	if (presentMillis - previousMillis >= 50 * delay_multiplier) {
		uint8_t brightness_for_fade = map(brightness, brightness_min, brightness_max, 50, 150);
		leds.fadeToBlackBy(brightness_for_fade);

		leds[LED_ID] = CHSV(hue, 255, brightness);
		leds[NUM_LEDS - 1 - LED_ID] = CHSV(hue + 128, 255, brightness);
		FastLED.show();

		LED_ID++;
		hue = hue + 255 / (NUM_LEDS * 2);

		if (LED_ID >= NUM_LEDS) {
			LED_ID = 0;
		}

		previousMillis = presentMillis;
	}
}

// void RGB_Contrasting_Collisions() {
//   if (presentMillis - previousMillis_for_fading >= 10) {
//     uint8_t brightness_for_fade = map(brightness, brightness_min, brightness_max, 1, 20);
//     leds.fadeToBlackBy(brightness_for_fade);
//     previousMillis_for_fading = presentMillis;
//   }
//
//   if (presentMillis - previousMillis >= 60 * delay_multiplier) {
//     //    uint8_t brightness_for_fade = map(brightness, brightness_min, brightness_max, 25, 125);
//     //    leds.fadeToBlackBy(brightness_for_fade);
//
//     LED_ID_1 = NUM_LEDS / 2 - 1 - LED_ID;
//     LED_ID_2 = LED_ID + NUM_LEDS / 2;
//     LED_ID_3 = LED_ID_1 + NUM_LEDS / 2;
//
//     if (LED_ID_1 < 0) {
//       LED_ID_1 = LED_ID_1 + NUM_LEDS;
//     }
//
//     if (LED_ID_2 >= NUM_LEDS) {
//       LED_ID_2 = LED_ID_2 - NUM_LEDS;
//     }
//
//     leds[LED_ID] = CHSV(hue, 255, brightness);
//     leds[LED_ID_1] = CHSV(hue + 128, 255, brightness);
//     leds[LED_ID_2] = CHSV(hue, 255, brightness);
//     leds[LED_ID_3] = CHSV(hue + 128, 255, brightness);
//
//     FastLED.show();
//
//     LED_ID++;
//     hue = hue + 255 / (NUM_LEDS * 2);
//
//     if (LED_ID >= NUM_LEDS) {
//       LED_ID = 0;
//     }
//
//     previousMillis = presentMillis;
//   }
// }

void RGB_Twins()
{
	if (presentMillis - previousMillis >= 50 * delay_multiplier) {
		uint8_t brightness_for_fade = map(brightness, brightness_min, brightness_max, 50, 150);
		leds.fadeToBlackBy(brightness_for_fade);

		LED_ID_1 = LED_ID + NUM_LEDS / 2;

		if (LED_ID_1 >= NUM_LEDS) {
			LED_ID_1 = LED_ID_1 - NUM_LEDS;
		}

		leds[LED_ID] = CHSV(hue, 255, brightness);
		leds[LED_ID_1] = CHSV(hue, 255, brightness);
		FastLED.show();

		LED_ID++;
		hue = hue + 255 / (NUM_LEDS * 2);

		if (LED_ID >= NUM_LEDS) {
			LED_ID = 0;
		}

		previousMillis = presentMillis;
	}
}

void RGB_Contrasting_Double_Twins()
{
	if (presentMillis - previousMillis >= 50 * delay_multiplier) {
		uint8_t brightness_for_fade = map(brightness, brightness_min, brightness_max, 100, 200);
		leds.fadeToBlackBy(brightness_for_fade);

		LED_ID_1 = LED_ID + NUM_LEDS / 4;
		LED_ID_2 = LED_ID + NUM_LEDS / 2;
		LED_ID_3 = LED_ID + NUM_LEDS * 3 / 4;

		if (LED_ID_1 >= NUM_LEDS) {
			LED_ID_1 = LED_ID_1 - NUM_LEDS;
		}

		if (LED_ID_2 >= NUM_LEDS) {
			LED_ID_2 = LED_ID_2 - NUM_LEDS;
		}

		if (LED_ID_3 >= NUM_LEDS) {
			LED_ID_3 = LED_ID_3 - NUM_LEDS;
		}

		leds[LED_ID] = CHSV(hue, 255, brightness);
		leds[LED_ID_1] = CHSV(hue + 128, 255, brightness);
		leds[LED_ID_2] = CHSV(hue, 255, brightness);
		leds[LED_ID_3] = CHSV(hue + 128, 255, brightness);

		FastLED.show();

		LED_ID++;
		hue = hue + 255 / (NUM_LEDS * 2);

		if (LED_ID >= NUM_LEDS) {
			LED_ID = 0;
		}

		previousMillis = presentMillis;
	}
}

void RGB_Quartet()
{
	if (presentMillis - previousMillis >= 50 * delay_multiplier) {
		uint8_t brightness_for_fade = map(brightness, brightness_min, brightness_max, 50, 150);
		leds.fadeToBlackBy(brightness_for_fade);

		LED_ID_1 = LED_ID + NUM_LEDS / 4;
		LED_ID_2 = LED_ID + NUM_LEDS / 2;
		LED_ID_3 = LED_ID + NUM_LEDS * 3 / 4;

		if (LED_ID_1 >= NUM_LEDS) {
			LED_ID_1 = LED_ID_1 - NUM_LEDS;
		}

		if (LED_ID_2 >= NUM_LEDS) {
			LED_ID_2 = LED_ID_2 - NUM_LEDS;
		}

		if (LED_ID_3 >= NUM_LEDS) {
			LED_ID_3 = LED_ID_3 - NUM_LEDS;
		}

		leds[LED_ID] = CHSV(hue, 255, brightness);
		leds[LED_ID_1] = CHSV(hue + 64, 255, brightness);
		leds[LED_ID_2] = CHSV(hue + 128, 255, brightness);
		leds[LED_ID_3] = CHSV(hue + 192, 255, brightness);

		FastLED.show();

		LED_ID++;
		hue = hue + 255 / (NUM_LEDS * 2);

		if (LED_ID >= NUM_LEDS) {
			LED_ID = 0;
		}

		previousMillis = presentMillis;
	}
}

void Random_Twins()
{
	if (presentMillis - previousMillis >= 50 * delay_multiplier) {
		uint8_t brightness_for_fade = map(brightness, brightness_min, brightness_max, 10, 20);
		leds.fadeToBlackBy(brightness_for_fade);

		LED_ID_1 = LED_ID + NUM_LEDS / 2;

		if (LED_ID_1 >= NUM_LEDS) {
			LED_ID_1 = LED_ID_1 - NUM_LEDS;
		}

		hue = random(0, 255);
		uint8_t hue_1 = random(0, 255);

		leds[LED_ID] = CHSV(hue, 255, brightness);
		leds[LED_ID_1] = CHSV(hue_1, 255, brightness);
		FastLED.show();

		LED_ID++;

		if (LED_ID >= NUM_LEDS) {
			LED_ID = 0;
		}

		previousMillis = presentMillis;
	}
}

void RGB_Alternating()
{
	if (presentMillis - previousMillis >= 10 * delay_multiplier) {
		for (int i = 0; i < NUM_LEDS; i++) {
			if (i % 2 == 0) {
				leds[i] = CHSV(hue, 255, brightness);
			}
			else {
				leds[i] = CHSV(hue + 128, 255, brightness);
			}
		}
		FastLED.show();
		hue++;
		previousMillis = presentMillis;
	}
}

void Random_Alternating()
{
	if (presentMillis - previousMillis >= 1000 * delay_multiplier) {
		hue = random(0, 255);

		if (Tick_Tock == LOW) {
			for (int i = 0; i < NUM_LEDS; i = i + 2) {
				leds[i] = CHSV(hue, 255, brightness);
			}
		}
		else {
			for (int i = 1; i < NUM_LEDS; i = i + 2) {
				leds[i] = CHSV(hue, 255, brightness);
			}
		}

		FastLED.show();

		Tick_Tock = !Tick_Tock;

		previousMillis = presentMillis;
	}
}

void speed_mode_indicator()
{
	switch (speed_mode) {
	case 1:
		leds[0] = CHSV(0, 255, brightness); // Red to indicate slowest speed
		break;
	case 2:
		leds[0] = CHSV(64, 255, brightness); // Yellow
		break;
	case 3:
		leds[0] = CHSV(96, 255, brightness); // Green to indicate normal speed
		break;
	case 4:
		leds[0] = CHSV(128, 255, brightness); // Aqua
		break;
	case 5:
		leds[0] = CHSV(160, 255, brightness); // Blue to indicate fastest speed
		break;
	}
	FastLED.show();
}