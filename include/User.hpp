#pragma once

#include <string>

class User {
private:
    std::string userId;
    std::string username;
    std::string password;

    bool active;

public:
    User(
        const std::string& userId,
        const std::string& username,
        const std::string& password
    );

    virtual ~User() = default;

    const std::string& getUserId() const;
    const std::string& getUsername() const;

    bool isActive() const;

    bool authenticate(
        const std::string& password
    ) const;

    void changePassword(
        const std::string& currentPassword,
        const std::string& newPassword
    );

    void activate();
    void deactivate();

    virtual std::string getRole() const = 0;

    virtual void print() const;
};