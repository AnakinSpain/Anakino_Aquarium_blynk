////////////////////////////////////////////////////////////////
// Funcion variable blynk
////////////////////////////////////////////////////////////////
void blynk(){
  Blynk.virtualWrite(V5, temp_agua); // envia la temp al widget agua
  Blynk.virtualWrite(V6, tempHB); // envia la temp al widget habitacion
  Blynk.virtualWrite(V7, tempD); // envia la temp al widget 
}




