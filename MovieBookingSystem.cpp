#include <iostream>
#include "string"


using namespace std;

enum class SeatStatus {
    AVAILABLE,
    BOOKED,
    RESERVED
};

class City;

class Discount {
protected:
    double discount;
public:
    Discount(double discount) : discount(discount) {

    }
    virtual ~Discount() = default;
    virtual double applyDiscount(double baseAmount);
};

class MemberShipDiscount :public Discount {
public:
    MemberShipDiscount(double discount) : Discount(discount) {

    }

    double applyDiscount(double baseAmount) {
        return max(baseAmount - discount, 0.0);
    }
};

class Seat {
private:
    int seatId;
    double seatPrice;
    SeatStatus status;
public:
    Seat(int id, double price) {
        seatId = id;
        seatPrice = price;
        status = SeatStatus::AVAILABLE;
    }
    int getSeatId() const {
        return seatId;
    }
    double getSeatPrice() const {
        return seatPrice;
    }
    bool isAvailable() const {
        return status == SeatStatus::AVAILABLE;
    }
    void setStatus(SeatStatus status) {
        this->status = status;
    }
    virtual ~Seat() = default;
};

class GoldSeat :public Seat {
public:
    GoldSeat(int id, double price): Seat(id, price) {

    }
};

class SilverSeat :public Seat {
public:
    SilverSeat(int id, double price): Seat(id, price) {

    }
};

class PlatinumSeat :public Seat {
public:
    PlatinumSeat(int id, double price): Seat(id, price) {

    }
};

class ShowTime {
private:
    int showId;
    time_t startTime;
    int duration;
    vector<Seat*> seats;
public:
    ShowTime(int showId, time_t startTime, int duration, vector<Seat*> seats) {
        this->showId = showId;
        this->startTime = startTime;
        this->duration = duration;
        this->seats = seats;
    }

    void showAvailableSeats() {

    }
};

class Hall {
private:
    int hallId;
    vector<ShowTime*> showTimes;
public:
    Hall(int hallId) {
        this->hallId = hallId;
    }

    void addShowTime(ShowTime* showtime) {
        this->showTimes.push_back(showtime);
    }

    vector<ShowTime*> getSHowTimes() {
        return showTimes;
    }
};

enum class Genre {
    HORROR,
    ACTION,
    COMEDY,
    UNKNOWN
};

class Movie {
private:
    string name;
    time_t releaseDate;
    string language;
    Genre genre;
    int duration;
    vector<ShowTime*> showTimes;
public:
    Movie(string name, time_t releaseDate, string language, Genre genre, int duration) {
        this->name = name;
        this->releaseDate = releaseDate;
        this->language = language;
        this->genre = genre;
        this->duration = duration;
    }

    void addShowTime(ShowTime* showtime) {
        showTimes.push_back(showtime);
    }

    vector<ShowTime*> getShowTimes() {
        return showTimes;
    }
};

class Cinema {
private:
    string name;
    string id;
    vector<Hall*> halls;
    City* city;
public:
    Cinema(string name, string id, vector<Hall*> halls, City* city) {
        this->name = name;
        this->id = id;
        this->halls = halls;
        this->city = city;
    }
};

class City {
private:
    string name;
    int id;
    vector<Cinema*> cinemas;
public:
    City(string name, int id) {

    }
    void addCinema(Cinema* cinema) {
        cinemas.push_back(cinema);
    }
    vector<Cinema*> getCinemas() {
        return cinemas;
    }
};

class MovieTicket {
private:
    int ticketId;
    Seat* seat;
    ShowTime* showTime;
    Movie* movie;
public:
    MovieTicket(int ticketId, ShowTime* showTime, Movie* movie) {
        this->ticketId = ticketId;

    }
    int getTicketId() const {
        return ticketId;
    }
    Seat* getSeat() const {
        return seat;
    }
    ShowTime* geShowTime() {
        return showTime;
    }
    Movie* getMovie() {
        return movie;
    }
};

class Booking {
private:
    int bookId;
    Movie* movie;
    vector<MovieTicket*> tickets;
    int number_of_seats;
public:
    Booking(int bookId, vector<MovieTicket*> tickets, Movie* movie) {
        this->bookId = bookId;
        this->movie = movie;
        this->tickets = tickets;
        this->number_of_seats = tickets.size();
    }

    int getBookId() const {
        return bookId;
    }
    vector<MovieTicket*> getTickets() {
        return tickets;
    }
    Movie* getMovie() {
        return movie;
    }
};

class Payment {
protected:
    int paymentId;
    double amount;
    time_t timestamp;
    vector<Discount*> discounts;
public:
    Payment(int paymentId, double amount, vector<Discount*> discounts) {
        this->paymentId = paymentId;
        this->amount = amount;
        this->timestamp = time(nullptr);
        this->discounts = discounts;
    }
    virtual void makePayment() = 0;
    virtual ~Payment() = default;
};

class CreditCardPayment :public Payment {
private:
    string nameonCard;
public:
    CreditCardPayment(int paymentId, double amount, vector<Discount*> discounts) : Payment(paymentId, amount, discounts) {

    }

    void makePayment() override {
        for (auto itr = discounts.begin(); itr != discounts.end(); itr++) {
            amount = (*itr)->applyDiscount(amount);
        }
        cout<<"Made payment via Credit card for amount "<<amount<<endl;
    }
};

class Cash :public Payment {
public:
    Cash(int paymentId, double amount, vector<Discount*> discounts) : Payment(paymentId, amount, discounts) {

    }

    void makePayment() override {
        cout<<"Made payment via Cash for amount "<<amount<<endl;
    }
};

class Person {
private:
    string name;
    string phone;
    string email;
    string password;
public:
    Person(string name, string phone, string email, string password) {
        this->name = name;
        this->phone = phone;
        this->email = email;
        this->password = password;
    }

    void notify(const string & string) {
        cout<<"Notifying "<<string<<endl;
    }
};

class Customer : public Person {
public:
    Customer(string name, string phone, string email, string password) : Person(name, phone, email, password) {

    }
    void createBooking(Booking* booking);
    void updateBooking(Booking* booking);
    void cancelBooking(Booking* booking);
};

class Admin : public Person {
public:
    Admin(string name, string phone, string email, string password): Person(name, phone, email, password) {

    }
    void addShow(ShowTime* show);
    void deleteShow(ShowTime* show);
    void updateShow(ShowTime* show);
    void addMovie(Movie* movie);
    void deleteMovie(Movie* movie);
};

class TicketAgent : public Person {
public:
    TicketAgent(string name, string phone, string email, string password): Person(name, phone, email, password) {

    }

    void createBooking(Booking* booking);
};


class Notification {
protected:
    int id;
    string message;
public:
    Notification(int id, string message) : id(id), message(message) {

    }
    virtual void sendNotification(Person* person);
    ~Notification() = default;
};

class EmailNotification :public Notification {
public:
    EmailNotification(int id, string message) : Notification(id, message) {

    }
    void sendNotification(Person* person) {
        person->notify(message);
    }
};

class Search {
public:
    virtual vector<Movie*> searchMovieTitle(string title);
    virtual vector<Movie*> searchByGenre(string genre);
    virtual vector<Movie*> searchByTitle(string title);
    virtual vector<Movie*> seachByLanguage(string language);
};

class Catalog {
private:
    unordered_map<string, vector<Movie*>> movies;
    unordered_map<string, vector<Movie*>> moviesByGenre;
    unordered_map<string, vector<Movie*>> moviesByTitle;
    unordered_map<string, vector<Movie*>> moviesByLanguage;
public:
    vector<Movie*> searchMovieTitle(string title) {
        return movies[title];
    }
    vector<Movie*> searchByGenre(string genre) {
        return moviesByGenre[genre];
    }
    vector<Movie*> searchByTitle(string title) {
        return moviesByTitle[title];
    }
    vector<Movie*> seachByLanguage(string language) {
        return moviesByLanguage[language];
    }
};

int main() {
    return 0;
}


#include <iostream>
#include <string>
#include <vector>
#include <unordered_map>
#include <ctime>
#include <mutex>
#include <thread>
#include <algorithm>

using namespace std;

enum class SeatStatus {
    AVAILABLE,
    BOOKED,
    RESERVED
};

class City;

class Discount {
protected:
    double discount;
public:
    Discount(double discount) : discount(discount) {}
    virtual ~Discount() = default;
    virtual double applyDiscount(double baseAmount) {
        return baseAmount; // default implementation returns base
    }
};

class MemberShipDiscount : public Discount {
public:
    MemberShipDiscount(double discount) : Discount(discount) {}
    double applyDiscount(double baseAmount) override {
        return max(baseAmount - discount, 0.0);
    }
};

class Seat {
private:
    int seatId;
    double seatPrice;
    SeatStatus status;
    mutable mutex mtx;
public:
    Seat(int id, double price) : seatId(id), seatPrice(price), status(SeatStatus::AVAILABLE) {}

    int getSeatId() const { return seatId; }
    double getSeatPrice() const { return seatPrice; }
    bool isAvailable() const { return status == SeatStatus::AVAILABLE; }

    bool bookSeat() {
        lock_guard<mutex> lock(mtx);
        if (status == SeatStatus::AVAILABLE) {
            status = SeatStatus::BOOKED;
            return true;
        }
        return false;
    }

    void setStatus(SeatStatus status) {
        lock_guard<mutex> lock(mtx);
        this->status = status;
    }

    virtual ~Seat() = default;
};

class GoldSeat : public Seat {
public:
    GoldSeat(int id, double price): Seat(id, price) {}
};

class SilverSeat : public Seat {
public:
    SilverSeat(int id, double price): Seat(id, price) {}
};

class PlatinumSeat : public Seat {
public:
    PlatinumSeat(int id, double price): Seat(id, price) {}
};

class ShowTime {
private:
    int showId;
    time_t startTime;
    int duration;
    vector<Seat*> seats;
public:
    ShowTime(int showId, time_t startTime, int duration, vector<Seat*> seats)
        : showId(showId), startTime(startTime), duration(duration), seats(seats) {}

    void showAvailableSeats() {
        for (auto& seat : seats) {
            if (seat->isAvailable()) {
                cout << "Seat " << seat->getSeatId() << " is available\n";
            }
        }
    }

    Seat* getSeatById(int id) {
        for (auto& seat : seats) {
            if (seat->getSeatId() == id) return seat;
        }
        return nullptr;
    }

    const vector<Seat*>& getSeats() const { return seats; }
};

class Hall {
private:
    int hallId;
    vector<ShowTime*> showTimes;
public:
    Hall(int hallId) : hallId(hallId) {}

    void addShowTime(ShowTime* showtime) {
        this->showTimes.push_back(showtime);
    }

    vector<ShowTime*> getShowTimes() {
        return showTimes;
    }
};

enum class Genre {
    HORROR,
    ACTION,
    COMEDY,
    UNKNOWN
};

class Movie {
private:
    string name;
    time_t releaseDate;
    string language;
    Genre genre;
    int duration;
    vector<ShowTime*> showTimes;
public:
    Movie(string name, time_t releaseDate, string language, Genre genre, int duration)
        : name(name), releaseDate(releaseDate), language(language), genre(genre), duration(duration) {}

    void addShowTime(ShowTime* showtime) {
        showTimes.push_back(showtime);
    }

    vector<ShowTime*> getShowTimes() {
        return showTimes;
    }

    string getName() const { return name; }
};

class Cinema {
private:
    string name;
    string id;
    vector<Hall*> halls;
    City* city;
public:
    Cinema(string name, string id, vector<Hall*> halls, City* city)
        : name(name), id(id), halls(halls), city(city) {}
};

class City {
private:
    string name;
    int id;
    vector<Cinema*> cinemas;
public:
    City(string name, int id) : name(name), id(id) {}

    void addCinema(Cinema* cinema) {
        cinemas.push_back(cinema);
    }

    vector<Cinema*> getCinemas() {
        return cinemas;
    }
};

class MovieTicket {
private:
    int ticketId;
    Seat* seat;
    ShowTime* showTime;
    Movie* movie;
public:
    MovieTicket(int ticketId, Seat* seat, ShowTime* showTime, Movie* movie)
        : ticketId(ticketId), seat(seat), showTime(showTime), movie(movie) {}

    int getTicketId() const { return ticketId; }
    Seat* getSeat() const { return seat; }
    ShowTime* getShowTime() { return showTime; }
    Movie* getMovie() { return movie; }
};

class Booking {
private:
    int bookId;
    Movie* movie;
    vector<MovieTicket*> tickets;
    int number_of_seats;
public:
    Booking(int bookId, vector<MovieTicket*> tickets, Movie* movie)
        : bookId(bookId), movie(movie), tickets(tickets), number_of_seats(tickets.size()) {}

    int getBookId() const { return bookId; }
    vector<MovieTicket*> getTickets() { return tickets; }
    Movie* getMovie() { return movie; }
};

class Payment {
protected:
    int paymentId;
    double amount;
    time_t timestamp;
    vector<Discount*> discounts;
public:
    Payment(int paymentId, double amount, vector<Discount*> discounts)
        : paymentId(paymentId), amount(amount), timestamp(time(nullptr)), discounts(discounts) {}

    virtual void makePayment() = 0;
    virtual ~Payment() = default;
};

class CreditCardPayment : public Payment {
public:
    CreditCardPayment(int paymentId, double amount, vector<Discount*> discounts)
        : Payment(paymentId, amount, discounts) {}

    void makePayment() override {
        for (auto& d : discounts) {
            amount = d->applyDiscount(amount);
        }
        cout << "Made payment via Credit card for amount " << amount << endl;
    }
};

class Cash : public Payment {
public:
    Cash(int paymentId, double amount, vector<Discount*> discounts)
        : Payment(paymentId, amount, discounts) {}

    void makePayment() override {
        cout << "Made payment via Cash for amount " << amount << endl;
    }
};

class Person {
private:
    string name;
    string phone;
    string email;
    string password;
public:
    Person(string name, string phone, string email, string password)
        : name(name), phone(phone), email(email), password(password) {}

    void notify(const string& message) {
        cout << "Notifying: " << message << endl;
    }
};

class Customer : public Person {
public:
    Customer(string name, string phone, string email, string password)
        : Person(name, phone, email, password) {}

    void createBooking(Booking* booking) {}
    void updateBooking(Booking* booking) {}
    void cancelBooking(Booking* booking) {}
};

class Admin : public Person {
public:
    Admin(string name, string phone, string email, string password)
        : Person(name, phone, email, password) {}

    void addShow(ShowTime* show) {}
    void deleteShow(ShowTime* show) {}
    void updateShow(ShowTime* show) {}
    void addMovie(Movie* movie) {}
    void deleteMovie(Movie* movie) {}
};

class TicketAgent : public Person {
public:
    TicketAgent(string name, string phone, string email, string password)
        : Person(name, phone, email, password) {}

    void createBooking(Booking* booking) {}
};

class Notification {
protected:
    int id;
    string message;
public:
    Notification(int id, string message) : id(id), message(message) {}
    virtual void sendNotification(Person* person) {
        person->notify(message);
    }
    virtual ~Notification() = default;
};

class EmailNotification : public Notification {
public:
    EmailNotification(int id, string message) : Notification(id, message) {}
    void sendNotification(Person* person) override {
        person->notify(message);
    }
};

class Search {
public:
    virtual vector<Movie*> searchMovieTitle(string title) { return {}; }
    virtual vector<Movie*> searchByGenre(string genre) { return {}; }
    virtual vector<Movie*> searchByTitle(string title) { return {}; }
    virtual vector<Movie*> seachByLanguage(string language) { return {}; }
};

class Catalog {
private:
    unordered_map<string, vector<Movie*>> movies;
    unordered_map<string, vector<Movie*>> moviesByGenre;
    unordered_map<string, vector<Movie*>> moviesByTitle;
    unordered_map<string, vector<Movie*>> moviesByLanguage;
public:
    vector<Movie*> searchMovieTitle(string title) {
        return movies[title];
    }
    vector<Movie*> searchByGenre(string genre) {
        return moviesByGenre[genre];
    }
    vector<Movie*> searchByTitle(string title) {
        return moviesByTitle[title];
    }
    vector<Movie*> seachByLanguage(string language) {
        return moviesByLanguage[language];
    }
};

int main() {
    // Example usage
    vector<Seat*> seats;
    for (int i = 1; i <= 5; i++) {
        seats.push_back(new GoldSeat(i, 200));
    }

    ShowTime* show = new ShowTime(101, time(nullptr), 120, seats);
    Movie* movie = new Movie("Interstellar", time(nullptr), "English", Genre::ACTION, 169);
    movie->addShowTime(show);

    cout << "Available seats before booking:\n";
    show->showAvailableSeats();

    Seat* targetSeat = show->getSeatById(2);
    if (targetSeat && targetSeat->bookSeat()) {
        MovieTicket* ticket = new MovieTicket(1, targetSeat, show, movie);
        Booking* booking = new Booking(1, { ticket }, movie);
        cout << "Booking ID: " << booking->getBookId() << " | Seat: " << targetSeat->getSeatId() << endl;
    } else {
        cout << "Seat already booked.\n";
    }

    cout << "\nAvailable seats after booking:\n";
    show->showAvailableSeats();
    return 0;
}
