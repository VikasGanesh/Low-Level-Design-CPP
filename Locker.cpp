
#include <iostream>
#include <string>
#include <vector>
#include <unordered_map>
#include <chrono>
#include <ctime>
#include <cstdlib>
#include <memory>

using namespace std;
using namespace std::chrono;

// ----------- Enums -----------

enum class LockerSize { SMALL, MEDIUM, LARGE };
enum class PackageSize { SMALL, MEDIUM, LARGE };
enum class LockerStatus { CLOSED, BOOKED, AVAILABLE };

// ----------- Notification -----------

class Notification {
public:
    virtual bool setNotification(string message) = 0;
};

class EmailNotification : public Notification {
public:
    bool setNotification(string message) override {
        cout << "Email: " << message << endl;
        return true;
    }
};

class MessageNotification : public Notification {
public:
    bool setNotification(string message) override {
        cout << "Message: " << message << endl;
        return true;
    }
};

// ----------- Item & Order -----------

class Item {
    int item_id, quantity;
public:
    Item(int item_id, int quantity) : item_id(item_id), quantity(quantity) {}
    int getItemId() { return item_id; }
};

class Order {
    int order_id;
    vector<Item*> items;
public:
    Order(int order_id, vector<Item*> items) : order_id(order_id), items(items) {}
    vector<Item*> getItems() { return items; }
};

// ----------- Code Generation -----------

class CodeGenration {
public:
    virtual string generateCode() = 0;
};

class RandomCodeGenration : public CodeGenration {
public:
    string generateCode() override {
        return to_string(1000 + rand() % 9000);
    }
};

// ----------- Package -----------

class Package {
    int package_id;
    Order* order;
    PackageSize packageSize;
public:
    Package(int package_id, Order* order, PackageSize size)
        : package_id(package_id), order(order), packageSize(size) {}

    virtual void pack() {}
    PackageSize getPackageSize() { return packageSize; }
    Order* getOrder() { return order; }
};

class LockerPackage : public Package {
    string code;
    time_t validity_till;
    int locker_id;
    CodeGenration* codegen;
public:
    LockerPackage(int package_id, Order* order, PackageSize size,
                  int locker_id, time_t valid_till, CodeGenration* gen)
        : Package(package_id, order, size), locker_id(locker_id),
          validity_till(valid_till), codegen(gen) {}

    void pack() override {
        this->code = codegen->generateCode();
    }

    bool validateCode(const string& inputCode) { return inputCode == code; }
    string getCode() { return code; }
};

// ----------- Locker -----------

class Locker {
    int id;
    LockerSize size;
    LockerStatus status;
    LockerPackage* package = nullptr;
public:
    Locker(int id, LockerSize size, LockerStatus status)
        : id(id), size(size), status(status) {}

    LockerSize getSize() { return size; }
    LockerStatus getStatus() { return status; }
    int getId() { return id; }

    bool isAvailable() { return status == LockerStatus::AVAILABLE; }

    void placePackage(LockerPackage* pkg) {
        package = pkg;
        status = LockerStatus::BOOKED;
    }

    LockerPackage* pickUpPackage(string code) {
        if (!package || !package->validateCode(code)) return nullptr;
        LockerPackage* ret = package;
        package = nullptr;
        status = LockerStatus::AVAILABLE;
        return ret;
    }
};

// ----------- LockerLocation -----------

class LockerLocation {
    int id;
    vector<Locker*> lockers;
public:
    LockerLocation(int id) : id(id) {}

    void addLocker(Locker* locker) {
        lockers.push_back(locker);
    }

    Locker* getAvailableLocker(PackageSize size) {
        LockerSize reqSize = static_cast<LockerSize>(size);
        for (auto& locker : lockers) {
            if (locker->getSize() == reqSize && locker->isAvailable()) {
                return locker;
            }
        }
        return nullptr;
    }

    int getId() { return id; }
};

// ----------- Users -----------

class User {
protected:
    string email, phone;
public:
    User(string email, string phone) : email(email), phone(phone) {}
};

class Customer : public User {
    int customer_id;
    vector<Notification*> notifications;
public:
    Customer(int id, string email, string phone, vector<Notification*> notifs)
        : User(email, phone), customer_id(id), notifications(notifs) {}

    void receiveCode(string code) {
        for (auto notif : notifications) {
            notif->setNotification("Your locker code is: " + code);
        }
    }

    void collectPackage(Locker* locker, string code) {
        LockerPackage* pkg = locker->pickUpPackage(code);
        if (pkg) cout << "Customer collected package successfully!\n";
        else cout << "Invalid code. Cannot collect package.\n";
    }
};

class DeliveryAgent : public User {
    int agent_id;
public:
    DeliveryAgent(int id, string email, string phone) : User(email, phone), agent_id(id) {}

    void placePackageInLocker(Locker* locker, LockerPackage* pkg) {
        locker->placePackage(pkg);
        cout << "Delivery agent placed the package in locker #" << locker->getId() << endl;
    }
};

// ----------- OrderService -----------

class OrderService {
public:
    static Order* createOrder(vector<Item*> items) {
        return new Order(rand(), items);
    }

    static LockerPackage* packOrder(Order* order, Locker* locker, PackageSize size, CodeGenration* codegen) {
        LockerPackage* pkg = new LockerPackage(rand(), order, size, locker->getId(), system_clock::to_time_t(system_clock::now()) + 86400, codegen);
        pkg->pack();
        return pkg;
    }

    static void sendNotification(Customer* customer, string code) {
        customer->receiveCode(code);
    }
};

// ----------- Locker Management System -----------

class LockerManagementSystem {
    unordered_map<int, LockerLocation*> locations;

    LockerManagementSystem() {}
public:
    static LockerManagementSystem& getInstance() {
        static LockerManagementSystem instance;
        return instance;
    }

    void addLocation(LockerLocation* loc) {
        locations[loc->getId()] = loc;
    }

    LockerLocation* getLocation(int id) {
        return locations.count(id) ? locations[id] : nullptr;
    }
};

// ----------- Main Simulation -----------

class LockerOrderSystem {
public:
    void placeOrder(Customer* customer, vector<Item*> items, LockerLocation* location, PackageSize size) {
        // 1. Create the order
        Order* order = OrderService::createOrder(items);

        // 2. Find an available locker
        Locker* locker = location->getAvailableLocker(size);
        if (!locker) {
            cout << "No available locker for the given package size.\n";
            return;
        }

        // 3. Pack the order and generate the locker package
        CodeGenration* gen = new RandomCodeGenration();
        LockerPackage* pkg = OrderService::packOrder(order, locker, size, gen);

        // 4. Place the package in the locker
        DeliveryAgent agent(101, "agent@delivery.com", "9876543210");
        agent.placePackageInLocker(locker, pkg);

        // 5. Send notification to the customer
        OrderService::sendNotification(customer, pkg->getCode());

        // 6. Customer collects the package
        customer->collectPackage(locker, pkg->getCode());
    }
};

int main() {
    srand(time(nullptr));
    LockerManagementSystem& lms = LockerManagementSystem::getInstance();

    // Setup Locker locations and lockers
    auto* location = new LockerLocation(1);
    location->addLocker(new Locker(101, LockerSize::SMALL, LockerStatus::AVAILABLE));
    location->addLocker(new Locker(102, LockerSize::MEDIUM, LockerStatus::AVAILABLE));
    lms.addLocation(location);

    // Customer places an order
    vector<Notification*> notifs = { new EmailNotification() };
    Customer customer(1, "vikas@example.com", "1234567890", notifs);
    vector<Item*> items = { new Item(1, 2) };

    // Place the order and simulate the full process
    LockerOrderSystem lockerOrderSystem;
    lockerOrderSystem.placeOrder(&customer, items, location, PackageSize::SMALL);

    return 0;
}
