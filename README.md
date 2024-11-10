# Atlas Engine

### [Github](https://github.com/Serfercont/AtlasEngine)

## Descripción
Atlas Engine es un motor de videojuegos desarrollado en **SDL** y **OpenGL** que permite cargar y editar modelos FBX de forma sencilla. Además, facilita la importación de texturas, ofreciendo controles de cámara estilo Unity y ventanas de editor que simplifican la creación de escenas 2D y 3D.

## Miembros
- [Carlos González](https://github.com/gosu00)
- [Sergio Fernández](https://github.com/Serfercont)
- [Pau Blanco](https://github.com/PauloWhite2004)

## Características
- **Carga Automática de Modelo**: El modelo `Baker_house.FBX` se carga al inicio del motor.
- **Soporte para Modelos FBX con Textura Difusa**: Los modelos se renderizan en tiempo real con una textura difusa (de un canal).
- **Arrastrar y Soltar (Drag and Drop)**:
  - **Modelos**: Puedes cargar archivos FBX desde carpetas del sistema arrastrándolos al motor.
  - **Texturas**: Soporta texturas en formato PNG y DDS, aplicables a los objetos cargados.
- **Estructura de GameObject**: Cada modelo cargado se instancia como un GameObject con tres componentes:
  - **Transform**: Posición, rotación y escala del objeto.
  - **Mesh**: Representación de la geometría del objeto.
  - **Texture**: Información sobre la textura aplicada.
- **Controles de Cámara Estilo Unity**:
  - **Movimiento FPS**: Con clic derecho y teclas **WASD** y subir con la tecla **Q** y bajas con **E**.
  - **Zoom**: Con la rueda del ratón.
  - **Órbita**: `Alt` + clic izquierdo para orbitar el objeto.
  - **Centrado**: Tecla **F** para centrar la cámara en el objeto.
  - **Velocidad Duplicada**: Modo acelerado con **Shift**.
- **Ventanas del Editor**:
  - **Consola**: Log del proceso de carga de geometría (usando ASSIMP) y de inicialización de bibliotecas.
  - **Configuración**: Incluye un gráfico de FPS en tiempo real, configuración de variables de cada módulo (renderizado, entrada, etc.), y detalles de consumo de memoria y hardware.
  - **Jerarquía**: Lista y permite seleccionar todos los GameObjects de la escena.
  - **Inspector**: Información sobre Transform, Mesh y Texture del objeto seleccionado aún no funcional.
- **Menús**:
  - Menú general con opciones para salir, abrir GitHub y ver información del motor.
  - Menú para cargar formas básicas y para activar o desactivar ventanas del editor.

## Funcionalidades Extras
- **Docking**: Se puede organizar las ventanas del proyecto a tu gusto con un sistema de Docking.
## Futuras Implementaciones:
- **Funcionalidad del inspector**: Los GameObjects podrán ser modificados (Posición, Escala...)
- **Se mejorará la camara**: Se cambiaran los controles haciéndose más intuitivos y se aplicará el **Culling** entre otras cosas.
