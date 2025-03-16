/* Basic NIVLang program 
 * Basic calculator functions and simple menu
 */

# Global variables
float PI = 3.14159;
int MAX_INPUTS = 10;

fun add(float a, float b) {
    return a + b;
}

fun subtract(float a, float b) {
    return a - b;
}

fun multiply(float a, float b) {
    return a * b;
}

fun divide(float a, float b) {
    if (b == 0.0) {
        print("Error: Division by zero");
        return 0.0;
    }
    return a / b;
}

fun calculateArea(float radius) {
    return PI * radius * radius;
}

fun calculateVolume(float radius) {
    return (4.0 / 3.0) * PI * radius * radius * radius;
}

fun printMenu() {
    print("Simple Calculator");
    print("1. Add two numbers");
    print("2. Subtract two numbers");
    print("3. Multiply two numbers");
    print("4. Divide two numbers");
    print("5. Calculate circle area");
    print("6. Calculate sphere volume");
    print("0. Exit");
    print("Enter your choice: ");
}

fun getInputs() {
    float a = 0.0;
    float b = 0.0;
    
    print("Enter first number: ");
    a = input();
    
    print("Enter second number: ");
    b = input();
    
    return a, b;
}

fun main() {
    int choice = -1;
    float result = 0.0;
    
    while (choice != 0) {
        printMenu();
        choice = input();
        
        if (choice == 1) {
            float a = 0.0;
            float b = 0.0;
            a, b = getInputs();
            result = add(a, b);
            print("Result: " + result);
        }
        else if (choice == 2) {
            float a = 0.0;
            float b = 0.0;
            a, b = getInputs();
            result = subtract(a, b);
            print("Result: " + result);
        }
        else if (choice == 3) {
            float a = 0.0;
            float b = 0.0;
            a, b = getInputs();
            result = multiply(a, b);
            print("Result: " + result);
        }
        else if (choice == 4) {
            float a = 0.0;
            float b = 0.0;
            a, b = getInputs();
            result = divide(a, b);
            print("Result: " + result);
        }
        else if (choice == 5) {
            float radius = 0.0;
            print("Enter radius: ");
            radius = input();
            result = calculateArea(radius);
            print("Circle area: " + result);
        }
        else if (choice == 6) {
            float radius = 0.0;
            print("Enter radius: ");
            radius = input();
            result = calculateVolume(radius);
            print("Sphere volume: " + result);
        }
        else if (choice == 0) {
            print("Exiting calculator...");
        }
        else {
            print("Invalid choice, try again.");
        }
    }
    
    return 0;
}