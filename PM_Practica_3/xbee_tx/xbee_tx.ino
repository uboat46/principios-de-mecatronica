#include <XBee.h>

XBee xbee = XBee();
uint8_t payload[] = {0 , 0};
Tx16Request tx = Tx16Request(0x1874, payload, sizeof(payload));
//XBeeResponse response = XBeeResponse();
//ZBRxResponse rx = ZBRxResponse();

void setup() {
  Serial.begin(9600);
  xbee.setSerial(Serial);
}

void loop() {
    // xbee.send(tx);
//  xbee.readPacket();
//  if(xbee.getResponse().isAvailable()) {
//    if(xbee.getResponse().getApiId() == ZB_RX_RESPONSE) {
//      xbee.getResponse().getZBRxResponse(rx);
//      for (int i = 0; i < rx.getDataLength(); i++) {
//        Serial.print(rx.getData(i), HEX);
//        Serial.print(", ");
//      }
//      Serial.println("");
//    }
//  }
}
