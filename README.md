# Estación de monitoreo medioambiental y de calidad del aire de bajo costo para el sector de quintero y puchuncavi
_Estación de monitoreo medioambiental y de calidad del aire de bajo costo, utilizada como tesis de pregrado para optar al título de Ingeniero en Computación e Informática de la Universidad Andrés Bello_

## Pre-requisitos
_Instrumentos y aplicaciones mínimas para la implementación:_
- Arduino UNO.
- Raspberry Pi con SO raspbian y conexión a internet.
- A lo menos un sensor de los utilizados en este proyecto.
- Node-RED instalado en la Raspberry.
- Cliente SSH.

## Instalación
![Sketch del hardware](Documentacion/imagenes/SKETCH.png)

1.- Descargar el release y descomprimir.  
  
2.- Cargar script para Arduino.  
  
![Script de Arduino](Documentacion/imagenes/SubirScript.png)
  
3.- Conectar el Arduino con la Raspberry Pi mediante el puerto USB
  
![Imagen de cable](Documentacion/imagenes/Usb.png)

4.- En la Raspberry Pi, se recomienda habilitar SSH para el acceso desde otro dispositivo.  
Esto se activa en `Menu > Preferencias > Configuración de Raspberry Pi > Interfaces`, y habilitar SSH.  

5.- Instalar Node-RED, para eso se abre una terminal de la Raspberry Pi y copiar el comando que está a continuación.
```
bash <(curl -sL https://raw.githubusercontent.com/node-red/raspbian-deb-package/master/resources/update-nodejs-and-nodered)
```

![Imagen de bash](Documentacion/imagenes/raspiInstallBash.png)  

6.- Al finalizar la instalación, ingresando desde un navegador web en su red local la IP de su dispositivo Raspberry más el puerto 1880 se ingresa al programa Node-RED, como se ejemplifica en la imagen siguiente.  

![Imagen de Node](Documentacion/imagenes/NodeDireccion.png)

7.- Dentro de Node-RED, en la esquina superior derecha, acceder a `Menu > Menage palette > Install`.  
![](Documentacion/imagenes/NodePalette.png)  
![](Documentacion/imagenes/nodePallete2.png)  
  
Buscar e instalar:
- node-red-dashboard.
- node-red-contrib-fs-ops.
- node-red-node-xmpp.
- node-red-contrib-simpletime.

## Importar flujo en Node-RED
Para importar un flujo a Node-RED se debe ir a `Menu > Import > Clipboard > select a file to import`, y seleccionar el archivo `estacion.json` descargado en el release.

### Se continua el proceso con la segunda parte del proyecto. La instalación del cliente XMPP [acá](https://github.com/ecaceres01/Estacion/releases)
