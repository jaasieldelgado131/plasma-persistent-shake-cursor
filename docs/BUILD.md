# Compilar y probar

Estas instrucciones corresponden a Plasma **6.8.2**, Qt **6.12.0** y KDE Frameworks **6.31.0**, usados en la validación. Son necesarias las dependencias de desarrollo de los paquetes `kwin` y `plasma-desktop` de la distribución, incluidos CMake, Ninja, compilador C++, ECM, Qt y KDE Frameworks. También se requieren los paquetes de protocolos `wayland-protocols` y `plasma-wayland-protocols`.

## Preparar fuentes

Desde la raíz de este repositorio:

```bash
mkdir -p work/kwin-source
git clone --depth 1 --branch v6.8.2 https://invent.kde.org/plasma/kwin.git work/kwin-source/kwin
git clone --depth 1 --branch v6.8.2 https://invent.kde.org/plasma/plasma-desktop.git work/kwin-source/plasma-desktop
git -C work/kwin-source/kwin apply ../../../patches/arch-kwin-crash-fix.patch
git -C work/kwin-source/kwin apply ../../../patches/kwin-keep-size.patch
git -C work/kwin-source/plasma-desktop apply ../../../patches/accesibilidad-keep-size.patch
```

Los archivos de fuentes de Releases ya incluyen estos cambios; si los usas, no apliques los parches de nuevo. La corrección adicional de Arch/CachyOS sólo es pertinente al reconstruir también la biblioteca del compositor.

## Compilar

```bash
cmake -S work/kwin-source/kwin -B work/build-kwin -G Ninja \
  -DCMAKE_BUILD_TYPE=Release -DCMAKE_INSTALL_PREFIX=/usr -DBUILD_TESTING=ON
cmake --build work/build-kwin --target kwin_wayland testShakeCursor --parallel 4

cmake -S tools/accessibility -B work/build-access -G Ninja \
  -DCMAKE_BUILD_TYPE=Release -DCMAKE_INSTALL_PREFIX=/usr \
  -DPLASMA_DESKTOP_SOURCE_DIR="$PWD/work/kwin-source/plasma-desktop"
cmake --build work/build-access --parallel 4
```

La herramienta de CMake compila únicamente el módulo de Accesibilidad y el programa de comprobación, sin reconstruir todo plasma-desktop. Sigue las opciones del directorio `kcms/` de KDE.

No ejecutes `cmake --install` sobre la sesión de trabajo para probar el cambio. El ejecutable final debe usar las bibliotecas de la misma versión de KDE y no debe conservar rutas de búsqueda hacia el directorio de compilación.

## Prueba de integración del efecto

```bash
mkdir -p work/test-config work/test-data
XDG_CONFIG_HOME="$PWD/work/test-config" \
XDG_DATA_HOME="$PWD/work/test-data" \
XCURSOR_THEME=breeze_cursors XCURSOR_SIZE=24 XCURSOR_PATH=/usr/share/icons \
KWIN_COMPOSE=O2 QT_FORCE_STDERR_LOGGING=1 \
ctest --test-dir work/build-kwin -R '^kwin-testShakeCursor$' --output-on-failure
```

La prueba usa su propio compositor y sesión D-Bus. Comprueba el comportamiento por defecto y desactivado, la retención, el crecimiento posterior, la recuperación al desactivar, la activación con reducción pendiente y la restauración al descargar el efecto.

## Comprobación del panel

`work/build-access/bin/accessibility_check` carga el KCM real. Sus argumentos son la ruta absoluta de `kcm_access.so` y la ruta de la captura de pantalla. Debe ejecutarse con una configuración temporal y dentro de `dbus-run-session`: registra dobles de los servicios D-Bus de KWin y del lector de pantalla, verifica el guardado de ambos estados y renderiza `ShakeCursor.qml`.

Para aislar los programas externos del panel, usa un directorio temporal al principio de `PATH` con ejecutables `kaccess` y `orca` que terminen con código cero. En la validación se usaron `QT_QPA_PLATFORM=offscreen`, `QT_QUICK_BACKEND=software`, `QT_QUICK_CONTROLS_STYLE=org.kde.breeze`, `QT_STYLE_OVERRIDE=Fusion` y `LANGUAGE=es`, junto con `XDG_CONFIG_HOME`, `XDG_DATA_HOME` y `XDG_RUNTIME_DIR` temporales. El archivo español `po/es/kcm_access.po` se compiló con `msgfmt` en `XDG_DATA_HOME/locale/es/LC_MESSAGES/kcm_access.mo`.

## Paquetes descargables

Los paquetes de v1.0.0 se generaron conservando el contenido de los paquetes originales de CachyOS y reemplazando únicamente los cuatro archivos descritos en el README. Se eliminaron las rutas de compilación del ejecutable, se actualizaron los metadatos y se regeneró `.MTREE`. No se alteró la biblioteca `libkwin` de CachyOS.

Los scripts en `packaging/` corresponden al archivo instalable de Releases. Necesitan los paquetes y `SHA256SUMS` incluidos en esa descarga; el clon del repositorio contiene sólo su código.
