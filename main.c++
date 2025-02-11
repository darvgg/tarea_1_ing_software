#include <iostream>
#include <vector>
#include <cmath>
#include <bitset>
#include <algorithm>
#include <sstream> // Incluir para usar istringstream

using namespace std;

struct SubnetRequest {
    int quantity;
    int hosts;
};

struct SubnetResult {
    string subnetAddress;
    int prefix;
};

// Convierte una IP en formato string a un entero sin signo
unsigned int ipToUInt(string ip) {
    unsigned int result = 0;
    int part;
    char dot;
    istringstream ss(ip);

    for (int i = 0; i < 4; i++) {
        ss >> part;
        result = (result << 8) + part;
        if (i < 3) {
            ss >> dot; // Leer el punto entre los octetos
        }
    }

    return result;
}

// Convierte un entero sin signo de vuelta a una dirección IP
string uintToIp(unsigned int ip) {
    return to_string((ip >> 24) & 0xFF) + "." +
        to_string((ip >> 16) & 0xFF) + "." +
        to_string((ip >> 8) & 0xFF) + "." +
        to_string(ip & 0xFF);
}

// Calcula el prefijo necesario para soportar el número de hosts solicitado
int calculatePrefix(int hosts) {
    return 32 - static_cast<int>(ceil(log2(hosts + 2)));
}

// Convierte el prefijo de la máscara en formato CIDR a su forma decimal
string prefixToMask(int prefix) {
    unsigned int mask = (0xFFFFFFFF << (32 - prefix)) & 0xFFFFFFFF;
    return uintToIp(mask);
}

// Imprime las subredes con sus respectivas direcciones y máscaras
void printSubnets(const vector<SubnetRequest>& requests, string network, int networkMask) {
    unsigned int baseIP = ipToUInt(network);
    vector<SubnetResult> results;

    // Ordenar las solicitudes de más a menos hosts
    vector<SubnetRequest> sortedRequests = requests;
    sort(sortedRequests.begin(), sortedRequests.end(), [](const SubnetRequest& a, const SubnetRequest& b) {
        return a.hosts > b.hosts;
        });

    for (const SubnetRequest& req : sortedRequests) {
        int prefix = calculatePrefix(req.hosts);
        int numHosts = pow(2, 32 - prefix);
        string mask = prefixToMask(prefix); // Obtener la máscara en formato decimal

        cout << "Para " << req.quantity << " - " << req.hosts << " h:\n";
        for (int i = 0; i < req.quantity; i++) {
            cout << uintToIp(baseIP) << "\t/" << prefix << " (" << mask << ")" << endl;
            baseIP += numHosts;
        }
        cout << endl;
    }
}

int main() {
    // Ejemplo de entrada
    string network = "192.150.0.0";
    int networkMask = 20;

    // Solicitudes de subredes redes
    vector<SubnetRequest> requests = {
        {16, 2},
        {3, 200},
        {4, 100},
        {5, 110},
        {5, 60},
        {4, 20},
        {2, 10}
    };

    printSubnets(requests, network, networkMask);

    return 0;
}