#include <bits/stdc++.h>
#include <iostream>

using namespace std;

// ENUMS

enum class AccountStatus {
    ACTIVE,
    DEACTIVATED
};

enum class ReactionType {
    HAPPY,
    DISLIKE,
    LIKE
};

// BASIC CLASSES

class Address {
private:
    string name;
    string streetname;
    string city;
    string state;
    string country;
    string pincode;
};

class Account {
private:
    string email;
    string password;
    time_t doj;
    AccountStatus status;

public:
    void resetPassword() {
        // logic to reset password
    }
};

class Education {
private:
    string name;
    string course_name;
    int doj;
    int dog;
    Address* address;
};

class Achievement {
private:
    string name;
    string description;
    int yoa;
    string awarded_by;
};

class Skill {
private:
    string name;
    int yoe;
    string description;
};

class Experience {
private:
    string name;
    time_t start_time;
    time_t end_time;
    string designation;
    string company_name;
    string company_id;
};

class Comment {
private:
    int commentId;
    int postId;
    int user_id;
    string comment;

public:
    Comment(int pid, int uid, string text) : postId(pid), user_id(uid), comment(text) {}
};

class Reaction {
private:
    ReactionType type;
    int user_id;
    int post_id;

public:
    Reaction(ReactionType t, int uid, int pid) : type(t), user_id(uid), post_id(pid) {}
};

class JobPosting {
private:
    int post_id;
    time_t dop;
    string title;
    string description;
};

class Recommendation {
private:
    int recommendationID;
    string title;
    string description;
    time_t dor;
    int given_by_user_id;
    int recipient_user_id;
};

class Notification {
private:
    string message;
    time_t timestamp;

public:
    Notification(string msg) : message(msg) {
        timestamp = time(nullptr);
    }
};

class INotificationStrategy {
public:
    virtual void send(int userId, const string& message) = 0;
};

class InAppNotificationStrategy : public INotificationStrategy {
public:
    void send(int userId, const string& message) override;
};

class NotificationService {
private:
    static INotificationStrategy* strategy;

public:
    static void setStrategy(INotificationStrategy* newStrategy) {
        strategy = newStrategy;
    }

    static void sendNotification(int userId, const string& message) {
        if (strategy) strategy->send(userId, message);
    }
};

INotificationStrategy* NotificationService::strategy = nullptr;

class Message {
private:
    int senderId;
    int receiverId;
    string text;
    time_t timestamp;

public:
    Message(int s, int r, string t) : senderId(s), receiverId(r), text(t) {
        timestamp = time(nullptr);
    }
};

class ConnectionRequest {
private:
    int fromUserId;
    int toUserId;
    bool accepted;

public:
    ConnectionRequest(int fromId, int toId) : fromUserId(fromId), toUserId(toId), accepted(false) {}

    void accept() { accepted = true; }
    void decline() { accepted = false; }
    bool isAccepted() const { return accepted; }
};

// FORWARD DECLARATIONS
class Company;
class User;
class Group;

// PROFILE

class Profile {
private:
    vector<Experience*> experiences;
    vector<Skill*> skills;
    vector<Achievement*> achievements;
    vector<Education*> educations;
    vector<Recommendation*> recommendations;
    vector<Company*> followingCompanies;
    vector<User*> followingUsers;

public:
    void addEducation(Education* edu) { educations.push_back(edu); }
    void addSkill(Skill* skill) { skills.push_back(skill); }
    void addRecommendation(Recommendation* rec) { recommendations.push_back(rec); }
};

// POST

class Post {
private:
    int post_id;
    int user_id;
    string title;
    string body;
    vector<string> imageUrls;
    vector<Comment*> comments;
    vector<Reaction*> reactions;

public:
    void addComment(Comment* comment) {
        comments.push_back(comment);
        NotificationService::sendNotification(user_id, "New comment on your post.");
    }

    void addReaction(Reaction* reaction) {
        reactions.push_back(reaction);
        NotificationService::sendNotification(user_id, "New reaction on your post.");
    }
};

// COMPANY

class Company {
private:
    int company_id;
    string name;
    string description;
    time_t doc;
    int total_strength;
    vector<JobPosting*> postings;

public:
    void addJobPosting(JobPosting* posting) {
        postings.push_back(posting);
    }
};

// USER

class User {
private:
    Profile* profile;
    Account* account;
    string first_name;
    string last_name;
    int userId;
    vector<Message*> inbox;
    vector<Notification*> notifications;

public:
    User(int uid) : userId(uid) {
        profile = new Profile();
        account = new Account();
    }

    int getUserId() { return userId; }
    Profile* getProfile() { return profile; }

    void sendMessage(User* receiver, string text) {
        Message* msg = new Message(userId, receiver->getUserId(), text);
        receiver->inbox.push_back(msg);
        NotificationService::sendNotification(receiver->getUserId(), "New message received.");
    }

    void receiveNotification(Notification* notif) {
        notifications.push_back(notif);
    }
};

void InAppNotificationStrategy::send(int userId, const string& message) {
    static unordered_map<int, User*> userDirectory; // For demo
    if (userDirectory.find(userId) == userDirectory.end()) return;
    userDirectory[userId]->receiveNotification(new Notification(message));
}

// GROUP

class Group {
private:
    int groupId;
    string name;
    unordered_set<int> users;

public:
    Group(int id, string groupName) : groupId(id), name(groupName) {}
    int getGroupId() { return groupId; }
    void addUser(int uid) { users.insert(uid); }
};

// REPOSITORIES

class IUserRepository {
public:
    virtual void addUser(User* user) = 0;
    virtual void removeUser(User* user) = 0;
};

class UserRepository : public IUserRepository {
private:
    unordered_map<int, User*> users;

public:
    void addUser(User* user) override {
        users[user->getUserId()] = user;
    }

    void removeUser(User* user) override {
        users.erase(user->getUserId());
    }

    User* getUser(int uid) {
        return users.count(uid) ? users[uid] : nullptr;
    }
};

class IGroupRepository {
public:
    virtual void addGroup(Group* grp) = 0;
    virtual void removeGroup(Group* grp) = 0;
};

class GroupRepository : public IGroupRepository {
private:
    unordered_map<int, Group*> groups;

public:
    void addGroup(Group* grp) override {
        groups[grp->getGroupId()] = grp;
    }

    void removeGroup(Group* grp) override {
        groups.erase(grp->getGroupId());
    }
};

// SERVICES

class CommentService {
public:
    static Comment* createComment(int postId, int userId, string comment) {
        return new Comment(postId, userId, comment);
    }
};

class ReactionService {
public:
    static Reaction* createReaction(ReactionType type, int userId, int postId) {
        return new Reaction(type, userId, postId);
    }
};

class GroupService {
public:
    static Group* createGroup(GroupRepository* repo, int groupId, string group_name, User* admin) {
        Group* g = new Group(groupId, group_name);
        g->addUser(admin->getUserId());
        repo->addGroup(g);
        return g;
    }
};

class FollowService {
public:
    static void followUser(User* follower, User* followee) {
        follower->getProfile()->addRecommendation(new Recommendation());
    }
};

int main() {
    UserRepository* userRepo = new UserRepository();
    GroupRepository* groupRepo = new GroupRepository();

    User* alice = new User(1);
    User* bob = new User(2);
    userRepo->addUser(alice);
    userRepo.addUser(bob);

    NotificationService::setStrategy(new InAppNotificationStrategy());

    alice->sendMessage(bob, "Hi Bob!");

    Group* g = GroupService::createGroup(groupRepo, 101, "Software Engineers", alice);

    Post* post = new Post();
    post->addComment(CommentService::createComment(10, 1, "Great post!"));
    post->addReaction(ReactionService::createReaction(ReactionType::LIKE, 1, 10));

    return 0;
}
