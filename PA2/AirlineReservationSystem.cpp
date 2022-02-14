#include "AirlineReservationSystem.h"

void AirlineReservationSystem::addPassenger(const std::string &firstname, const std::string &lastname)
{
    Passenger newPassenger = Passenger(firstname, lastname);
    this->passengers.insert(newPassenger);
}

Passenger *AirlineReservationSystem::searchPassenger(const std::string &firstname, const std::string &lastname)
{
    Passenger newPassenger = Passenger(firstname, lastname);
    return this->passengers.search(newPassenger)->data;
}

void AirlineReservationSystem::addFlight(const std::string &flightCode, const std::string &departureTime, const std::string &arrivalTime, const std::string &departureCity, const std::string &arrivalCity, int economyCapacity, int businessCapacity)
{
    Flight newFlight = Flight(flightCode, departureTime, arrivalTime, departureCity, arrivalCity, economyCapacity, businessCapacity);
    this->flights.insert(newFlight);
}

std::vector<Flight *> AirlineReservationSystem::searchFlight(const std::string &departureCity, const std::string &arrivalCity)
{
    std::vector<Flight *> flights_from_dep;
    Queue<BSTNode<Flight>> traverse_q;
    BSTNode<Flight> node;
    traverse_q.enqueue(flights.getRoot());
    while (!traverse_q.isEmpty())
    {
        node = traverse_q.dequeue();
        if (node.getDepartureCity() == departureCity && node.getArrivalCity() == arrivalCity)
        {
            flights_from_dep.push_back(&node.data);
        }
        if (node->left)
        {
            traverse_q.enqueue(node->left);
        }
        else if (node->right)
        {
            traverse_q.enqueue(node->right);
        }
    }
    return flights_from_dep;
}
Flight *searchFlight_with_code(BSTNode<Flight> node, const std::string flightCode)
{
    if (node == NULL)
    {
        return NULL;
    }
    else if (flightCode < node.data.getFlightCode())
    {
        return searchFlight_with_code(node->left, flightCode);
    }
    else if (flightCode > node.data.getFlightCode())
    {
        return searchFlight_with_code(node->right, flightCode);
    }
    else if (flightCode == node.data.getFlightCode())
    {
        return node;
    }
}

void AirlineReservationSystem::issueTicket(const std::string &firstname, const std::string &lastname, const std::string &flightCode, TicketType ticketType)
{
    Passenger *passer = searchPassenger(firstname, lastname);
    Flight *plane = searchFlight_with_code(flights.getRoot(), flightCode);
    if (plane && passer)
    {
        Ticket newTicket = Ticket(passer, plane, ticketType);
        plane->addTicket(newTicket);
    }
}

void AirlineReservationSystem::saveFreeTicketRequest(const std::string &firstname, const std::string &lastname, const std::string &flightCode, TicketType ticketType)
{
    Passenger *passer = searchPassenger(firstname, lastname);
    Flight *plane = searchFlight_with_code(flights.getRoot(), flightCode);
    if (plane && passer)
    {
        Ticket newTicket = Ticket(passer, plane, ticketType);
        freeTicketRequests.enqueue(newTicket);
    }
}

void AirlineReservationSystem::executeTheFlight(const std::string &flightCode)
{
    Flight *plane = searchFlight_with_code(flights.getRoot(), flightCode);
    if (plane)
    {
        int businessCap = plane->getBusinessCapacity();
        int economyCap = plane->getEconomyCapacity();
        while ((businessCap || economyCap) && !freeTicketRequests.isEmpty())
        {
            if (freeTicketRequests.peek().getTicketType() == economy && economyCap)
            {
                economyCap--;
                freeTicketRequests.dequeue();
            }
            else if (freeTicketRequests.peek().getTicketType() == business && businessCap)
            {
                businessCap--;
                freeTicketRequests.dequeue();
            }
            else
            {
                freeTicketRequests.enqueue(freeTicketRequests.dequeue());
            }
        }
        plane->setCompleted();
    }
}

void AirlineReservationSystem::print() const
{
    std::cout << "# Printing the airline reservation system ..." << std::endl;

    std::cout << "# Passengers:" << std::endl;
    passengers.print(inorder);

    std::cout << "# Flights:" << std::endl;
    flights.print(inorder);

    std::cout << "# Free ticket requests:" << std::endl;
    freeTicketRequests.print();

    std::cout << "# Printing is done." << std::endl;
}
