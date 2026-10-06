#pragma once

#include "User.hpp"

#include <string>

class Employee : public User {
private:
    std::string employeeId;
    std::string firstName;
    std::string lastName;
    std::string department;
    std::string hireDate;

    double salary;
    bool onDuty;

public:
    Employee(
        const std::string& userId,
        const std::string& username,
        const std::string& password,
        const std::string& employeeId,
        const std::string& firstName,
        const std::string& lastName,
        const std::string& department,
        const std::string& hireDate,
        double salary
    );

    virtual ~Employee() = default;

    const std::string& getEmployeeId() const;
    const std::string& getFirstName() const;
    const std::string& getLastName() const;
    const std::string& getDepartment() const;
    const std::string& getHireDate() const;

    double getSalary() const;

    bool isOnDuty() const;

    std::string getFullName() const;

    void setDepartment(
        const std::string& department
    );

    void setSalary(
        double salary
    );

    void startDuty();
    void endDuty();

    std::string getRole() const override;

    void print() const override;
};