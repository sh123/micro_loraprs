#ifndef CONFIG_H
#define CONFIG_H

// lora default parameters
#define CFG_LORA_FREQ         433.750E6
#define CFG_LORA_SYNC_WORD    0x12
#define CFG_LORA_BW           31.25e3
#define CFG_LORA_SF           7
#define CFG_LORA_CR           6
#define CFG_LORA_PWR          10
#define CFG_LORA_CRC          1
#define CFG_LORA_EXPLICIT     true

// lora pinouts
#define CFG_LORA_PIN_NSS      5
#define CFG_LORA_PIN_RST      4
#define CFG_LORA_PIN_DIO0     3
#define CFG_LORA_PIN_DIO1     2

// generic parameters
#define CFG_MY_CALLSIGN       "N0CALL-11"

#endif // CONFIG_H