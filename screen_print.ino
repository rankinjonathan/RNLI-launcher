void printToScreen(char* twitterText) {

  Serial.print("Point 4 -- ");
  Serial.println(twitterText);

  u8x8.clear();
//  u8x8.setFont(u8x8_font_chroma48medium8_r);
  u8x8.setCursor(0, 0);
  u8x8.print(twitterText);
  delay(1000);

  //    return;

}
