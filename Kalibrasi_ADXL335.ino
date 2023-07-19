unsigned long int milli_time;
unsigned long int startTime = 0;
float xOffset = 0.0;   // offset pada sumbu X
float yOffset = 0.0;   // offset pada sumbu Y
float zOffset = 0.0;   // offset pada sumbu Z
bool isCalibrating = false;

void setup() {
  Serial.begin(230400);
  Serial.print("Time");
  Serial.print(",");
  Serial.print("X");
  Serial.print(",");
  Serial.print("Y");
  Serial.print(",");
  Serial.println("Z");

  Serial.setTimeout(10); // Set timeout for Serial input to 10ms

  // Menjalankan kalibrasi saat Arduino terhubung
  startCalibration();
}

void loop() {
  milli_time = millis();
  double pewaktu = 0.0;

  if (startTime > 0) {
    pewaktu = (milli_time - startTime) * 0.001;
  }

  double x_adc_value, y_adc_value, z_adc_value;
  double data1, data2, data3;

  // Menghitung data percepatan setelah kalibrasi
  x_adc_value = analogRead(A0); /* Nilai digital tegangan pada pin x_out */
  y_adc_value = analogRead(A1); /* Nilai digital tegangan pada pin y_out */
  z_adc_value = analogRead(A2); /* Nilai digital tegangan pada pin z_out */

  data1 = (((double)(x_adc_value * 5.0 / 1023.0) - 1.65 - xOffset) / 0.330); /* Percepatan pada sumbu X dalam satuan g */
  data2 = (((double)(y_adc_value * 5.0 / 1023.0) - 1.65 - yOffset) / 0.330); /* Percepatan pada sumbu Y dalam satuan g */
  data3 = (((double)(z_adc_value * 5.0 / 1023.0) - 1.80 - zOffset) / 0.330); /* Percepatan pada sumbu Z dalam satuan g */

  //Serial.print(pewaktu, DEC);
  //Serial.print(",");
  Serial.print(data1, DEC);
  Serial.print(",");
  Serial.print(data2, DEC);
  Serial.print(",");
  Serial.print(data3, DEC);
  Serial.println(" ");

  delay(0.01);
}

void startCalibration() {
  isCalibrating = true;
  xOffset = 0.0;
  yOffset = 0.0;
  zOffset = 0.0;

  Serial.println("Memulai kalibrasi...");
  Serial.println("Harap letakkan sensor pada posisi TOP dan diam.");

  delay(5000);

  for (int i = 0; i < 10; i++) {
    double x_adc_value = analogRead(A0);
    double y_adc_value = analogRead(A1);
    double z_adc_value = analogRead(A2);

    xOffset += (double)(x_adc_value * 5.0 / 1023.0) - 1.65;
    yOffset += (double)(y_adc_value * 5.0 / 1023.0) - 1.65;
    zOffset += (double)(z_adc_value * 5.0 / 1023.0) - 1.80;

    delay(10);
  }

  xOffset /= 10.0;
  yOffset /= 10.0;
  zOffset /= 10.0;

  zOffset -= 0.33;

  Serial.println("Kalibrasi selesai.");
  isCalibrating = false;

  startTime = millis(); // Mengatur waktu mulai setelah kalibrasi selesai
}
