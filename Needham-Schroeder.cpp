/*
 * Ryan English (rhe0002)
 * COMP 5370
 * Homework 4
 * Needham-Schroeder Protocol Simulator
 */

#include <iostream>
using namespace std;

class Actor {
private:
    string name = "";
    int key = 0;
    int session_key = 0;
public:
    Actor(string myName);
    void setKey(int newKey) {key = newKey;}
    string getName() {return name;}
    int getKey() {return key;}
    int getSessionKey() {return session_key;}
    void setSessionKey(int newKey) {session_key = newKey;}
};

class CentralAuthority {
public:
    void generateKey(int in_key1, int in_key2, int& skey1, int& skey2);
    string encrypt_decrypt(string str, int key);
};

int randInt(int a, int b);

int main() {
    int session1, session2;
    string u1_response, u2_response, encrypted = "", decrypted = "";

    srand (time(0));

    CentralAuthority* ca = new CentralAuthority();
    Actor* u1 = new Actor("User1");
    Actor* u2 = new Actor("User2");

    cout << "----------STARTING NEEDHAM-SCHROEDER PROTOCOL SIMULATOR----------" << endl << endl;

    //generate random key values
    u1->setKey(randInt(1, 20));
    u2->setKey(randInt(1, 20));

    //make sure keys aren't the same
    while (u1->getKey() == u2->getKey()) {
        u2->setKey(randInt(1, 20));
    }

    //cout << "User1's key: " << u1->getKey() << endl;
    //cout << "User2's key: " << u2->getKey() << endl;

    //User1 sends key to CA, where CA generates a session key, encrypted with User1's key
    cout << u1->getName() << " and " << u2->getName() << " are receiving key from Central Authority" << endl;
    ca->generateKey(u1->getKey(), u2->getKey(), session1, session2);
    u1->setSessionKey(session1);
    cout << u1->getName() << "'s encrypted session key: " << u1->getSessionKey() << endl;
    u1->setSessionKey(u1->getSessionKey() ^ u1->getKey());
    cout << u1->getName() << "'s decrypted session key: " << u1->getSessionKey() << endl;

    //Same for User2
    u2->setSessionKey(session2);
    cout << u2->getName() << "'s encrypted session key: " << u2->getSessionKey() << endl;
    u2->setSessionKey(u2->getSessionKey() ^ u2->getKey());
    cout << u2->getName() << "'s decrypted session key: " << u2->getSessionKey() << endl << endl;

    cout << "Attempting to open session..." << endl;
    if (u1->getSessionKey() != u2->getSessionKey()) {
        cerr << "Keys do not match. Exiting now." << endl;
        return 1;
    }
    else cout << "Session successfully opened!" << endl << endl;

    //User1 sends initial message to User2
    u1_response = "This is User1, can you hear me?";
    cout << u1->getName() << "'s original message to " << u2->getName() << ": ";
    cout << u1_response << endl;
    cout << u1->getName() << " is sending encrypted message to " << u2->getName() << ": ";
    encrypted = ca->encrypt_decrypt(u1_response, u1->getSessionKey()); //Encrypt original message
    cout << encrypted << endl;

    //User2 receives message and decrypts
    cout << u2->getName() << " is receiving message from " << u1->getName() << ": " << encrypted << endl;
    cout << u2->getName() << " is decrypting message with session key..." << endl;
    decrypted = ca->encrypt_decrypt(encrypted, u1->getSessionKey()); //Decrypt encrypted message
    cout << "Decrypted message: " << decrypted << endl << endl;

    //User2 responds to User1
    u2_response = "Yes, loud and clear!";
    cout << u2->getName() << "'s original message to " << u1->getName() << ": ";
    cout << u2_response << endl;
    cout << u2->getName() << " is sending encrypted message to " << u1->getName() << ": ";
    encrypted = ca->encrypt_decrypt(u2_response, u2->getSessionKey()); //Encrypt original message
    cout << encrypted << endl;

    //User1 receives message and decrypts
    cout << u1->getName() << " is receiving message from " << u2->getName() << ": " << encrypted << endl;
    cout << u1->getName() << " is decrypting message with session key..." << endl;
    decrypted = ca->encrypt_decrypt(encrypted, u2->getSessionKey()); //Decrypt encrypted message
    cout << "Decrypted message: " << decrypted << endl << endl;

    cout << "----------CLOSING SESSION AND ENDING SIMULATION----------" << endl;


    return 0;
}

Actor::Actor(string myName) {
    name = myName;
}

void CentralAuthority::generateKey(int in_key1, int in_key2, int &skey1, int& skey2) {
    int sessionKey = randInt(1, 100);
    skey1 = sessionKey ^ in_key1;
    skey2 = sessionKey ^ in_key2;
}

string CentralAuthority::encrypt_decrypt(string str, int key) {\
    string newStr = "";
    for (int i = 0; i < str.length(); i++) {
        newStr += char((int)str[i] ^ key);
    }
    return newStr;
}

int randInt(int a, int b) {
    return rand()%b+a;
}