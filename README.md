# PS4 Input Mapper

Proyecto separado del PS4 Cloud Browser para crear y administrar perfiles de teclado y mouse.

## Objetivo de la primera versión

- Aplicación PS4 independiente en formato PKG.
- Perfil inicial editable: **PC estándar — teclado y mouse**.
- Crear, duplicar, renombrar, seleccionar, importar, exportar y borrar perfiles.
- Editor de asignaciones con captura de tecla/botón y detección de conflictos.
- Ajustes independientes de movimiento y cámara.
- Prueba visual de entradas y guardado persistente.

## Perfil inicial

| Acción | Entrada |
|---|---|
| Movimiento | WASD |
| Cámara | Mouse |
| Ataque ligero (R1) | Clic izquierdo |
| Ataque pesado (R2) | Clic derecho |
| Bloquear con escudo (L1) | Q |
| Apuntar / lanzar hacha (L2) | Ctrl |
| Esquivar (Cross) | Espacio |
| Interactuar | E |
| Correr (L3) | Shift |
| Fijar objetivo (R3) | Clic central |
| Menú | Esc |

No se presupone que el mouse tenga botones laterales. El editor permitirá asignarlos manualmente si el dispositivo los ofrece.

## Estado técnico honesto

El repositorio contiene el formato JSON, el perfil inicial y un core portable de traducción con pruebas de escritorio. El puente HID conserva por separado el estado del teclado y del mouse, acumula el movimiento relativo una sola vez por muestra y permite limpiar cada dispositivo al desconectarlo. También incluye un contrato portable de transporte para separar el polling USB de la lógica del mapper; aún no hay backend USB específico de PS4.

**El editor PS4 aún es una interfaz inicial estática: no tiene navegación, captura ni guardado interactivo. Tampoco existe un sistema verificado de inyección de mando en juegos.** La aplicación de configuración y el runtime de entrada son componentes separados.

La compatibilidad universal con juegos que solo aceptan mando depende de validar una ruta de entrada en PS4/GoldHEN. El puente actual acepta informes HID Boot de teclado (8 bytes) y mouse (3 bytes); todavía no enumera USB en PS4 ni envía reportes al mando. No se afirmará compatibilidad hasta probarla en hardware real.

## Pruebas locales

Ejecuta la suite portable completa con:

```sh
sh scripts/test-portable.sh
```

El mismo comando se ejecuta automáticamente en GitHub Actions para cada push a `main` o a ramas `integration/**`, y en pull requests. Estas pruebas validan únicamente el core portable; no prueban acceso USB ni inyección de mando en PS4.

```sh
cc -std=c11 -Wall -Wextra -Werror -Iinclude \
  src/mapper.c tests/test_mapper.c -o mapper_test
./mapper_test
python3 tests/test_profile.py

cc -std=c11 -Wall -Wextra -Werror -Iinclude \
  src/hid_adapter.c src/hid_runtime.c tests/test_hid_runtime.c -o hid_runtime_test
./hid_runtime_test

cc -std=c11 -Wall -Wextra -Werror -Iinclude \
  src/mapper.c src/hid_adapter.c src/hid_runtime.c tests/test_hid_runtime_mapper.c -o hid_runtime_mapper_test
./hid_runtime_mapper_test
```

## Diseño del editor

Consulta [PROFILE_EDITOR.md](PROFILE_EDITOR.md) y [PROFILE_FORMAT.md](PROFILE_FORMAT.md). Para el puente HID consulta [HID_RUNTIME.md](HID_RUNTIME.md).
