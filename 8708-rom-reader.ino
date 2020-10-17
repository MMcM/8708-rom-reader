
#define MAX_ADDR 0x7FF

#define ADDR_LOW_DDR DDRC
#define ADDR_LOW_PORT PORTC

#define ADDR_HIGH_DDR DDRB
#define ADDR_HIGH_PORT PORTB
#define ADDR_HIGH_MASK (MAX_ADDR>>8)

#define DATA_DDR DDRF
#define DATA_PIN PINF
#define DATA_PORT PORTF

#define ENABLE_DDR DDRE
#define ENABLE_PORT PORTE
#define ENABLE_OE (1<<7)
#define ENABLE_CE (1<<6)
#define ENABLE_MASK (ENABLE_OE|ENABLE_CE)

uint8_t read_rom_addr(uint16_t addr) {
  ADDR_LOW_PORT = addr & 0xFF;
  ADDR_HIGH_PORT = (addr >> 8) & ADDR_HIGH_MASK;

#if ENABLE_CE
  ENABLE_PORT &= ~ENABLE_CE;
#endif
#if ENABLE_OE
  ENABLE_PORT &= ~ENABLE_OE;
#endif
  delayMicroseconds(1);

  uint8_t data = DATA_PIN;

  ENABLE_PORT |= ENABLE_MASK;

  return data;
}

void setup() {
  ADDR_LOW_DDR = 0xFF;
  ADDR_HIGH_DDR |= ADDR_HIGH_MASK;

  DATA_DDR = 0;
  DATA_PORT |= 0xFF;
  
  ENABLE_DDR |= ENABLE_MASK;
  ENABLE_PORT |= ENABLE_MASK;

  Serial.begin(115200);
  while (!Serial) {
  }
}

void loop() {
  static uint16_t addr = 0;

  if ((addr & 0x00F) == 0) {
    Serial.println();
    if (addr < 0x100) {
      if (addr < 0x10) {
        Serial.print('0');
      }
      Serial.print('0');
    }
    Serial.print(addr, HEX);
    Serial.print(": ");
  }
  
  uint8_t data = read_rom_addr(addr++);
  if (data < 0x10) {
    Serial.print('0');
  }
  Serial.print(data, HEX);

  delay(100);

  if (addr > MAX_ADDR) {
    addr = 0;
  }
}
