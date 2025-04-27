#include<bits/stdc++.h>
#include <fstream>
using namespace std;

// Product Class
class Product {
public:
    int id;
    string name;
    double price;
    int quantity;

    Product() {}

    Product(int pid, string pname, double pprice, int pquantity) {
        id = pid;
        name = pname;
        price = pprice;
        quantity = pquantity;
    }

    void display() {
        cout << id << "\t" << name << "\t₹" << price << "\tQty: " << quantity << endl;
    }
};

// Supermarket Class
class Supermarket {
    vector<Product> inventory;

public:
    void loadProducts() {
        ifstream file("products.txt");
        if (file.is_open()) {
            Product temp;
            while (file >> temp.id >> temp.name >> temp.price >> temp.quantity) {
                inventory.push_back(temp);
            }
            file.close();
        }
    }

    void saveProducts() {
        ofstream file("products.txt");
        for (auto& p : inventory) {
            file << p.id << " " << p.name << " " << p.price << " " << p.quantity << endl;
        }
        file.close();
    }

    void addProduct() {
        int id, quantity;
        string name;
        double price;
        cout << "Enter Product ID: ";
        cin >> id;
        cout << "Enter Product Name: ";
        cin >> name;
        cout << "Enter Product Price: ";
        cin >> price;
        cout << "Enter Product Quantity: ";
        cin >> quantity;

        inventory.push_back(Product(id, name, price, quantity));
        cout << "Product Added Successfully!\n";
        saveProducts();
    }

    void updateProduct() {
        int id;
        cout << "Enter Product ID to Update: ";
        cin >> id;
        for (auto& p : inventory) {
            if (p.id == id) {
                cout << "Enter new name, price and quantity:\n";
                cin >> p.name >> p.price >> p.quantity;
                cout << "Product Updated Successfully!\n";
                saveProducts();
                return;
            }
        }
        cout << "Product Not Found!\n";
    }

    void deleteProduct() {
        int id;
        cout << "Enter Product ID to Delete: ";
        cin >> id;
        for (auto it = inventory.begin(); it != inventory.end(); ++it) {
            if (it->id == id) {
                inventory.erase(it);
                cout << "Product Deleted Successfully!\n";
                saveProducts();
                return;
            }
        }
        cout << "Product Not Found!\n";
    }

    void displayProducts() {
        cout << "\nAvailable Products:\n";
        cout << "ID\tName\tPrice\tQuantity\n";
        for (auto& p : inventory) {
            p.display();
        }
    }

    Product* findProduct(int id) {
        for (auto& p : inventory) {
            if (p.id == id) return &p;
        }
        return nullptr;
    }
};

// Customer Class
class Customer {
    vector<Product> cart;
    Supermarket& market;

public:
    Customer(Supermarket& m) : market(m) {}

    void shopping() {
        int choice;
        do {
            market.displayProducts();
            cout << "\nEnter Product ID to Add to Cart (0 to Checkout): ";
            int id;
            cin >> id;
            if (id == 0) break;
            Product* p = market.findProduct(id);
            if (p != nullptr) {
                int qty;
                cout << "Enter Quantity: ";
                cin >> qty;
                if (p->quantity >= qty) {
                    cart.push_back(Product(p->id, p->name, p->price, qty));
                    p->quantity -= qty;
                    cout << "Added to Cart!\n";
                } else {
                    cout << "Not enough stock!\n";
                }
            } else {
                cout << "Product Not Found!\n";
            }
        } while (true);

        generateBill();
        market.saveProducts();
    }

    void generateBill() {
        double total = 0;
        cout << "\n===== Bill =====\n";
        cout << "ID\tName\tQty\tPrice\n";
        for (auto& p : cart) {
            cout << p.id << "\t" << p.name << "\t" << p.quantity << "\t₹" << p.price * p.quantity << endl;
            total += p.price * p.quantity;
        }
        cout << "----------------------------\n";
        cout << "Total Amount: ₹" << total << endl;
        cout << "Thank you for shopping!\n";
    }
};

// Admin Login
bool adminLogin() {
    string username, password;
    cout << "Enter Admin Username: ";
    cin >> username;
    cout << "Enter Admin Password: ";
    cin >> password;
    return (username == "admin" && password == "1234");  // Hardcoded for now
}

// Main Function
int main() {
    Supermarket market;
    market.loadProducts();
    int userType;

    cout << "==== Welcome to Supermarket Management System ====\n";
    cout << "1. Admin\n2. Customer\nChoose User Type: ";
    cin >> userType;

    if (userType == 1) {
        if (!adminLogin()) {
            cout << "Invalid Credentials!\n";
            return 0;
        }

        int choice;
        do {
            cout << "\n---- Admin Menu ----\n";
            cout << "1. Add Product\n2. Update Product\n3. Delete Product\n4. View Products\n5. Logout\n";
            cout << "Enter Choice: ";
            cin >> choice;

            switch (choice) {
                case 1: market.addProduct(); break;
                case 2: market.updateProduct(); break;
                case 3: market.deleteProduct(); break;
                case 4: market.displayProducts(); break;
                case 5: cout << "Logging Out...\n"; break;
                default: cout << "Invalid Choice!\n";
            }
        } while (choice != 5);
    } 
    else if (userType == 2) {
        Customer c(market);
        c.shopping();
    }
    else {
        cout << "Invalid User Type!\n";
    }

    return 0;
}
