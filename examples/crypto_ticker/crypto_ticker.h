#include <pgmspace.h>  // PROGMEM support header

String VERSION = "v1.0.0-alpha-3";

#define WIFI_TIMEOUT 10 // Second

#define SDA 4
#define SCL 5
#define AP_SSID     "Crypto-Ticker"
#define AP_PASSWORD "cryptoticker"

#define LOOP_DELAY        10000 // 100 ms
#define REFLASH_DELAY     10000 // 10 Second
#define RETRY_COUNT       10

String DOLLOR = "$";
const char* HOSTNAME = "crypto-ticker";


unsigned int COIN_COLORS[COINS_LENGTH] = {
  0xF443, // bitcoin
  0x4A4E, // ethereum
  0xED85, // binance-coin
  0x241F, // cardano
  0x4D52, // tether
  0xC808, // polkadot
  0xFFDF, // xrp
  0xF80E, // uniswap
  0xEF5D, // litecoin
  0x2A9A, // chainlink
};
