#include "Employee.hpp"

#include <iomanip>
#include <iostream>
#include <stdexcept>

Employee::Employee(
    const std::string& userId,
    const std::string& username,
    const std::string& password,
    const std::string& employeeId,
    const std::string& firstName,
    const std::string& lastName,
    const std::string& department,
    const std::string& hireDate,
    double salary
)
    : User(
          userId,
          username,
          password
      ),
      employeeId(employeeId),
      firstName(firstName),
      lastName(lastName),
      department(department),
      hireDate(hireDate),
      salary(salary),
      onDuty(false)
{
    if (employeeId.empty()) {
        throw std::invalid_argument(
            "Employee ID cannot be empty"
        );
    }

    if (firstName.empty()) {
        throw std::invalid_argument(
            "Employee first name cannot be empty"
        );
    }

    if (lastName.empty()) {
        throw std::invalid_argument(
            "Employee last name cannot be empty"
        );
    }

    if (department.empty()) {
        throw std::invalid_argument(
            "Department cannot be empty"
        );
    }

    if (hireDate.empty()) {
        throw std::invalid_argument(
            "Hire date cannot be empty"
        );
    }

    if (salary < 0.0) {
        throw std::invalid_argument(
            "Salary cannot be negative"
        );
    }
}

const std::string&
Employee::getEmployeeId() const
{
    return employeeId;
}

const std::string&
Employee::getFirstName() const
{
    return firstName;
}

const std::string&
Employee::getLastName() const
{
    return lastName;
}

const std::string&
Employee::getDepartment() const
{
    return department;
}

const std::string&
Employee::getHireDate() const
{
    return hireDate;
}

double Employee::getSalary() const
{
    return salary;
}

bool Employee::isOnDuty() const
{
    return onDuty;
}

std::string Employee::getFullName() const
{
    return firstName + " " + lastName;
}

void Employee::setDepartment(
    const std::string& department
)
{
    if (department.empty()) {
        throw std::invalid_argument(
            "Department cannot be empty"
        );
    }

    this->department = department;
}

void Employee::setSalary(
    double salary
)
{
    if (salary < 0.0) {
        throw std::invalid_argument(
            "Salary cannot be negative"
        );
    }

    this->salary = salary;
}

void Employee::startDuty()
{
    if (!isActive()) {
        throw std::runtime_error(
            "Inactive employee cannot start duty"
        );
    }

    if (onDuty) {
        throw std::runtime_error(
            "Employee is already on duty"
        );
    }

    onDuty = true;
}

void Employee::endDuty()
{
    if (!onDuty) {
        throw std::runtime_error(
            "Employee is not currently on duty"
        );
    }

    onDuty = false;
}

std::string Employee::getRole() const
{
    return "Employee";
}

void Employee::print() const
{
    std::cout
        << "\n========================================\n"
        << "          EMPLOYEE INFORMATION\n"
        << "========================================\n";

    std::cout
        << "Employee ID: "
        << employeeId
        << '\n';

    std::cout
        << "Name:        "
        << getFullName()
        << '\n';

    std::cout
        << "Username:    "
        << getUsername()
        << '\n';

    std::cout
        << "Department:  "
        << department
        << '\n';

    std::cout
        << "Role:        "
        << getRole()
        << '\n';

    std::cout
        << "Hire Date:   "
        << hireDate
        << '\n';

    std::cout
        << std::fixed
        << std::setprecision(2);

    std::cout
        << "Salary:      $"
        << salary
        << '\n';

    std::cout
        << "Account:     "
        << (isActive()
                ? "Active"
                : "Inactive")
        << '\n';

    std::cout
        << "Duty Status: "
        << (onDuty
                ? "On Duty"
                : "Off Duty")
        << '\n';

    std::cout
        << "========================================\n";
}