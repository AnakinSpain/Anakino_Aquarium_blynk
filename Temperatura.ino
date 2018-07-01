
/////////////////////////////////////////////////////////////////
// Controlamos las temperaturas para 
// activar o desactivar calentador y ventiladores
/////////////////////////////////////////////////////////////////
void check_temp(){

  contador_temp ++;                  // LEE LOS VALORES DE LAS TEMPERATURAS

  sensors.requestTemperatures();   // call sensors.requestTemperatures() to issue a global 
  // temperature request to all devices on the bus
  temperatura_agua_temp += (sensors.getTempC(sensor_agua));  // Serial.print(temperatura_agua_temp);
  temperatura_habitacion_temp += (sensors. getTempC(sensor_habitacion));       // lee temperatura habitacion.
  temperatura_disipador_temp += (sensors.getTempC(sensor_disipador));   //read heatsink temperature

  if(contador_temp == 10)
  {
    temp_agua = temperatura_agua_temp / 10;
    tempHB = temperatura_habitacion_temp / 10;
    tempD = temperatura_disipador_temp / 10; 
    
    contador_temp = 0;  
    temperatura_agua_temp = 0;    
    temperatura_habitacion_temp = 0;
    temperatura_disipador_temp = 0; 
  }
                                         // ACTUA EN FUNCION DE LOS VALORES DE LA TEMPERATURA
  
    if (temp_agua != -127 && temp_agua != 85){
     
      if (temp_agua <= temp_agua_des-temperatura_margen){
        SetRele(calentador,HIGH); // Encendemos Calentador
        led1.on();
        estado1 = 0;       
  //      Serial.print("Enciende calentador: "); Serial.print(temp_agua); Serial.print(" "); Serial.println(temp_agua_des-temperatura_margen); 
        alarmas = 1;  
       }
       
      if (temp_agua >= temp_agua_des+temperatura_margen){
        SetRele(calentador,LOW); // Apagamos Calentador
        led1.off();
        estado1 = 1;
   //     Serial.print("Apaga calentador");Serial.print(temp_agua); Serial.print(" "); Serial.println(temp_agua_des+temperatura_margen);
        alarmas = 0; 
      }
  /*    Serial.println(temp_agua);
      Serial.print(" ");
      Serial.println(temp_agua_des+0.5);
      Serial.print(" ");
      Serial.println(EEPROM.read(1));
   */   
    }
    else
    {
      SetRele(calentador,LOW); // Apagamos Calentador
    }
    
} 

/////////////////////////////////////////////////////////////////
//Fin controlTemperatura
/////////////////////////////////////////////////////////////////

