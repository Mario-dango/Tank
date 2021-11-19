class sensor_temp:

    id_sens = 0
    
    def __init__(self, id_temp) -> None:
        self.__temp = 0
        sensor_temp.id_sens += 1
        self.__time = 0
        return

    def set_temp(self, temp, tiempo):
        self.__temp = temp
        self.__time = tiempo

    def get_temp(self, id):
        return "Temperatura del sensor {} {}".format(id, self.__temp)

