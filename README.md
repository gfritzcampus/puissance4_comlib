# Bibliothèque de communication de Puissance 4

La bilbiothèque est prévue pour les 2 cartes :
- core
- displayer ou ihm

## Principe général

La bibliothèque s'articule autour d'un lien série. Pour cela, il faut 
renseigner le context, qui permettra à la libraire d'utiliser le lien série. Elle
a besoin d'une fonction pour envoyer sur le bus série et d'un buffer pour stocker
les données reçues

```C
// Fonction d'envoie sur le lien série - votre charge
size_t sendOnUart(const void * const buffer, size_t size) {
  // Envoie sur le bus série du "size" octets contenu dans 
  // le buffer "buffer".
  // Renvoie le nombre d'octets qui a pu être effectivement
  // envoyé
  return XXX;
}

#define SIZE_OF_BUFFER 250
int main(int argc, char * argv[]) {
  ...
  char buffer[250];
  P4SerialContext context = {
    .send = sendOnUart,
    .buffer = INIT_BUFFER(buffer, SIZE_OF_BUFFER)
  };

  // Pour envoyer un message (ici l'appuie par le joueur 1
  // du bouton gauche)
  p4PlayerPress(&context, P4P_PLAYER_1, P4A_LEFT, P4AS_DOWN);


  // Réception
  char data = getByteFromUart();

  P4DisplayerCommand cmd = p4AccumulateFromDisplayer(&context, data);
  if (cmd.resultCode == P4RC_OK) {
    if (cmd.type == P4DCT_PlayerCommand) {
      // une action d'un joueur vient d'être reçu
    }
    else if (cmd.type == P4DCT_LightSensor) {
      // une valeur mesurée par le capteur de luminosité à été reçu
    }
    else if (cmd.type == P4DCT_None) {
      // le dernier caractère reçu ne complète aucune commande
    }
  }
```

## Core

Le core doit inclure les fichiers :
- inc/board_control.h
- inc/common.h
- inc/displayer_receiver.h
- inc/decode.h
- inc/receiver.h
- src/board_control.c
- src/displayer_receiver.c
- src/receiver.c

### Changer la couleur d'un anneau

Un anneau est identifié par son numéro de ligne (de 0 à 6) et par son numéro de colonne 
(de 0 à 6). La couleur est au format RGB - chacune des composantes peut prendre les valeurs
de 0 à 255. La couleur noir `rgb(0,0,0)` éteint l'anneau. 

```C
P4MatrixPoint ring = { 
  .row = 0,
  .column = 1 };
P4Color color = {
  .red = 0,
  .green = 255,
  .blue = 125 };

P4ReturnCode ret = p4SetRingColor(&context, ring, color);

/////// Ou

P4ReturnCode ret = p4SetRingColor(&context, { 0, 1 }, { 125, 125, 125 });
```

### Changer l'intensité d'une zone

Une zone est l'ensemble des anneaux contenus dans le carré identifié par l'anneau
en haut à gauche et par l'anneau en bas à droite. Ainsi, pour changer l'intégralité
du plateau, la zone est définie par l'anneau en haut à gauche (0,0) et par l'anneau
en bas à droite (6,6). L'intensité est en pourcentage, de 0 à 100.

```C
P4MatrixZone zone = {
  .startPoint = { 
    .row = 0,
    .column = 0 },
  .endPoint = {
    .row = 6,
    .column = 6 }};

P4ReturnCode ret = p4SetZoneIntensity(&context, zone, 50);

///////////// Ou

P4ReturnCode ret = p4SetZoneIntensity(&context, { { 0,0 }, { 6,6 } }, 50);
```


### Récpetion des trames

La récéption depuis l'uart se fait à votre charge. Chaque octet reçu est ensuite 
à injecter dans l'accumulateur, qui en retour vous indiquera

## Displayer/IHM

Le displayer doit inclure les fichiers :
- inc/board_receiver.h
- inc/common.h
- inc/decode.h
- inc/displayer_control.h
- inc/receiver.h
- src/board_receiver.c
- src/displayer_control.c
- src/receiver.c

