#include "rsa.h"
#include <set>
#include <cmath>
#include <cstdlib>
#include <numeric>
#include <iostream>

std::set<int>prime = {}; // a set will be the collection of prime numbers,
// where we can select random primes p and q
int public_key;
int private_key;
int n;
// we will run the function only once to fill the set of
// prime numbers
void primefiller()
{
    // method used to fill the primes set is seive of
    // eratosthenes(a method to collect prime numbers)

    std::vector<bool> seive(250, true);
    seive[0] = false;
    seive[1] = false;
    for (int i = 2; i < 250; i++) {
        for (int j = i * 2; j < 250; j += i) {
            seive[j] = false;
        }
    } // filling the prime numbers
    for (int i = 0; i < seive.size(); i++) {
        if (seive[i])
            prime.insert(i);
    }
}
// picking a random prime number and erasing that prime
// number from list because p!=q
int pickrandomprime()
{
    srand(time(NULL));
    int k = rand() % prime.size();
    auto it = prime.begin();
    while (k--)
        it++;
    int ret = *it;
    prime.erase(it);
    return ret;
}
void rsa_encryption::setkeys(int& private_key, int& public_key, int & n)
{
    primefiller();
    int prime1 = pickrandomprime(); // first prime number
    int prime2 = pickrandomprime(); // second prime number
    // to check the prime numbers selected
    // cout<<prime1<<" "<<prime2<<endl;
    n = prime1 * prime2;
    int fi = (prime1 - 1) * (prime2 - 1);
    int e = 2;
    while (1) {
        if (std::gcd(e, fi) == 1)
            break;
        e++;
    } // d = (k*Φ(n) + 1) / e for some integer k
    public_key = e;
    int d = 2;
    while (1) {
        if ((d * e) % fi == 1)
            break;
        d++;
    }
    private_key = d;
}
// to encrypt the given number
long long int encrypt(double message, const int& public_key, const int& n)
{
    int e = public_key;
    long long int encrpyted_text = 1;
    while (e--) {
        encrpyted_text *= message;
        encrpyted_text %= n;
    }
    return encrpyted_text;
}
// to decrypt the given number
long long int decrypt(int encrpyted_text, const int& private_key, const int& n)
{
    int d = private_key;
    long long int decrypted = 1;
    while (d--) {
        decrypted *= encrpyted_text;
        decrypted %= n;
    }
    return decrypted;
}
// first converting each character to its ASCII value and
// then encoding it then decoding the number to get the
// ASCII and converting it to character
std::vector<int> rsa_encryption::encoder(std::string message, const int& public_key, const int& n)
{
    std::vector<int> form;
    // calling the encrypting function in encoding function
    for (auto& letter : message)
        form.push_back(encrypt((int)letter, public_key, n));
    return form;
}
std::string rsa_encryption::decoder(std::vector<int> encoded, const int& private_key, const int& n)
{
    std::string s;
    // calling the decrypting function decoding function
    for (auto& num : encoded)
        s += decrypt(num, private_key, n);
    return s;
}
//int main()
//{
//    setkeys(private_key, public_key, n);
//    std::string message = "Test Message";
//    // uncomment below for manual input
//    // cout<<"enter the message\n";getline(cin,message);
//    // calling the encoding function
//    std::vector<int> coded = encoder(message, public_key, n);
//    std::cout << "Initial message:\n" << message;
//    std::cout << "\n\nThe encoded message(encrypted by public "
//        "key)\n";
//    for (auto& p : coded)
//        std::cout << p;
//    std::cout << "\n\nThe decoded message(decrypted by private "
//        "key)\n";
//    std::cout << decoder(coded, private_key, n) << std::endl;
//    return 0;
//}
