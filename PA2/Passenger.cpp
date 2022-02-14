#include "Passenger.h"

Passenger::Passenger(std::string firstname, std::string lastname)
{
    this->firstname = firstname;
    this->lastname = lastname;
}

const std::string &Passenger::getFirstname() const
{
    return firstname;
}

const std::string &Passenger::getLastname() const
{
    return lastname;
}

bool Passenger::operator<(const Passenger &rhs) const
{
    std::string surname = lastname;
    std::string rhs_surname = rhs.lastname;
    int i = 0;
    while (surname[i] && rhs_surname[i])
    {
        char c1 = surname[i];
        char c2 = rhs_surname[i];
        if (c1 < 91 && c1 > 64)
        {
            c1 = c1 + 32;
        }
        if (c2 < 91 && c2 > 64)
        {
            c2 = c2 + 32;
        }

        if (rhs_surname[i] < surname[i])
        {
            return false;
        }
        else if (surname[i] < rhs_surname[i])
        {
            return true;
        }

        i++;
    }
    if (rhs_surname.size() > surname.size())
    {
        return true;
    }
    else if (surname.size() == rhs_surname.size())
    {
        std::string name = firstname;
        std::string rhs_name = rhs.firstname;
        int i = 0;
        while (name[i] && rhs_name[i])
        {
            char c1 = name[i];
            char c2 = rhs_name[i];
            if (c1 < 91 && c1 > 64)
            {
                c1 = c1 + 32;
            }
            if (c2 < 91 && c2 > 64)
            {
                c2 = c2 + 32;
            }
            if (rhs_name[i] > name[i])
            {
                return false;
            }
            else if (rhs_name[i] < name[i])
            {
                return true;
            }

            i++;
        }
        if (rhs_name.size() > name.size())
        {
            return true;
        }
    }

    return false;
}

bool Passenger::operator>(const Passenger &rhs) const
{
    return rhs < *this;
}

bool Passenger::operator==(const Passenger &rhs) const
{
    return firstname == rhs.firstname && lastname == rhs.lastname;
}

bool Passenger::operator!=(const Passenger &rhs) const
{
    return !(rhs == *this);
}

std::ostream &operator<<(std::ostream &os, const Passenger &passenger)
{
    os << "Passenger(firstname: " << passenger.firstname;
    os << ", lastname: " << passenger.lastname << ")";
    return os;
}
