#include <SPI.h>
#include <Ethernet.h>
#include <EthernetUdp.h>

// Adresse IP statique
IPAddress ip(172, 16, 1, 3);

// Port UDP pour la communication avec le switch
unsigned int localPort = 8888;

// Création d'un objet Ethernet pour gérer la connexion réseau
EthernetUDP udp;

void setup() {
  // Débute la communication série pour le débogage
  Serial.begin(9600);

  // Adresse MAC de la carte Ethernet Arduino
  byte mac[] = { 0xA8, 0x61, 0x0A, 0xAE, 0x94, 0x9B};

  // Initialise l'Ethernet avec l'adresse MAC et l'adresse IP statique
  Ethernet.begin(mac, ip);

  // Débute la communication UDP
  udp.begin(localPort);

  // Attend 1 seconde pour permettre la connexion Ethernet de s'établir
  delay(1000);
}


void loop() {
  // Message à envoyer au switch
  char message[] = "Hello, switch!";

  // Envoie du message au switch
  udp.beginPacket(IPAddress(172, 16, 1, 1), 8888); // Remplacez l'adresse IP par celle de votre switch
  udp.write(message, strlen(message)); // Correction : ajout de la longueur du message
  udp.endPacket();

  // Attente de la réponse du switch
  delay(1000); // Attendez suffisamment longtemps pour que le switch réponde

  // Vérification de la réponse du switch
  int packetSize = udp.parsePacket();
  if (packetSize) {
    // Lecture des données reçues dans le tampon packetBuffer
    char packetBuffer[UDP_TX_PACKET_MAX_SIZE];
    udp.read(packetBuffer, UDP_TX_PACKET_MAX_SIZE);
    // Affichage de la réponse du switch sur le moniteur série
    Serial.print("Response from switch: ");
    Serial.println(packetBuffer);
  } else {
    // Si aucune réponse n'est reçue, affichez un message d'erreur
    Serial.println("No response from switch.");
  }

  // Attente avant d'envoyer le prochain message
  delay(5000); // Attendre 5 secondes avant d'envoyer le prochain message
}
