# SimulacionFisicaVideojuegos
Repositorio de las prácticas de la asignatura de Simulación Física de Videojuegos

En la carpeta skeleton estará vuestro código fuente

Necesitaréis además descomprimir el archivo que se indica a continuación en la raíz del repositorio:

Carpetas bin y common https://ucomplutense-my.sharepoint.com/:u:/g/personal/davalejo_ucm_es/EZcsdyIoqTFOrF4hdfkVYk0BJZ9IycJ3QqXQFwaXVrlSKg?e=pfs6IO

---

### Proyecto final
<ins>Input</ins>

&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;La cámara está en primera persona, de modo que sigue al puntero del ratón. El input es el siguiente:
- Se utilizan las teclas **IJKL** para **desplazarse en todas las direcciones**. El jugador solo puede desplazarse en la tierra, pero no mientras está en el aire.
- La acción de que la cámara siga al puntero del ratón se puede intercalar. Si se clica en la pantalla, se parará esta acción si no lo estaba y si estaba parada, se activará.
- Se utiliza la **barra espaciadora** para **saltar**. Se puede saltar tanto en el sitio como en carrera.

##### Modo DEBUG
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;Se puede activar o desactivar cambiando el valor de la **constante *DEBUG*** que se encuentra en la **clase *Scene***.

---

### Práctica 5
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;Se han incluido con solidos rigidos dos generadores (lluvia uniforme y fuente guassiana) y una fuerza (explosion). Se ha trabajado con materiales, diferentes formas de configurar la masa (tensor de inercia, dando un valor a la densidad)... Ademas, se ha modificado la estructura base para que sea muy sencillo incluir nuevos generadores/fuerzas que funcionen con solidos rigidos (mismas clases tanto para particulas como para solidos rigidos).

&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;Inicialmente, se muestran los dos generadores en funcionamiento y el area donde afecta la explosion. Aunque se muestra el area, inicialmente esta desactivada.

<ins>Input</ins>
- Z --> activar/desactivar lluvia uniforme
- X --> activar/desactivar fuente gaussiana
- C --> activar/desactivar fuerza de la explosion
- V --> ejecutar explosion

&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;Por lo tanto, para que al principio funcione la explosion primero hay que activarla (C) y luego, ejecutarla (V).
