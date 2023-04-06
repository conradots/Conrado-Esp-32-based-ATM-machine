include <WiFi.h>
#include <WiFiClientSecure.h>
#include <UniversalTelegramBot.h>
#include <ArduinoJson.h>
#include <HTTPClient.h>
const char* ssid = "Ammmmm";
const char* password = "yadnyesh2289";
//define chat_id and bottoken
#define BOTtoken "5924533106:AAEeuVoDofcxB71JC0U4CP_txzfCLNzs0bU"
#define CHAT_ID "1783892261"
int otp;
int v;
int otp_ip[9];
int amount_ip[9];
int withdraw_amount[5];
int pinOrder[9] = { 4, 2, 15, 13, 12, 14, 27, 33, 32 };
int incorrect_otp_flag = 0;
int authorised_flag = 0;
int amount = 15000;
int notesleft[] = { 5, 10, 10 };
int denominations[] = { 200, 100, 50 };
int transactions[5];
int transactionCount = 0;
WiFiClientSecure client;
UniversalTelegramBot bot(BOTtoken, client);
// Checking for new commands every 1 second.
int botRequestDelay = 1000;
unsigned long lastTimeBotRan;
//function to return the touch value inputted from touch pins
int getTouchValue(String val) {
//for otp input
if (val == "otp input") {
for (int x = 0; x < 9; x++)
otp_ip[x] = touchRead(pinOrder[x]);
return 0;
}
//for amount input
else if (val == "amount_ip") {
int retval = 99;
Serial.println("enter");
for (int x = 0; x < 9; x++)
amount_ip[x] = touchRead(pinOrder[x]);
for (int y = 0; y < 9; y++) {
if (amount_ip[y] < 60 && y != 1)
retval = y;
}
return retval;
}
}
void newCommand(int numNewMessages) {
Serial.println("New Command Found");
for (int i = 0; i < numNewMessages; i++) {
String chat_id = String(bot.messages[i].chat_id);
if (chat_id != CHAT_ID) {
bot.sendMessage(chat_id, "Unauthorized user", "");
continue;
}
String text = bot.messages[i].text;
Serial.println(text);
if (text == "/start") {
String welcome = "Welcome, \n";
welcome += "Use the following commands to control your outputs.\n\n";
welcome += "/login to login into the atm.\n";
welcome += "/withdraw to withdraw money.\n";
welcome += "/balance to check your account balance.\n";
welcome += "/records to get a record of previous transactions.\n";
welcome += "/userlogin to login by username.\n";
welcome += "/deposit to deposit money.\n";
bot.sendMessage(chat_id, welcome, "");
}
if (text == "/userlogin") {
bot.sendMessage(chat_id, "Please enter username.\n", "");
}
if (text == "yadnyesh") {
bot.sendMessage(chat_id, "Please type your password.\n", "");
}
if (text == "kolte") {
bot.sendMessage(chat_id, "Your account is been verified. Type /withdraw or /deposit.\n", "");
authorised_flag = 1;
}
if (text == "/login") {
otp = random(1, 9);
bot.sendMessage(chat_id, "Your OTP for login is ..." + String(otp), "");
delay(9000);
int ff = getTouchValue("otp input");
for (int k = 0; k < 8; k++) {
Serial.println(otp_ip[k]);
}
for (int k = 0; k < 8 && k != otp / 10 && k != otp % 10; k++) {
if (otp_ip[k] <= 50) {
incorrect_otp_flag = 1;
break;
}
}
if ((incorrect_otp_flag == 0) && (otp_ip[otp % 10] <= 50) && (otp_ip[otp / 10] <= 50)) {
bot.sendMessage(chat_id, "Welcome! You have been ... authorised!", "");
authorised_flag = 1;
} else {
bot.sendMessage(chat_id, "Incorrect OTP. Please ... try and login again", "");
}
}
if (text == "/withdraw") {
if (authorised_flag == 0)
bot.sendMessage(chat_id, "Unauthorised User", "");
else {
bot.sendMessage(chat_id, "Input the amount using touch pins when instructed. Don't press anything if you dont wish to
enter a digit", "");
for (int countDigit = 0; countDigit < 5; countDigit++) {
bot.sendMessage(chat_id, "Enter ... digit" + String(countDigit), "");
delay(1000);
withdraw_amount[countDigit] = getTouchValue("amount_ip");
delay(1000);
}
int sum = 0;
for (int countDigit = 0; countDigit < 5; countDigit++) {
if (withdraw_amount[countDigit] != 99)
sum = (sum * 10) + withdraw_amount[countDigit];
}
bot.sendMessage(chat_id, String(sum), "");
if (sum % 100 == 0) {
bot.sendMessage(chat_id, "Amount withdrawn is ..." + String(sum), "");
amount = amount - sum;
bot.sendMessage(chat_id, "Remaining balance is ..." + String(amount), "");
int temp = sum;
int i = 0;
for (i = 0; i < 3; i++) {
while (notesleft[i] > 0 && temp > denominations[i]) {
temp = temp - denominations[i];
notesleft[i] = notesleft[i] - 1;
}
if (temp == 0)
break;
}
Serial.print(notesleft[0]);
Serial.print(notesleft[1]);
Serial.print(notesleft[2]);
transactions[transactionCount] = sum;
transactionCount++;
} else
bot.sendMessage(chat_id, "Invalid amount. Please ...withdraw again", "");
}
}
if (text == "/deposit") {
if (authorised_flag == 0)
bot.sendMessage(chat_id, "Unauthorised User", "");
else {
bot.sendMessage(chat_id, "Input the amount using touch pins when instructed. Don't press anything if you dont wish to
enter a digit", "");
for (int countDigit = 0; countDigit < 5; countDigit++) {
bot.sendMessage(chat_id, "Enter ... digit" + String(countDigit), "");
delay(1000);
withdraw_amount[countDigit] = getTouchValue("amount_ip");
delay(1000);
}
int sum = 0;
for (int countDigit = 0; countDigit < 5; countDigit++) {
if (withdraw_amount[countDigit] != 99)
sum = (sum * 10) + withdraw_amount[countDigit];
}
bot.sendMessage(chat_id, String(sum), "");
if (sum % 100 == 0) {
bot.sendMessage(chat_id, "Amount diposited is ..." + String(sum), "");
amount = amount + sum;
bot.sendMessage(chat_id, "Actual Balance balance is ..." + String(amount), "");
int temp = sum;
int i = 0;
for (i = 0; i < 3; i++) {
while (notesleft[i] > 0 && temp > denominations[i]) {
temp = temp - denominations[i];
notesleft[i] = notesleft[i] - 1;
}
if (temp == 0)
break;
}
Serial.print(notesleft[0]);
Serial.print(notesleft[1]);
Serial.print(notesleft[2]);
transactions[transactionCount] = sum;
transactionCount++;
} else
bot.sendMessage(chat_id, "Invalid amount. Please ...withdraw again", "");
}
}
if (text == "/balance") {
if (authorised_flag == 0)
bot.sendMessage(chat_id, "Unauthorised User", "");
else {
String avail_bal = "Balance Available is ..." + String(amount);
bot.sendMessage(chat_id, avail_bal, "");
}
}
if (text == "/records") {
String ans = "You have " + String(transactionCount) + " ...previous transactions ";
String rec;
for (int l = 0; l < transactionCount; l++)
rec = rec + "Rs " + String(transactions[l]) + ",";
bot.sendMessage(chat_id, ans, "");
bot.sendMessage(chat_id, rec, "");
}
}
}
void setup() {
Serial.begin(115200);
#ifdef ESP8266
client.setInsecure();
#endif
pinMode(2, INPUT);
pinMode(4, INPUT);
pinMode(15, INPUT);
pinMode(13, INPUT);
pinMode(12, INPUT);
pinMode(14, INPUT);
pinMode(27, INPUT);
pinMode(33, INPUT);
pinMode(32, INPUT);
// Connect to Wi−Fi
WiFi.mode(WIFI_STA);
WiFi.begin(ssid, password);
client.setCACert(TELEGRAM_CERTIFICATE_ROOT);
WiFi.setSleep(false);
while (WiFi.status() != WL_CONNECTED) {
delay(1000);
Serial.println("Connecting to WiFi..");
}
// Print ESP32 Local IP Address
Serial.println(WiFi.localIP());
}
void loop() {
if (millis() > lastTimeBotRan + botRequestDelay) {
int numNewMessages = bot.getUpdates(bot.last_message_received + 1);
while (numNewMessages) {
Serial.println("got response");
newCommand(numNewMessages);
numNewMessages = bot.getUpdates(bot.last_message_received + 1);
}
lastTimeBotRan = millis();
}
}
/*
#include "WiFi.h"
#include <HTTPClient.h>
// WiFi credentials
const char* ssid = "Ammmmm"; // change SSID
const char* password = "yadnyesh2289"; // change password
*/