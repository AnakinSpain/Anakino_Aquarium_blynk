void check_ventilador(){
  if (temp_agua>=temp_agua_des+4)
  { 
    SetRele(ventilador,HIGH);
  }
  else
  {
    SetRele(ventilador, LOW);
    
  }
}

