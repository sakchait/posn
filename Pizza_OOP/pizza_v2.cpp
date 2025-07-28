#include <iostream>
#include <vector>
#include <memory>
#include <string>
#include <iomanip>

// --- Base Pizza Interface ---
class Pizza {
public:
    virtual double CalculateCost() const = 0;
    virtual std::string GetDescription() const = 0;
    virtual ~Pizza() {}
};

// --- Concrete Pizzas ---
class PepperoniPizza : public Pizza {
public:
    double CalculateCost() const override {
        return 8.5;
    }
    std::string GetDescription() const override {
        return "Pepperoni Pizza";
    }
};

class CheesePizza : public Pizza {
public:
    double CalculateCost() const override {
        return 7.0;
    }
    std::string GetDescription() const override {
        return "Cheese Pizza";
    }
};

struct PizzaComparer {
    bool operator()(const std::shared_ptr<Pizza>& lhs, const std::shared_ptr<Pizza>& rhs) const {
        return lhs->GetDescription() < rhs->GetDescription();  // sort alphabetically
    }
};

// --- Decorator Base Class ---
class PizzaDecorator : public Pizza {
protected:
    std::shared_ptr<Pizza> pizza;
public:
    PizzaDecorator(std::shared_ptr<Pizza> pizza) : pizza(pizza) {}
};

// --- Concrete Toppings ---
class ExtraCheese : public PizzaDecorator {
public:
    ExtraCheese(std::shared_ptr<Pizza> pizza) : PizzaDecorator(pizza) {}
    double CalculateCost() const override {
        return pizza->CalculateCost() + 1.5;
    }
    std::string GetDescription() const override {
        return pizza->GetDescription() + " + Extra Cheese";
    }
};

class MushroomTopping : public PizzaDecorator {
public:
    MushroomTopping(std::shared_ptr<Pizza> pizza) : PizzaDecorator(pizza) {}
    double CalculateCost() const override {
        return pizza->CalculateCost() + 2.0;
    }
    std::string GetDescription() const override {
        return pizza->GetDescription() + " + Mushrooms";
    }
};

class Order {
private:
    std::multiset<std::shared_ptr<Pizza>, PizzaComparer> pizzas;

public:
    void AddPizza(std::shared_ptr<Pizza> pizza) {
        pizzas.insert(pizza);  // inserts in sorted order
    }

    double CalculateTotalCost() const {
        double total = 0;
        for (const auto& p : pizzas)
            total += p->CalculateCost();
        return total;
    }

    void PrintOrderSummary() const {
        for (const auto& p : pizzas)
            std::cout << p->GetDescription() << " - $" << std::fixed << std::setprecision(2) << p->CalculateCost() << "\n";
        std::cout << "Total: $" << std::fixed << std::setprecision(2) << CalculateTotalCost() << "\n";
    }
};

// --- Factory Method ---
class PizzaFactory {
public:
    static std::shared_ptr<Pizza> CreatePizza(const std::string& type) {
        if (type == "Pepperoni")
            return std::make_shared<PepperoniPizza>();
        else if (type == "Cheese")
            return std::make_shared<CheesePizza>();
        else
            return nullptr;
    }
};

// --- Main Program ---
int main() {
    // Create pizzas
    auto pepperoniPizza = PizzaFactory::CreatePizza("Pepperoni");
    auto cheesePizza = PizzaFactory::CreatePizza("Cheese");

    // Add toppings
    auto toppedCheese = std::make_shared<ExtraCheese>(cheesePizza);
    auto toppedPepperoni = std::make_shared<MushroomTopping>(pepperoniPizza);

    // Add to order
    Order order;
    order.AddPizza(toppedCheese);
    order.AddPizza(toppedPepperoni);

    // Print order
    order.PrintOrderSummary();

    return 0;
}
