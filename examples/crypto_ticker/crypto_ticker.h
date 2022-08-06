
#define WIFI_TIMEOUT 10 // Second

#define SDA 4
#define SCL 5

#define FF20 &FreeSans24pt7b
#define FF19 &FreeSans18pt7b
#define FF18 &FreeSans12pt7b
#define FF17 &FreeSans9pt7b
#define GFXFF 1

#define LOOP_DELAY        10000 // 100 ms
#define REFLASH_DELAY     10000 // 10 Second
#define RETRY_COUNT       10

#define STATUS_BUSY  0
#define STATUS_IDLE  1
#define STATUS_ERROR 2
#define STATUS_DONE  3

uint16_t status_colors[] = {TFT_YELLOW, TFT_WHITE, TFT_RED, TFT_GREEN};

String DOLLOR = "$";

/*
 * Coins
 */
#define COINS_LENGTH 10
String COINS[COINS_LENGTH] = {
  "bitcoin",
  "ethereum",
  "binance-coin",
  "cardano",
  "tether",
  "polkadot",
  "xrp",
  "uniswap",
  "litecoin",
  "chainlink",
};
