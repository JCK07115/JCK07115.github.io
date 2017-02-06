boolean channel_1_prev, channel_2_prev, channel_3_prev, channel_4_prev, channel_5_prev;
float timer_1, timer_2, timer_3, timer_4, timer_5;
float rec_input_channel_1, rec_input_channel_2, rec_input_channel_3, rec_input_channel_4, rec_input_channel_5;

void setup() {
  PCICR |= (1 << PCIE0);      //enable the PCMSK0, so we can detect when a member of the PCINT0_vect changes

  //PCMSK0 enables checking for individual changes in pin inputs, after an interrupt has been triggered
  PCMSK0 |= (1 << PCINT0);    //trigger an interrupt when digital pin 8, of the PCINT0_vect, changes
  PCMSK0 |= (1 << PCINT1);    //trigger an interrupt when digital pin 9, of the PCINT0_vect, changes
  PCMSK0 |= (1 << PCINT2);    //trigger an interrupt when digital pin 10, of the PCINT0_vect, changes
  PCMSK0 |= (1 << PCINT3);    //trigger an interrupt when digital pin 11, of the PCINT0_vect, changes
  PCMSK0 |= (1 << PCINT4);    //trigger an interrupt when digital pin 12, of the PCINT0_vect, changes

  Serial.begin(9600);
}

void loop() {
  // put your main code here, to run repeatedly:

}

/*this interrupt subroutine will be triggered whenever a change
in the PCINT0_vect is detected. the PCINT0_vect checks for changes
in the input of digital pins 8-13, but pins 9, 10, 14, 15,16, 17, 18, 19 on the ATMEGA328*/
ISR(PCINT0_vect){

  /*the PCINT0 (interrupt for digital pin 8) is represented by the bit B0 (zeroth bit)
  in the PINB register*/
  //Channel 1
  if(channel_1_prev == 0 && (PINB & B00000001)){
    channel_1_prev = 1;
    timer_1 = micros();
  } else if(channel_1_prev == 1 && (PINB & B00000001)){
    channel_1_prev = 0;
    rec_input_channel_1 = micros() - timer_1;
  }

  /*the PCINT1 (interrupt for digital pin 9) is represented by the bit B1 (first bit)
  in the PINB register*/
  //Channel 2
  if(channel_2_prev == 0 && (PINB & B00000010)){
    channel_2_prev = 1;
    timer_2 = micros();
  } else if(channel_2_prev == 1 && (PINB & B00000010)){
    channel_2_prev = 0;
    rec_input_channel_2 = micros() - timer_2;
  }

  /*the PCINT2 (interrupt for digital pin 10) is represented by the bit B2 (second bit)
  in the PINB register*/
  //Channel 3
  if(channel_3_prev == 0 && (PINB & B00000100)){
    channel_3_prev = 1;
    timer_3 = micros();
  } else if(channel_3_prev == 1 && (PINB & B00000100)){
    channel_3_prev = 0;
    rec_input_channel_3 = micros() - timer_3;
  }

  /*the PCINT3 (interrupt for digital pin 11) is represented by the bit B3 (third bit)
  in the PINB register*/
  //Channel 4
  if(channel_4_prev == 0 && (PINB & B00001000)){
    channel_4_prev = 1;
    timer_4 = micros();
  } else if(channel_4_prev == 1 && (PINB & B00001000)){
    channel_4_prev = 0;
    rec_input_channel_1 = micros() - timer_4;
  }

  /*the PCINT4 (interrupt for digital pin 12) is represented by the bit B4 (fourth bit)
  in the PINB register*/
  //Channel 5 - mode switch
  if(channel_5_prev == 0 && (PINB & B00010000)){
    channel_5_prev = 1;
    timer_5 = micros();
  } else if(channel_5_prev == 1 && (PINB & B00010000)){
    channel_5_prev = 0;
    rec_input_channel_5 = micros() - timer_5;
  }
}

