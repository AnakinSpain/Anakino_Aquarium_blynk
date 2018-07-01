
/////////////////////////////////////////////////////////////////
// control funcionamiento lampara UV
/////////////////////////////////////////////////////////////////
void check_UV(){

       
      if (modo_uv == 1){       // si esta configurado el modo automatico
        
       if (temp_uv == 1 )
       {
        SetRele(lamp_uv,HIGH); //activa rele
        Serial.println("UV Auto ON");
      }
      if (temp_uv == 0 )
      {
        SetRele(lamp_uv,LOW); //desactiva rele
        Serial.println("UV Auto OFF");
      }
    }

      

   else if (modo_uv == 2)
    {
      
    SetRele(lamp_uv,HIGH); // Encendemos luz UV, 
    Serial.println("UV Manual ON ");
    }
    else if   
      (modo_uv == 3){  
      SetRele(lamp_uv,LOW); // Apagamos luz UV, es de noche
      Serial.println("UV Manual OFF ");
    }
 
   }
 
/////////////////////////////////////////////////////////////////
//Fin controlUV
/////////////////////////////////////////////////////////////////
