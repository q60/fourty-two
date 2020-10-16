#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <Keypad.h>

LiquidCrystal_I2C lcd(0x27, 0x10, 2);

const byte ROWS = 4;
const byte COLS = 4;
char hexaKeys[ROWS][COLS] = {
	{'+','-','<','>'},
	{'.',',','[',']'},
	{'_','_','_','_'},
	{'*','C','#','_'}
};
byte colPins[COLS] = {7, 6, 5, 4};
byte rowPins[ROWS] = {11, 10, 9, 8};
Keypad customKeypad = Keypad(makeKeymap(hexaKeys), rowPins, colPins, ROWS, COLS);

String _text = String();
String _text_well_formed = String();
unsigned char* buf = new unsigned char[0x400];
int _counter_length;
int _erase_counter;
int _height;
int _length;

void (*reset_function) (void) = 0;

void setup() {
	lcd.begin();
	Serial.begin(9600);
	lcd.backlight();
	lcd.blink();
}

void loop() {
  char customKey = customKeypad.getKey();
	if (customKey) {
		switch (customKey) {
			case '#': {
				lcd.clear();
				_text.remove(_text.length() - 1);
				if (_length > 0x10) {
					_length--;
					_text_well_formed = _text.substring(_erase_counter, _text.length());
					if (_height == 1) {
						String _text_first_part = _text.substring((_erase_counter - 0x10), _erase_counter);
						lcd.print(_text_first_part);
					}
					lcd.setCursor(0, _height);
					lcd.print(_text_well_formed);
				} else {
					lcd.print(_text);
				}
				break;
			}

			case '*': {
				_text.getBytes(buf, 0x400, 0);
				lcd.clear();
				lcd.setCursor(0, 0);
				brainfuck((char*)buf);
				break;
			}

			case 'C': {
				lcd.clear();
				lcd.print("Reloading...");
				delay(500);
				reset_function();
				break;
			}

			default: {
				lcd.print(customKey);
				_text += customKey;
				_counter_length = _text.length();
				_length++;
				if(_counter_length >= 0x10) {
					if (_counter_length % 0x10 == 0 && round((_counter_length / 16.0) - 0.5) % 2 != 0) {
						lcd.setCursor(0, 1);
						_erase_counter += 0x10;
						_height = 1;
					} else if (_counter_length % 0x20 == 0) {
						lcd.clear();
						lcd.setCursor(0, 0);
						_erase_counter += 0x10;
						_height = 0;
					}
				}
				break;
			}
		}
	}
}

void brainfuck(char code[]) {
	int code_position = 0;
	byte memory[0xFF] = {0};
	byte memory_position;
	int stack[0xFF] = {0};
	byte stack_position = 0;

	while (code[code_position] != 0) {
		switch (code[code_position]) {
			case '>':
				memory_position++;
				break;

			case '<':
				memory_position--;
				break;

			case '+':
				memory[memory_position]++;
				break;

			case '-':
				memory[memory_position]--;
				break;

			case '.':
				lcd.print(char(memory[memory_position]));
				break;

			case ',':
				for (;;) {
					if (Serial.available() > 0) {
						memory[memory_position] = Serial.read();
						break;
					}
				}
				break;

			case '[':
				if (memory[memory_position]) {
					stack_position++;
					stack[stack_position] = code_position;
				} else {
					while(code[code_position] != ']') {
						code_position++;
					}
				}
				break;

			case ']':
				if (memory[memory_position]) {
					code_position = stack[stack_position];
				} else {
					stack_position--;
				}
				break;
		}
		code_position++;
	}
}
