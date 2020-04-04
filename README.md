# Bibliothèque de communication de Puissance 4

La bilbiothèque est prévue pour les 2 cartes :
- core
- displayer ou ihm

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

## Principe général

La bibliothèque s'articule autour d'un lien série. Pour cela, il faut 
renseigner le context, qui permettra à la libraire d'utiliser le lien série. Elle
a besoin d'une fonction pour envoyer sur le bus série et d'un buffer pour stocker
les données reçues

```C
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

