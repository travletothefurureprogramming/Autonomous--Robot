// Ορισμός των Pins του Αισθητήρα Υπερήχων (HC-SR04)
const int trigPin = 9;  // Trigger Pin
const int echoPin = 10; // Echo Pin

// Ορισμός των Pins του Driver κινητήρων (π.χ. L298N)
// Αριστερός κινητήρας
const int ENA = 5;
const int IN1 = 7;
const int IN2 = 6;
// Δεξιός κινητήρας
const int ENB = 11;
const int IN3 = 8;
const int IN4 = 4;

// Σταθερά απόστασης (σε cm) - Αν η απόσταση είναι μικρότερη από αυτή, θεωρείται εμπόδιο.
const int DISTANCE_THRESHOLD = 20;

void setup() {
    // Θέτει τα pins του αισθητήρα
    pinMode(trigPin, OUTPUT);
    pinMode(echoPin, INPUT);

    // Θέτει τα pins του driver κινητήρων ως εξόδους
    pinMode(ENA, OUTPUT);
    pinMode(IN1, OUTPUT);
    pinMode(IN2, OUTPUT);
    pinMode(ENB, OUTPUT);
    pinMode(IN3, OUTPUT);
    pinMode(IN4, OUTPUT);
    
    // Ρυθμίζει την ταχύτητα (PWM)
    analogWrite(ENA, 200); // 0-255
    analogWrite(ENB, 200); 
    
    Serial.begin(9600); // Εκκίνηση σειριακής επικοινωνίας για debugging
}

// Συνάρτηση για τη μέτρηση της απόστασης σε cm
long getDistance() {
    // Καθαρίζει το trigPin
    digitalWrite(trigPin, LOW);
    delayMicroseconds(2);
    
    // Στέλνει παλμό
    digitalWrite(trigPin, HIGH);
    delayMicroseconds(10);
    digitalWrite(trigPin, LOW);
    
    // Διαβάζει το echoPin, επιστρέφει τη διάρκεια του παλμού
    long duration = pulseIn(echoPin, HIGH);
    
    // Μετατρέπει τη διάρκεια σε απόσταση (cm). Η ταχύτητα του ήχου είναι 340m/s (0.034cm/μs)
    // Απόσταση = (Διάρκεια * 0.034) / 2
    long distance = duration * 0.034 / 2; 
    
    return distance;
}

// Συνάρτηση κίνησης: Προχώρα μπροστά
void moveForward() {
    digitalWrite(IN1, HIGH);
    digitalWrite(IN2, LOW);
    digitalWrite(IN3, HIGH);
    digitalWrite(IN4, LOW);
    Serial.println("Moving Forward");
}

// Συνάρτηση κίνησης: Σταμάτα
void stopRobot() {
    digitalWrite(IN1, LOW);
    digitalWrite(IN2, LOW);
    digitalWrite(IN3, LOW);
    digitalWrite(IN4, LOW);
    Serial.println("Stopped");
}

// Συνάρτηση κίνησης: Στρίψε δεξιά (π.χ. 300ms)
void turnRight() {
    digitalWrite(IN1, HIGH); // Αριστερός κινητήρας εμπρός
    digitalWrite(IN2, LOW);
    digitalWrite(IN3, LOW);  // Δεξιός κινητήρας πίσω (ή σταματημένος)
    digitalWrite(IN4, HIGH); 
    delay(300);
    stopRobot();
}

// Συνάρτηση κίνησης: Στρίψε αριστερά (π.χ. 300ms)
void turnLeft() {
    digitalWrite(IN1, LOW);  // Αριστερός κινητήρας πίσω (ή σταματημένος)
    digitalWrite(IN2, HIGH); 
    digitalWrite(IN3, HIGH); // Δεξιός κινητήρας εμπρός
    digitalWrite(IN4, LOW);
    delay(300);
    stopRobot();
}


void loop() {
    long currentDistance = getDistance();
    Serial.print("Distance: ");
    Serial.print(currentDistance);
    Serial.println(" cm");
    
    // Έλεγχος για εμπόδιο
    if (currentDistance <= DISTANCE_THRESHOLD) {
        stopRobot(); // 1. Σταμάτα
        delay(500); // Περίμενε μισό δευτερόλεπτο

        // 2. Στρίψε πίσω λίγο
        digitalWrite(IN1, LOW);
        digitalWrite(IN2, HIGH);
        digitalWrite(IN3, LOW);
        digitalWrite(IN4, HIGH);
        delay(300);
        stopRobot();

        // 3. Αποφάσισε πού να στρίψεις (Απλοποιημένο: Στρίψε τυχαία δεξιά)
        turnRight();
        
    } else {
        // Δεν υπάρχει εμπόδιο, προχώρα
        moveForward();
    }
    
    delay(100); // Μικρή καθυστέρηση μεταξύ των μετρήσεων
}
