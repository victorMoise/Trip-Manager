#ifndef RSA_HPP
#define RSA_HPP


#include <iostream>
#include <cmath>
#include <sstream>

class RSA {
private:
    long long p, q; // Prime numbers
    long long n; // Modulus
    long long e, d; // Public and private exponents

    long long gcd(long long a, long long b) {
        if (b == 0)
            return a;
        return gcd(b, a % b);
    }

    long long modPow(long long base, long long exponent, long long modulus) {
        if (exponent == 0)
            return 1;
        long long result = modPow(base, exponent / 2, modulus);
        result = (result * result) % modulus;
        if (exponent % 2 == 1)
            result = (result * base) % modulus;
        return result;
    }

public:
    RSA(long long prime1, long long prime2) {
        p = prime1;
        q = prime2;

        n = p * q;
        
        long long phi = (p - 1) * (q - 1);

        for (e = 2; e < phi; e++) {
            if (gcd(e, phi) == 1)
                break;
        }

        for (d = 2; d < phi; d++) {
            if ((d * e) % phi == 1)
                break;
        }
    }

    std::string encrypt(const std::string& plaintext) {
        std::stringstream encryptedText;

        for (int i = 0; i < plaintext.size(); i++) {
            char c = plaintext[i];
            long long encryptedChar = modPow(c, e, n);
            encryptedText << encryptedChar << ' '; // Add space delimiter
        }

        std::string encryptedString = encryptedText.str();

        // Remove the trailing space if it exists
        if (!encryptedString.empty() && encryptedString.back() == ' ') {
            encryptedString.pop_back();
        }   

        return encryptedString;
    }

    std::string decrypt(const std::string& ciphertext) {
        std::stringstream ss(ciphertext);
        std::string decryptedText;

        long long encryptedChar;
        while (ss >> encryptedChar) {
            long long decryptedChar = modPow(encryptedChar, d, n);
            decryptedText += static_cast<char>(decryptedChar);
        }

        return decryptedText;
    }
};


#endif

