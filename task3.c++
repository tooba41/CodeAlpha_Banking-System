#include <iostream>
#include <string>
#include <vector>
#include <algorithm>

using namespace std;

class BankAccount {
private:
    int accountNumber;
    float balance;
    string firstName;
    string lastName;

public:
    BankAccount(int accNum, float initialBalance, string fName, string lName)
        : accountNumber(accNum), balance(initialBalance), firstName(fName), lastName(lName) {}

    int getAccountNumber() const { return accountNumber; }
    float getBalance() const { return balance; }
    string getFirstName() const { return firstName; }
    string getLastName() const { return lastName; }

    void deposit(float amount) {
        balance += amount;
    }

    bool withdraw(float amount) {
        if (amount <= balance) {
            balance -= amount;
            return true;
        } else {
            return false;
        }
    }

    void printAccountInfo() const {
        cout << "Account Number: " << accountNumber << endl;
        cout << "Name: " << firstName << " " << lastName << endl;
        cout << "Balance: $" << balance << endl;
    }
};

class Transaction {
private:
    int transactionId;
    int accountNumber;
    float amount;
    string type; // "deposit", "withdrawal", "transfer-out", "transfer-in"

public:
    Transaction(int transId, int accNum, float amt, string transType)
        : transactionId(transId), accountNumber(accNum), amount(amt), type(transType) {}

    int getTransactionId() const { return transactionId; }
    int getAccountNumber() const { return accountNumber; }
    float getAmount() const { return amount; }
    string getType() const { return type; }

    void printTransactionInfo() const {
        cout << "Transaction ID: " << transactionId << endl;
        cout << "Account Number: " << accountNumber << endl;
        cout << "Amount: $" << amount << endl;
        cout << "Type: " << type << endl;
    }
};

class Customer {
private:
    int customerId;
    string firstName;
    string lastName;
    vector<BankAccount> accounts;

public:
    Customer(int custId, string fName, string lName)
        : customerId(custId), firstName(fName), lastName(lName) {}

    int getCustomerId() const { return customerId; }
    string getFirstName() const { return firstName; }
    string getLastName() const { return lastName; }

    void addAccount(const BankAccount& account) {
        accounts.push_back(account);
    }

    void printCustomerInfo() const {
        cout << "Customer ID: " << customerId << endl;
        cout << "Name: " << firstName << " " << lastName << endl;
        cout << "Accounts:" << endl;
        for (const auto& account : accounts) {
            account.printAccountInfo();
            cout << endl;
        }
    }

    BankAccount* findAccount(int accountNum) {
        for (auto& account : accounts) {
            if (account.getAccountNumber() == accountNum) {
                return &account;
            }
        }
        return nullptr;
    }
};

class Bank {
private:
    vector<Customer> customers;
    vector<Transaction> transactions;
    int nextTransactionId = 1;

public:
    void createCustomer() {
        int custId;
        string fName, lName;

        cout << "Enter customer ID: ";
        cin >> custId;
        cout << "Enter your First Name: ";
        cin >> fName;
        cout << "Enter your Last Name: ";
        cin >> lName;

        customers.emplace_back(custId, fName, lName);
        cout << "Customer created successfully." << endl;
    }

    Customer* findCustomer(int customerId) {
        for (auto& customer : customers) {
            if (customer.getCustomerId() == customerId) {
                return &customer;
            }
        }
        return nullptr;
    }

    void createAccount() {
        int custId;
        cout << "Enter customer ID: ";
        cin >> custId;

        Customer* customer = findCustomer(custId);
        if (customer) {
            int accNum;
            float initialBalance;
            cout << "Enter account number: ";
            cin >> accNum;
            cout << "Enter initial balance: ";
            cin >> initialBalance;

            BankAccount account(accNum, initialBalance, customer->getFirstName(), customer->getLastName());
            customer->addAccount(account);
            cout << "Account created successfully." << endl;
        } else {
            cout << "Customer not found." << endl;
        }
    }

    BankAccount* findAccount(int accountNum) {
        for (auto& customer : customers) {
            BankAccount* account = customer.findAccount(accountNum);
            if (account) {
                return account;
            }
        }
        return nullptr;
    }

    void deposit() {
        int accNum;
        float amount;
        cout << "Enter account number: ";
        cin >> accNum;

        BankAccount* account = findAccount(accNum);
        if (account) {
            cout << "Enter deposit amount: ";
            cin >> amount;
            account->deposit(amount);

            transactions.emplace_back(nextTransactionId++, accNum, amount, "deposit");

            cout << "Deposit successful. New balance: $" << account->getBalance() << endl;
        } else {
            cout << "Account not found." << endl;
        }
    }

    void withdraw() {
        int accNum;
        float amount;
        cout << "Enter account number: ";
        cin >> accNum;

        BankAccount* account = findAccount(accNum);
        if (account) {
            cout << "Enter withdrawal amount: ";
            cin >> amount;
            if (account->withdraw(amount)) {
                transactions.emplace_back(nextTransactionId++, accNum, amount, "withdrawal");

                cout << "Withdrawal successful. New balance: $" << account->getBalance() << endl;
            } else {
                cout << "Insufficient funds." << endl;
            }
        } else {
            cout << "Account not found." << endl;
        }
    }

    void checkBalance() {
        int accNum;
        cout << "Enter account number: ";
        cin >> accNum;

        BankAccount* account = findAccount(accNum);
        if (account) {
            cout << "Account balance: $" << account->getBalance() << endl;
        } else {
            cout << "Account not found." << endl;
        }
    }

    void viewCustomerInfo() {
        int custId;
        cout << "Enter customer ID: ";
        cin >> custId;

        Customer* customer = findCustomer(custId);
        if (customer) {
            customer->printCustomerInfo();
        } else {
            cout << "Customer not found." << endl;
        }
    }

    void viewTransactions() {
        for (const auto& transaction : transactions) {
            transaction.printTransactionInfo();
            cout << endl;
        }
    }

    void transfer() {
        int fromAccNum, toAccNum;
        float amount;
        cout << "Enter the source account number: ";
        cin >> fromAccNum;
        cout << "Enter the destination account number: ";
        cin >> toAccNum;
        cout << "Enter the amount to transfer: ";
        cin >> amount;

        BankAccount* fromAccount = findAccount(fromAccNum);
        BankAccount* toAccount = findAccount(toAccNum);

        if (fromAccount && toAccount) {
            if (fromAccount->withdraw(amount)) {
                toAccount->deposit(amount);

                // Record transactions for both withdrawal and deposit
                transactions.emplace_back(nextTransactionId++, fromAccNum, amount, "transfer-out");
                transactions.emplace_back(nextTransactionId++, toAccNum, amount, "transfer-in");

                cout << "Transfer successful. New balance in source account: $" << fromAccount->getBalance() << endl;
                cout << "New balance in destination account: $" << toAccount->getBalance() << endl;
            } else {
                cout << "Insufficient funds in source account." << endl;
            }
        } else {
            cout << "One or both accounts not found." << endl;
        }
    }
};

int main() {
    Bank bank;
    int choice;

    do {
        cout << "\nBanking Menu:\n";
        cout << "1. Create Customer\n";
        cout << "2. Create Account\n";
        cout << "3. Deposit\n";
        cout << "4. Withdraw\n";
        cout << "5. Check Balance\n";
        cout << "6. View Customer Info\n";
        cout << "7. View Transactions\n";
        cout << "8. Transfer\n";
        cout << "9. Exit\n";
        cout << "Enter your choice: ";
        cin >> choice;

        switch (choice) {
            case 1:
                bank.createCustomer();
                break;
            case 2:
                bank.createAccount();
                break;
            case 3:
                bank.deposit();
                break;
            case 4:
                bank.withdraw();
                break;
            case 5:
                bank.checkBalance();
                break;
            case 6:
                bank.viewCustomerInfo();
                break;
            case 7:
                bank.viewTransactions();
                break;
            case 8:
                bank.transfer();
                break;
            case 9:
                cout << "Exiting..." << endl;
                break;
            default:
                cout << "Invalid choice. Please enter a number between 1 and 9." << endl;
        }
    } while (choice != 9);

    return 0;
}
