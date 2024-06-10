#include <Ethernet.h>
#include <SPI.h>

// Adresse MAC pour votre shield Ethernet
byte mac[] = { 0xA8, 0x61, 0x0A, 0xAE, 0x94, 0x9B }; // Adresse MAC de votre shield

IPAddress ip(172, 16, 1, 3); // Adresse IP statique de l'Arduino
IPAddress subnet(255, 255, 0, 0); // Masque de sous-réseau
IPAddress gateway(172, 16, 1, 1); // Passerelle par défaut
IPAddress dnsServer(8, 8, 8, 8); // Serveur DNS (Google DNS par exemple)

// Serveurs à tester
char server1[] = "10.100.0.254"; // Passerelle de l'école
char server2[] = "103.224.182.219";  // Site web à tester
const int serverPort = 80; // Port HTTP

// Paramètres pour le test Telnet
char telnetServer[] = "172.16.1.1"; // IP de votre pare-feu
const int telnetPort = 23; // Port Telnet par défaut

EthernetClient client;

void setup() {
  Serial.begin(9600);
  while (!Serial) {
    ; // Attendre que le port série soit disponible (Leonardo)
  }

  // Initialiser le shield Ethernet avec une adresse IP statique
  Ethernet.begin(mac, ip, dnsServer, gateway, subnet);

  // Attendre un peu pour être sûr que le shield soit initialisé
  delay(1000);

  // Afficher l'adresse IP attribuée
  Serial.print("Adresse IP attribuée : ");
  Serial.println(Ethernet.localIP());

  // Tentative de connexion Telnet au pare-feu
  Serial.println("Tentative de connexion Telnet au pare-feu...");
  if (client.connect(telnetServer, telnetPort)) {
    Serial.println("Connecté au pare-feu via Telnet.");
    // Envoyer une commande Telnet si nécessaire
    client.println("Votre commande Telnet ici");
  } else {
    Serial.println("Échec de connexion au pare-feu via Telnet.");
  }

  // Attendre pour s'assurer que la réponse est complète
  delay(2000);
  client.stop(); // Terminer la connexion avec le serveur Telnet

  // Tentative de connexion HTTP pour obtenir l'adresse IP - Serveur 1
  Serial.println("Tentative de connexion à la passerelle...");
  if (client.connect(server1, serverPort)) {
    Serial.println("Connecté à la passerelle.");
    client.print("GET / HTTP/1.1\r\n");
    client.print("Host: ");
    client.println(server1);
    client.println("Connection: close\r\n");
    client.println();
  } else {
    Serial.println("Échec de connexion à la passerelle.");
  }

  // Attendre pour s'assurer que la réponse est complète
  delay(2000);
  client.stop(); // Terminer la connexion avec le serveur 1

  // Tentative de connexion HTTP pour obtenir l'adresse IP - Serveur 2
  Serial.println("Tentative de connexion à exemple.com...");
  if (client.connect(server2, serverPort)) {
    Serial.println("Connecté à exemple.com.");
    client.print("GET / HTTP/1.1\r\n");
    client.print("Host: ");
    client.println(server2);
    client.println("Connection: close\r\n");
    client.println();
  } else {
    Serial.println("Échec de connexion à exemple.com.");
  }
}

void loop() {
  // Si des données sont disponibles du serveur, les afficher
  if (client.available()) {
    char c = client.read();
    Serial.print(c);

    // Si nous recevons une réponse HTML
    if (c == '<') {
      if (client.find("html>")) {
        Serial.println("Connexion au serveur réussie.");
      }
    }
  }

  // Si la connexion est terminée, afficher une notification et arrêter le client
  if (!client.connected() && client.available() == 0) {
    Serial.println();
    Serial.println("Déconnexion.");
    client.stop();

    // Faire une pause avant de recommencer
    while (true);
  }
}
