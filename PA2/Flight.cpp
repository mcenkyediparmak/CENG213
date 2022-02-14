#include "Flight.h"

Flight::Flight(const std::string &flightCode, const std::string &departureTime, const std::string &arrivalTime, const std::string &departureCity, const std::string &arrivalCity, int economyCapacity, int businessCapacity)
{
    this->flightCode = flightCode;
    this->departureTime = departureTime;
    this->arrivalTime = arrivalTime;
    this->departureCity = departureCity;
    this->arrivalCity = arrivalCity;
    this->economyCapacity = economyCapacity;
    this->businessCapacity = businessCapacity;
    this->completed = false;
}

const std::string &Flight::getFlightCode() const
{
    return flightCode;
}

const std::string &Flight::getDepartureTime() const
{
    return departureTime;
}

const std::string &Flight::getArrivalTime() const
{
    return arrivalTime;
}

const std::string &Flight::getDepartureCity() const
{
    return departureCity;
}

const std::string &Flight::getArrivalCity() const
{
    return arrivalCity;
}

int Flight::getEconomyCapacity() const
{
    return economyCapacity;
}

int Flight::getBusinessCapacity() const
{
    return businessCapacity;
}

bool Flight::isCompleted() const
{
    return completed;
}

void Flight::setCompleted(bool completed)
{
    this->completed = completed;
}

bool Flight::addTicket(const Ticket &ticket)
{
    int cap;
    if (ticket.getTicketType() == business)
    {
        cap = businessCapacity;
    }
    else
    {
        cap = economyCapacity;
    }
    for (int i = 0; i < this->tickets.size(); i++)
    {
        if (this->tickets[i].getTicketType() == ticket.getTicketType())
        {
            cap--;
        }
    }
    if (cap > 0)
    {
        this->tickets.push_back(ticket);
        return true;
    }
    else
    {
        return false;
    }
}

bool Flight::operator<(const Flight &rhs) const
{
    std::string code = flightCode;
    std::string rhs_code = rhs.getFlightCode();
    int i = 0;
    while (code[i] && rhs_code[i])
    {
        char c1 = code[i];
        char c2 = rhs_code[i];
        if (c1 < 91 && c1 > 64)
        {
            c1 = c1 + 32;
        }
        if (c2 < 91 && c2 > 64)
        {
            c2 = c2 + 32;
        }
        if (rhs_code[i] > code[i])
        {
            return false;
        }
        else if (rhs_code[i] < code[i])
        {
            return true;
        }

        i++;
    }
    if (rhs_code.size() > code.size())
    {
        return true;
    }
    return false;
}

bool Flight::operator>(const Flight &rhs) const
{
    return rhs < *this;
}

bool Flight::operator==(const Flight &rhs) const
{
    return flightCode == rhs.flightCode;
}

bool Flight::operator!=(const Flight &rhs) const
{
    return !(rhs == *this);
}

std::ostream &operator<<(std::ostream &os, const Flight &flight)
{
    os << "Flight(flightCode: " << flight.flightCode;
    os << ", departureTime: " << flight.departureTime;
    os << ", arrivalTime: " << flight.arrivalTime;
    os << ", departureCity: " << flight.departureCity;
    os << ", arrivalCity: " << flight.arrivalCity;
    os << ", economyCapacity: " << flight.economyCapacity;
    os << ", businessCapacity: " << flight.businessCapacity;
    os << ", completed: " << (flight.completed ? "yes" : "no");
    os << ", tickets: [";
    for (int i = 0; i < flight.tickets.size(); ++i)
    {
        os << flight.tickets[i];
        if (i != (flight.tickets.size() - 1))
            os << ", ";
    }
    os << "])";
    return os;
}
