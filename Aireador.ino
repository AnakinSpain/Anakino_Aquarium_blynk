
/////////////////////////////////////////////////////////////////
// control funcionamiento del aireador
/////////////////////////////////////////////////////////////////
void check_ai(){

      
      if (modo_ai == 1)     
      {       
        if (temp_ai == 1 )
        {      
        Serial.println(" AI Auto ON");
        SetRele(aireador,HIGH); //activa rele 
        }
      else if (temp_ai == 0)
      {        
        Serial.println("AI Auto OFF");
        SetRele(aireador,LOW); //desactiva rele
      }
    }
      
   else if (modo_ai == 2)
    {
    Serial.println("AI Manual ON");  
    SetRele(aireador,HIGH); // Encendemos  ai, 
    }
    else if   
      (modo_ai == 3){
      Serial.println("AI Manual OFF");  
      SetRele(aireador,LOW); // Apagamos  ai, 
      
    }    
   }
   
 
/////////////////////////////////////////////////////////////////
//Fin control ai
/////////////////////////////////////////////////////////////////

