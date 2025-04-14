// Enumeration
enum PaymentStatus {
  COMPLETED, 
  FAILED, 
  PENDING, 
  UNPAID, 
  REFUNDED
};

enum AccountStatus {
  ACTIVE, 
  CLOSED, 
  CANCELED, 
  BLACKLISTED, 
  NONE
};

// Custom Person data type class
class Person {
  private:
    string name;
    string address;
    string phone;
    string email;
};

// Custom Address data type class
class Address {
  private:
    int zipCode;
    string address;
    string city;
    string state;
    string country;
};

// ParkingSpot is an abstract class
class ParkingSpot {
  private:
    int id;
    bool isFree;
    Vehicle vehicle; 

  public:
    bool isFree(); 
    virtual bool assignVehicle(Vehicle vehicle) = 0; 
    bool removeVehicle(){
      // definition
    } 
};

class Handicapped : public ParkingSpot {
  public:
    bool assignVehicle(Vehicle vehicle) {
        // definition
    }
};

class Compact : public ParkingSpot {
  public:
    bool assignVehicle(Vehicle vehicle) {
        // definition
    }
};

class Large : public ParkingSpot {
  public: 
    bool assignVehicle(Vehicle vehicle) {
        // definition
    }
};

class Motorcycle : public ParkingSpot {
  public: 
    bool assignVehicle(Vehicle vehicle) {
        // definition
    }
};

// Vehicle is an abstract class
class Vehicle {
  private:
    string licenseNo;

  public:
    void virtual assignTicket(ParkingTicket ticket) = 0; 
};

class Car : public Vehicle {
  public:
    void assignTicket(ParkingTicket ticket) {
        // definition
    } 
};

class Van : public Vehicle {
  public:
    void assignTicket(ParkingTicket ticket) {
        // definition
    } 
};

class Truck : public Vehicle {
  public:
    void assignTicket(ParkingTicket ticket) {
        // definition
    } 
};

class MotorCycle : public Vehicle {
  public:
    void assignTicket(ParkingTicket ticket) {
        // definition
    } 
};

class Account {
  // Data members
  private: 
    string userName;
    string password;
    Person person; // Refers to an instance of the Person class
    AccountStatus status; // Refers to the AccountStatus enum

  public:
    virtual bool resetPassword() = 0;
};

class Admin : public Account {
  public: 
    // spot here refers to an instance of the ParkingSpot class
    bool addParkingSpot(ParkingSpot spot);
    // displayBoard here refers to an instance of the DisplayBoard class
    bool addDisplayBoard(DisplayBoard displayBoard);
    // entrance here refers to an instance of the Entrance class
    bool addEntrance(Entrance entrance);
    // exit here refers to an instance of the Exit class
    bool addExit(Exit exit;
  
    // Will implement the functionality in this class
    bool resetPassword() {
      // definition
    }
};

class ParkingAttendant : public Account {
  public: 
    bool processTicket(string ticketNumber);

    // Will implement the functionality in this class
    bool resetPassword() {
      // definition
    }
};

class DisplayBoard {
// Data members
  private:
    int id;
    std::vector<ParkingSpot*> parkingSpots;

  // Member functions
  public:
    DisplayBoard(int id) : id(id) {}
    void addParkingSpot(ParkingSpot* spot);
    void showFreeSlot();
};

class ParkingRate {
  // Data members
  private:
    double hours;
    double rate;

// Member function
  public: 
    void calculate();
};

class Entrance {
  // Data members 
  private:
    int id;

  // Member function
  public:
    ParkingTicket getTicket(); 
};

class Exit {
  // Data members 
  private:
    int id; 

  // Member function
  public:
    void validateTicket(ParkingTicket ticket);{
      // Perform validation logic for the parking ticket
      // Calculate parking charges, if necessary
      // Handle the exit process
    }
};

class ParkingTicket {
  private: 
    int ticketNo;
    time_t timestamp;
    time_t exit;
    double amount;
    bool status;
  
    // Following are the instances of their respective classes
    Vehicle vehicle;
    Payment payment;
    Entrance entrance;
    Exit exitIns;    
};

// Payment is an abstract class
class Payment {
    private:
        double amount;
        PaymentStatus status;
        time_t timestamp;

    public virtual bool initiateTransaction() = 0;
};

class Cash : public Payment {
    public bool initiateTransaction() {
        // definition
    }
};

class CreditCard : public Payment {
    public bool initiateTransaction() {
        // definition
    }
};

class ParkingLot {
    private:
        int id;
        string name;
        string address;
        ParkingRate parkingRate;

        map<string, Entrance> entrance;
        map<string, Exit> exit;

        // Create a hashmap that identifies all currently generated tickets using their ticket number
        map<string, ParkingTicket> tickets;

        // The ParkingLot is a singleton class that ensures it will have only one active instance at a time
        // Both the Entrance and Exit classes use this class to create and close parking tickets
        static ParkingLot parkingLot = NULL;

        // Created a private constructor to add a restriction (due to Singleton)
        ParkingLot() {
            // Call the name, address and parking_rate 
            // Create initial entrance and exit hashmaps respectively
        }

    // Created a static method to access the singleton instance of ParkingLot
    public:
        static ParkingLot getInstance() {
            if (parkingLot == NULL) {
                parkingLot = new ParkingLot();
            }
            return parkingLot;
        }

        bool addEntrance(Entrance entrance) {}
        bool addExit(Exit exit) {}

        // This function allows parking tickets to be available at multiple entrances
        ParkingTicket getParkingTicket(Vehicle vehicle) {}

        bool isFull(ParkingSpot type) {}
};

class CarRateCalculator : public IRateCalculator {
    vector<RateSlab> slabs = {
        {1, 20.0},
        {3, 15.0},
        {INT_MAX, 10.0}
    };

public:
    double calculateRate(int duration) override {
        double total = 0;
        int remainingHours = duration;
        int previousLimit = 0;

        for (auto& slab : slabs) {
            int slabHours = std::min(remainingHours, slab.upToHour - previousLimit);
            total += slabHours * slab.ratePerHour;
            remainingHours -= slabHours;
            previousLimit = slab.upToHour;

            if (remainingHours <= 0) break;
        }

        return total;
    }
};


int main() {
    // Step 1: Setup entrance and register with ParkingLot
    auto entrance = make_unique<Entrance>("E1");
    ParkingLot::getInstance().addEntrance("E1", std::move(entrance));

    // Step 2: Vehicle arrives
    Vehicle v("KA-01-AB-1234");

    // Step 3: Vehicle interacts with the entrance to get a ticket
    Entrance* e = ParkingLot::getInstance().getEntrance("E1");
    if (e) {
        ParkingTicket ticket = e->getTicket(v);
        cout << "Client: Received ticket number " << ticket.ticketNumber << endl;
    } else {
        cout << "Entrance not found!" << endl;
    }

    return 0;
}
