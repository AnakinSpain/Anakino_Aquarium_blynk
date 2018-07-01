////////////////////////////////////////////////////////////////
// VOID LOOP
///////////////////////////////////////////////////////////////
void loop() {

    blynk();              // envia valores de las variables de arduino a blynk
    check_temp();         // Comprueba valores temperatura
    check_UV();          // Comprobamos horario para encender UV
    check_ai();           // Comprobamos horario para encender AIREADOR
    check_nivel();       // Comprobamos el nivel del agua
    check_ventilador(); // Comprueba si activa ventilador
    Blynk.run();        // Aplicacion Blynk

}
////////////////////////////////////////////////////////////////
// FIN DEL LOOP
///////////////////////////////////////////////////////////////

