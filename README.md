# Estacion
_Estación de monitoreo medioambiental y de calidad del aire de bajo costo_

## Pre-requisitos
_Instrumentos y aplicaciones mínimas para la implementación._

* Node-RED
> **Nota** Para el correcto funcionamiento de node-RED y el nodo de XMPP

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

