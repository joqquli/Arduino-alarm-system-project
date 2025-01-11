Hälytysjärjestelmäprojekti

📋 Projektin kuvaus

Tämä projekti on yksinkertainen hälytysjärjestelmä, joka on rakennettu käyttäen Arduinoa ja seuraavia komponentteja:

Näppäimistö käyttäjän syötteille

Ultrasonic-anturi etäisyyden mittaamiseen

LCD-näyttö viestien näyttämistä varten

Punainen LED-merkkivalo hälytyksen ilmaisemiseksi

Pahvinen kotelo projektin suojaamiseksi ja pitämiseksi siistinä

Järjestelmä tunnistaa, jos kohde (esimerkiksi henkilö) lähestyy hälytysanturaa ja käynnistää hälytyksen. Näppäimistöä käytetään järjestelmän aktivoimiseen ja deaktivointiin.

🧰 Tarvittavat komponentit

Arduino Uno tai vastaava mikrokontrolleri

4x4 näppäimistö

HC-SR04 ultrasonic-anturi

LCD-näyttö (16x2)

Punainen LED

Vastuksia ja hyppylankoja

Pahvinen kotelo projektin kokoamiseen

📂 Projektin tiedostot

Projektissa käytetyt tiedostot:

active.ino - Hälytysjärjestelmän pääkoodi, joka sisältää logiikan hälytyksen aktivoimiseksi ja deaktivoinniksi.

alert.ino - Hälytyksen käynnistyksen ja hälytyksestä poistumisen logiikka.

settings.ino - Käyttäjän syöttämien asetusten käsittely ja näytön hallinta.

⚙️ Asennusohjeet

Lataa kaikki projektin tiedostot ja avaa ne Arduino IDE:ssä.

Kytke seuraavat komponentit Arduinoon:

Näppäimistö digitaalisiin portteihin

Ultrasonic-anturi digitaalisiin portteihin

LCD-näyttö I2C-liitännällä

LED valmiustilaa ja hälytystä varten

Lataa koodi Arduinoon.

🔑 Käyttöohjeet

Järjestelmän käynnistyessä LCD-näyttö näyttää tervetuloviestin.

Käyttäjä voi syöttää PIN-koodin näppäimistöllä järjestelmän aktivoimiseksi.

Ultrasonic-anturi seuraa etäisyyttä. Kun anturi havaitsee liikkeen määritellyn etäisyyden sisällä, hälytys käynnistyy.

Hälytyksen voi deaktivoida syöttämällä oikean PIN-koodin.
