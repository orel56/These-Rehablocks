
##################################OLD##########################
typecode_dictionnary = {                                      #
                     "led" : '000011',
                     "buzzer" : '100001',
                     "potentiometer": '000000',
                     "accelerometer" : '000001',
                     "joystick": '000010',
                     "ultrason": '001000',
                     "button":'000111'}
familly_dictionnary = {
                     "led" : '000',
                     "buzzer" : '000',
                     "potentiometer": '001',
                     "accelerometer" : '001',
                     "joystick": '001',
                     "ultrason": '001',
                     "button": '001'}

################################# OLD #########################

config_nodes = {"led": {"type" :'000011', "fam" : '000', "produced_subject" : '60'}, 
                "accelerometer": {"type" :'000001',"fam" : '001',"produced_subject":'65051'},
                 "ultrason" :  {"type" :'001000',"fam" : '001',"produced_subject": '35'},
                 "buzzer" :  {"type" :'100001',"fam" : '000',"produced_subject":'61'},
                 "joystick":  {"type" :'000010',"fam" : '001',"produced_subject": '4'},
                 "button": {"type": '000111','fam':"001","produced_subject":'17'}}

subject_dictionnary = {"mouv":'1',
                       "angle_x":"2",
                       "angle_y":"3",
                       "potar_value":"16",
                       "button_status" : "17",
                       "led_status":"60",
                       "buzz_status" : "61",
                       "obstacle" : "35",                       
                       "direction" : "4"
                       }