#include <iostream>
#include <string>

// Product - The final object being constructed
class HotelRoom {
private:
    std::string bed;
    bool hasTV;
    bool hasMiniFridge;
    bool hasJacuzzi;
    bool hasBalcony;
public:
    void setBed(const std::string& b) { bed = b; }
    void setTV(bool tv) { hasTV = tv; }
    void setMiniFridge(bool fridge) { hasMiniFridge = fridge; }
    void setJacuzzi(bool jacuzzi) { hasJacuzzi = jacuzzi; }
    void setBalcony(bool balcony) { hasBalcony = balcony; }
    
    void showRoom() const {
        std::cout << "Room Configuration:\n";
        std::cout << "Bed: " << bed << "\n";
        std::cout << "TV: " << (hasTV ? "Yes" : "No") << "\n";
        std::cout << "Mini Fridge: " << (hasMiniFridge ? "Yes" : "No") << "\n";
        std::cout << "Jacuzzi: " << (hasJacuzzi ? "Yes" : "No") << "\n";
        std::cout << "Balcony: " << (hasBalcony ? "Yes" : "No") << "\n";
    }
};

// Builder Interface
class HotelRoomBuilder {
public:
    virtual void buildBed() = 0;
    virtual void buildTV() = 0;
    virtual void buildMiniFridge() = 0;
    virtual void buildJacuzzi() = 0;
    virtual void buildBalcony() = 0;
    virtual HotelRoom getRoom() = 0;
    virtual ~HotelRoomBuilder() = default;
};

// Concrete Builders
class BasicRoomBuilder : public HotelRoomBuilder {
private:
    HotelRoom room;
public:
    void buildBed() override { room.setBed("Single Bed"); }
    void buildTV() override { room.setTV(false); }
    void buildMiniFridge() override { room.setMiniFridge(false); }
    void buildJacuzzi() override { room.setJacuzzi(false); }
    void buildBalcony() override { room.setBalcony(false); }
    HotelRoom getRoom() override { return room; }
};

class DeluxeRoomBuilder : public HotelRoomBuilder {
private:
    HotelRoom room;
public:
    void buildBed() override { room.setBed("King Bed"); }
    void buildTV() override { room.setTV(true); }
    void buildMiniFridge() override { room.setMiniFridge(true); }
    void buildJacuzzi() override { room.setJacuzzi(false); }
    void buildBalcony() override { room.setBalcony(false); }
    HotelRoom getRoom() override { return room; }
};

class SuiteRoomBuilder : public HotelRoomBuilder {
private:
    HotelRoom room;
public:
    void buildBed() override { room.setBed("King Bed with Extra Comfort"); }
    void buildTV() override { room.setTV(true); }
    void buildMiniFridge() override { room.setMiniFridge(true); }
    void buildJacuzzi() override { room.setJacuzzi(true); }
    void buildBalcony() override { room.setBalcony(true); }
    HotelRoom getRoom() override { return room; }
};

// Director - Uses builder to construct objects
class HotelRoomDirector {
private:
    HotelRoomBuilder* builder;
public:
    HotelRoomDirector(HotelRoomBuilder* b) : builder(b) {}

    HotelRoom construct() {
        builder->buildBed();
        builder->buildTV();
        builder->buildMiniFridge();
        builder->buildJacuzzi();
        builder->buildBalcony();
        return builder->getRoom();
    }
};

// Client Code
int main() {
    // Constructing a Basic Room
    BasicRoomBuilder basicBuilder;
    HotelRoomDirector director1(&basicBuilder);
    HotelRoom basicRoom = director1.construct();
    basicRoom.showRoom();

    std::cout << "\n";

    // Constructing a Deluxe Room
    DeluxeRoomBuilder deluxeBuilder;
    HotelRoomDirector director2(&deluxeBuilder);
    HotelRoom deluxeRoom = director2.construct();
    deluxeRoom.showRoom();

    std::cout << "\n";

    // Constructing a Suite Room
    SuiteRoomBuilder suiteBuilder;
    HotelRoomDirector director3(&suiteBuilder);
    HotelRoom suiteRoom = director3.construct();
    suiteRoom.showRoom();

    return 0;
}
