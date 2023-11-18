#ifndef CONFIG_H
#define CONFIG_H

// radio module parameters
#define CFG_LORA_FREQ         433.775E6
#define CFG_LORA_PWR          10

// modulation selection
#define CFG_MOD_LORA          0
#define CFG_MOD_FSK           1
#ifdef CFG_USE_FSK
#define CFG_MOD               CFG_MOD_FSK
#else
#define CFG_MOD               CFG_MOD_LORA
#endif

// lora parameters
#define CFG_LORA_SYNC_WORD    0x12
#define CFG_LORA_BW           125.00e3
#define CFG_LORA_SF           7
#define CFG_LORA_CR           6
#define CFG_LORA_CRC          1
#define CFG_LORA_EXPLICIT     true
#define CFG_LORA_PREAMBLE     8

// fsk parameters
#define CFG_FSK_BIT_RATE      4.8
#define CFG_FSK_FREQ_DEV      1.2
#define CFG_FSK_RX_BW         9.7

// lora pinouts
#define CFG_LORA_PIN_NSS      5
#define CFG_LORA_PIN_RST      4
#define CFG_LORA_PIN_DIO0     3
#define CFG_LORA_PIN_DIO1     2

// generic parameters
#define CFG_MY_CALLSIGN       "N0CALL-11"

#endif // CONFIG_H