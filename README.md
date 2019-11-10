# Estación de monitoreo medioambiental y de calidad del aire de bajo costo para el sector de quintero y puchuncavi
_Estación de monitoreo medioambiental y de calidad del aire de bajo costo, utilizada como tesis de pregrado para optar al título de Ingeniero en Computación e Informática de la Universidad Andrés Bello_

## Pre-requisitos
_Instrumentos y aplicaciones mínimas para la implementación:_
* Arduino UNO.
* Raspberry Pi con su SO raspbian.
* A lo menos un sensor de los utilizados en este proyecto.
* Node-RED instalado en la Raspberry.

## Instalación
Script de instalación de node-RED para SO Debian (Raspbian):
```
bash <(curl -sL https://raw.githubusercontent.com/node-red/raspbian-deb-package/master/resources/update-nodejs-and-nodered)
```
Ejecuta el siguiente comando en tu carpeta de Node-RED, normalmente `~/.node-red`
```
npm install node-red-node-xmpp
```
## Importar flujo en Node-RED
Para importar un flujo a Node-RED se debe ir a `Menu > Import > Clipboard`
y copiar el flujo

