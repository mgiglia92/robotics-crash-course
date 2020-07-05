void setup() {

Serial.begin(9600);

}

void loop() {
  int sum = add(4, 3);
  Serial.println(sum);
}

int add(int a, int b)
{
  return a+b;
}
