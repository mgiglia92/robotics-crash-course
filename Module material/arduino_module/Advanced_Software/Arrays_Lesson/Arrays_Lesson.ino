// Lesson on Arrays

// Prints the array passed to it
void printArray(int arr[], int len){
  for(int i = 0; i < len - 1; i++){
    Serial.print(arr[i]);
    Serial.print(",");
  }
  Serial.println(arr[len-1]);
}


void setup() {
    // ------ Arrays -------
    // Both ways initialize the same array
    int arr[] = {1,2,3,4,5,6};
    int arr2[6];
    arr2[0] = 1;
    arr2[1] = 2;
    arr2[2] = 3;
    arr2[3] = 4;
    arr2[4] = 5;
    arr2[5] = 6;

    // ------ While Loops ------
    // I could initialize the same array using a while loop
    int value = 1;
    int arr3[6];
    while(value < 7){
        arr3[value - 1] = value;
        value++;
    }
    
    // ------- For Loops ---------
    int arr4[6];
    for(int i = 1; i < 7; i++){
        arr4[i - 1] = i;
    }

    Serial.begin(9600);
    
    // Arrays use pointers - 
    // Look what happens when I print the variable "arr" is pointing to.
    Serial.println(*arr);

    // All the arrays are the same even though they were created differently.
    printArray(arr, (sizeof(arr) / sizeof(arr[0])));
    printArray(arr2, (sizeof(arr2) / sizeof(arr2[0])));
    printArray(arr3, (sizeof(arr3) / sizeof(arr3[0])));
    printArray(arr4, (sizeof(arr4) / sizeof(arr4[0])));
}

void loop() {
  // put your main code here, to run repeatedly:

}
