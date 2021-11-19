class PCB:

    def __init__(self, version, propietario, fecha):
        self.__ver = version
        self.__owner = propietario
        self.__date = fecha

    def __str__(self) -> str:
        return "Placa Controladora: {}".format(self.set_ver)

    def set_ver(self, version_pcb):
        self.__ver = version_pcb
    
    def set_owner(self, autor):
        self.__owner = autor

    def set_date(self, tiempo):
        self__date = tiempo

    def get_all(self):
        return "PCB --> {} {} {}".format(self.__ver, self.__owner, self.__date)

    
