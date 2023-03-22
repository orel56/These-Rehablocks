/*
\author Quentin.M & Abbas.R
*/
#include <Xaal.h>
#include <Arduino.h>
#include <EEPROM.h>

#include <config.h>
#include <byteswap.h>

#include <YACL.h>
#include <WiFi.h>
#include <WiFiUdp.h>
WiFiUDP mcast;
WiFiUDP ntpUDP;

#include <NTPClient.h>
NTPClient timeClient(ntpUDP, "europe.pool.ntp.org", 0, 300 * 1000);

#include <Crypto.h>
#include <ChaChaPoly.h>
ChaChaPoly chacha;

const uint8_t XAAL_KEY[] = {0xd1, 0xd2, 0x78, 0x9b, 0xd5, 0x16, 0x49, 0x3c, 0x25, 0xfa, 0x60, 0xcf, 0x83, 0x08, 0x94, 0x7a, 0x1c, 0xdf, 0x00, 0x23, 0xfd, 0x27, 0xf2, 0x94, 0x59, 0x98, 0xc7, 0x1c, 0xdd, 0x3f, 0x7f, 0x01};



const char* SSID     = "Aurel";
const char* PASSWORD = "mpiphone";

const uint8_t UUID[] = {0x46, 0xf8, 0x18, 0x9b, 0x38, 0x4e, 0x4c, 0x13, 0x93, 0xde, 0xce, 0x5d, 0xef, 0x84, 0x9f, 0xc0};


int pot_val=0;

#define IETF_ABITES  16

typedef union {
  unsigned char buf[12];
  struct {
    uint64_t sec;
    uint32_t usec; };
} nonce_t;

class Message {
   public:
    const uint8_t *source;
    const char    *dev_type;
    int            msg_type = 0;
    const char    *action;
    CBORPair       body;
    Message();
    void dump();
};

Message::Message() {
}

void Message::dump() {
  Serial.printf("msg_type: %d action: %s\n",msg_type,action);
}

void hexdump(const uint8_t *buf,int size) {
  Serial.print("[");
  for (size_t i=0 ; i < size ; ++i) {
    Serial.print("0x");
	Serial.print(buf[i], HEX);
    Serial.print(",");
	}
  Serial.print("]");
}

void sendMessage(Message msg) {
  unsigned long long sec;
  unsigned long usec;
  nonce_t nonce;
  uint8_t *cypher; 
  uint16_t size;
  
  if (WiFi.status() != WL_CONNECTED) {
    Serial.print("# Error: no network\n");
    return;
  }

   CBORArray data = CBORArray();
  // ------------- headers -------------
  // version 
	data.append(7);
  // timestamp
  sec = timeClient.getEpochTime();
  usec = micros();
  data.append(sec);
  data.append(usec);

  // target is a list of address in bytes format.
  // in CBOR an empty list = 0x80, encoded in byte format, this shoud be [0x41,0x80]
  CBOR targets = CBOR();
  const uint8_t * ad = CBORArray(0).to_CBOR();
  targets.encode(ad, 1);
  data.append(targets);

  // ------------- payload -------------
  // source uuid address
  CBORArray buf = CBORArray();
  CBOR source = CBOR();
  source.encode(msg.source,16);
  buf.append(source);
  buf.append(msg.dev_type);
  buf.append(msg.msg_type);
  buf.append(msg.action);
  if (msg.body.length()!=0)
    buf.append(msg.body);

  // ------------- cyphering -------------
  chacha.clear();
  chacha.setKey(XAAL_KEY,32);
  // Nonce 
  nonce.sec =  __bswap_64(sec);
  nonce.usec = __bswap_32(usec);

  chacha.setIV(nonce.buf,12);
  // additionnal data
  chacha.addAuthData(CBORArray(0).to_CBOR(),1);
  // let's cipher & tag the buf
  size = buf.length();
  cypher = (uint8_t *) malloc(sizeof(uint8_t) * (size + IETF_ABITES));
  chacha.encrypt(cypher,(const uint8_t*)buf.to_CBOR(),size);
  // in combined mode ChachaPoly provide auth tag after ciphered data
  chacha.computeTag(cypher+size,IETF_ABITES);
  size = size + IETF_ABITES;

  // adding  cyphered payload
  CBOR tmp = CBOR();
  tmp.encode(cypher,size);
  data.append(tmp);
  
  // ------------- mcast sending ------------
  const uint8_t *cbor_encoded = data.to_CBOR();
  //hexdump(cbor_encoded,data.length());
  mcast.beginMulticastPacket();
  mcast.write(cbor_encoded,data.length());
  mcast.endPacket();
  Serial.print("Sent msg: " );
  msg.dump();
}

void sendAlive() {
  Message msg = Message();
  msg.source = UUID;
  msg.dev_type = "esp.basic";
  msg.action = "alive";
  msg.msg_type = 0; // REPLY
  msg.body.append("timeout",600);
  sendMessage(msg);
}

void sendDescription() {
  Message msg = Message();
  msg.source = UUID;
  msg.dev_type = "esp.basic";
  msg.msg_type = 2; // REPLY
  msg.action = "get_description";
  msg.body.append("vendor_id","Expressif");
  msg.body.append("product_id","ESP32 DEV C");
  msg.body.append("info",WiFi.localIP().toString().c_str());
  sendMessage(msg);
}

void sendStatus() {
  static uint16_t old_val = 0;
  uint16_t val_trig = 50;
  pot_val=analogRead(A0);

  //Serial.print("Reading: ");            // Prints weight readings in .2 decimal kg units.
  //Serial.println(" kg");

   if ( abs(pot_val - old_val) > val_trig){
    //float volt= pot_val/1241;

    Message msg = Message();
    msg.source = UUID;
    msg.dev_type = "esp.basic";
    msg.msg_type = 0; 

    msg.action = "attributes_change";
    msg.body.append("value", pot_val);
    sendMessage(msg);
    old_val = pot_val;
  }
}


void wifiInit() {
  Serial.print("# Init WiFi\n");
  WiFi.begin(SSID, PASSWORD);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
  }
  Serial.print("# WiFi connected\n");
  Serial.print("# IP address: ");
  Serial.println(WiFi.localIP());
  mcast.beginMulticast(IPAddress(IP_ADDR),PORT);
}

void ntpInit() {
  timeClient.update();
  Serial.println("# Time : " + timeClient.getFormattedTime());
}

void xaal_init()
{
  Serial.begin(115200);   // Starts serial communication in 115200 baud rate.
  
  wifiInit();
  ntpInit();
  Serial.println("# Boot Ok");
}

void xaal_tick(){
  static unsigned long last_alive,last_attribute = 0;
  unsigned long now;

  timeClient.update();
  now = timeClient.getEpochTime();

  if (now > (last_alive + 400)) {
    sendAlive();
    sendDescription();
    last_alive = now;
  }

  if (now > (last_attribute + 2)) {
    sendStatus();
    last_attribute = now;
  }

  delay(10);
}

void xaal_get(){};
void xaal_set(){};