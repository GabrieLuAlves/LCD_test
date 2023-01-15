#define digitalWrite(PIN, HIGH) digitalWrite(PIN, HIGH);
#define digitalWrite(PIN, LOW) digitalWrite(PIN, LOW);

class LCD {
public:
  LCD(byte, byte, byte, byte, byte, byte, byte);

  // Configure the i/o configurations of the pins
  void initializePins();

  void initializeBus();

  void clearScreen();
  void functionSet(bool, bool);
  void displayControl(bool displayOn, bool cursorOn, bool cursorBlinkOn);

  void sendString(const char* string);

  // debug
  void displayCurrentConfig();
  byte asciiToLCDCode(char c);
private:
  void waitForBusyFlag();

  byte rs;
  byte rw;
  byte e;

  byte d7;
  byte d6;
  byte d5;
  byte d4;
};

LCD::LCD(byte _rs, byte _rw, byte _e, byte _d7, byte _d6, byte _d5, byte _d4)
  : rs{_rs},
  rw{_rw},
  e{_e},
  d7{_d7},
  d6{_d6},
  d5{_d5},
  d4{_d4}
{}

void LCD::initializePins() {
  pinMode(rs, OUTPUT);
  pinMode(rw, OUTPUT);
  pinMode(e , OUTPUT);
  pinMode(d7, OUTPUT);
  pinMode(d6, OUTPUT);
  pinMode(d5, OUTPUT);
  pinMode(d4, OUTPUT);
}

void LCD::initializeBus() {
  digitalWrite(rs, LOW);
  digitalWrite(rw, LOW);
  digitalWrite(d7, LOW);
  digitalWrite(d6, LOW);
  digitalWrite(d5, HIGH);
  digitalWrite(d4, LOW);

  digitalWrite(e, HIGH);
  digitalWrite(e, LOW);
  waitForBusyFlag();
}

void LCD::clearScreen() {
  digitalWrite(rs, LOW);
  digitalWrite(rw, LOW);

  digitalWrite(d7, LOW);
  digitalWrite(d6, LOW);
  digitalWrite(d5, LOW);
  digitalWrite(d4, LOW);

  digitalWrite(e, HIGH);
  digitalWrite(e, LOW);
  waitForBusyFlag();
  
  digitalWrite(d7, LOW);
  digitalWrite(d6, LOW);
  digitalWrite(d5, LOW);
  digitalWrite(d4, HIGH);

  digitalWrite(e, HIGH);
  digitalWrite(e, LOW);
  waitForBusyFlag();
}

// @param twoLinesMode: if true, sets the display mode to 2-line 
void LCD::functionSet(bool twoLinesMode, bool largeLines) {
  digitalWrite(rs, LOW);
  digitalWrite(rw, LOW);
  
  digitalWrite(d7, LOW);
  digitalWrite(d6, LOW);
  digitalWrite(d5, HIGH);
  digitalWrite(d4, LOW);

  digitalWrite(e, HIGH);
  digitalWrite(e, LOW);
  waitForBusyFlag();

  digitalWrite(d7, twoLinesMode ? HIGH : LOW);
  digitalWrite(d6, largeLines ? HIGH : LOW);

  digitalWrite(e, HIGH);
  digitalWrite(e, LOW);
  waitForBusyFlag();
}

void LCD::displayControl(bool displayOn, bool cursorOn, bool cursorBlinkOn) {
  digitalWrite(rs, LOW);
  digitalWrite(rw, LOW);
  
  digitalWrite(d7, LOW);
  digitalWrite(d6, LOW);
  digitalWrite(d5, LOW);
  digitalWrite(d4, LOW);

  digitalWrite(e, HIGH);
  digitalWrite(e, LOW);
  waitForBusyFlag();

  digitalWrite(d7, HIGH);
  digitalWrite(d6, displayOn ? HIGH : LOW);
  digitalWrite(d5, cursorOn ? HIGH : LOW);
  digitalWrite(d4, cursorBlinkOn ? HIGH : LOW);

  digitalWrite(e, HIGH);
  digitalWrite(e, LOW);
  waitForBusyFlag();
}

void LCD::sendString(const char* string) {
  digitalWrite(rs, HIGH);
  digitalWrite(rw, LOW);

  byte code;
  byte firstPart;
  byte secondPart;

  for(char *c = string ; *c != '\0' ; c++) {
    code = asciiToLCDCode(*c);
    firstPart = code >> 4;
    secondPart = 0xF & code;
    
    digitalWrite(d7, firstPart & 0x08 ? HIGH : LOW);
    digitalWrite(d6, firstPart & 0x04 ? HIGH : LOW);
    digitalWrite(d5, firstPart & 0x02 ? HIGH : LOW);
    digitalWrite(d4, firstPart & 0x01 ? HIGH : LOW);

    digitalWrite(e, HIGH);
    digitalWrite(e, LOW);
    waitForBusyFlag(); 

    digitalWrite(d7, secondPart & 0x08 ? HIGH : LOW);
    digitalWrite(d6, secondPart & 0x04 ? HIGH : LOW);
    digitalWrite(d5, secondPart & 0x02 ? HIGH : LOW);
    digitalWrite(d4, secondPart & 0x01 ? HIGH : LOW);

    digitalWrite(e, HIGH);
    digitalWrite(e, LOW);
    waitForBusyFlag(); 
  }
}

void LCD::displayCurrentConfig() {
  Serial.println("PIN CONFIG");

  Serial.print("RS: ");
  Serial.println(rs);
  
  Serial.print("RW: ");
  Serial.println(rw);

  Serial.print("E : ");
  Serial.println(e);
  
  Serial.print("D7: ");
  Serial.println(d7);

  Serial.print("D6: ");
  Serial.println(d6);

  Serial.print("D5: ");
  Serial.println(d5);

  Serial.print("D4: ");
  Serial.println(d4);  
}

byte LCD::asciiToLCDCode(char c) {
  switch(c) {
    case ' ': return 0b00100000;
    case '!': return 0b00100001;
    case '"': return 0b00100010;
    case '#': return 0b00100011;
    case '$': return 0b00100100;
    case '%': return 0b00100101;
    case '&': return 0b00100110;
    case '\'': return 0b00100111;
    case '(': return 0b00101000;
    case ')': return 0b00101001;
    case '*': return 0b00101010;
    case '+': return 0b00101011;
    case ',': return 0b00101100;
    case '-': return 0b00101101;
    case '.': return 0b00101110;
    case '/': return 0b00101111;
    case '0': return 0b00110000;
    case '1': return 0b00110001;
    case '2': return 0b00110010;
    case '3': return 0b00110011;
    case '4': return 0b00110100;
    case '5': return 0b00110101;
    case '6': return 0b00110110;
    case '7': return 0b00110111;
    case '8': return 0b00111000;
    case '9': return 0b00111001;
    case ':': return 0b00111010;
    case ';': return 0b00111011;
    case '<': return 0b00111100;
    case '=': return 0b00111101;
    case '>': return 0b00111110;
    case '?': return 0b00111111;
    case '@': return 0b01000000;
    case 'A': return 0b01000001;
    case 'B': return 0b01000010;
    case 'C': return 0b01000011;
    case 'D': return 0b01000100;
    case 'E': return 0b01000101;
    case 'F': return 0b01000110;
    case 'G': return 0b01000111;
    case 'H': return 0b01001000;
    case 'I': return 0b01001001;
    case 'J': return 0b01001010;
    case 'K': return 0b01001011;
    case 'L': return 0b01001100;
    case 'M': return 0b01001101;
    case 'N': return 0b01001110;
    case 'O': return 0b01001111;
    case 'P': return 0b01010000;
    case 'Q': return 0b01010001;
    case 'R': return 0b01010010;
    case 'S': return 0b01010011;
    case 'T': return 0b01010100;
    case 'U': return 0b01010101;
    case 'V': return 0b01010110;
    case 'W': return 0b01010111;
    case 'X': return 0b01011000;
    case 'Y': return 0b01011001;
    case 'Z': return 0b01011010;
    case '[': return 0b01011011;
    case ']': return 0b01011101;
    case '^': return 0b01011110;
    case '_': return 0b01011111;
    case '`': return 0b01100000;
    case 'a': return 0b01100001;
    case 'b': return 0b01100010;
    case 'c': return 0b01100011;
    case 'd': return 0b01100100;
    case 'e': return 0b01100101;
    case 'f': return 0b01100110;
    case 'g': return 0b01100111;
    case 'h': return 0b01101000;
    case 'i': return 0b01101001;
    case 'j': return 0b01101010;
    case 'k': return 0b01101011;
    case 'l': return 0b01101100;
    case 'm': return 0b01101101;
    case 'n': return 0b01101110;
    case 'o': return 0b01101111;
    case 'p': return 0b01110000;
    case 'q': return 0b01110001;
    case 'r': return 0b01110010;
    case 's': return 0b01110011;
    case 't': return 0b01110100;
    case 'u': return 0b01110101;
    case 'v': return 0b01110110;
    case 'w': return 0b01110111;
    case 'x': return 0b01111000;
    case 'y': return 0b01111001;
    case 'z': return 0b01111010;
    case '{': return 0b01111011;
    case '|': return 0b01111100;
    case '}': return 0b01111101;
    default: return 0b000010000;
  } 
}

inline void LCD::waitForBusyFlag() {
  delay(4);
}

LCD lcd(10, 9, 8, 7, 6, 5, 4);

void setup() {
  Serial.begin(115200);

  lcd.displayCurrentConfig();
  lcd.initializePins();  
  lcd.initializeBus();

  lcd.functionSet(true, false);
  lcd.displayControl(true, true, false);
  
  lcd.clearScreen();
  lcd.sendString("Hello World");
  delay(1000);
  lcd.clearScreen();

}

void loop() {
  String text = Serial.readString();
  
  if(text == "") return;

  lcd.clearScreen();
  lcd.sendString(text.c_str());    
}
