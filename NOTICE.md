# Licencias y procedencia

Este proyecto modifica componentes de KDE Plasma. No es una versión oficial de KDE ni de CachyOS.

Fuentes base:

- [KWin v6.8.2](https://invent.kde.org/plasma/kwin/-/tree/v6.8.2), commit `f130ab33c8e6f3b9d728aade87836086ae9a8ed4`.
- [plasma-desktop v6.8.2](https://invent.kde.org/plasma/plasma-desktop/-/tree/v6.8.2), commit `3a67a04b656bda45b2c175b5f12bb7e4222d5026`.
- [Corrección de KWin usada por el paquete Arch/CachyOS](https://invent.kde.org/plasma/kwin/-/commit/c9d720793bbcef9483ab290673032d39441c0e6f), SHA-256 del parche `667d62447fdbc9fe0a760efee7c44e96c988384a5ad86fac49be7576d52010c2`.

Las modificaciones y herramientas nuevas de este repositorio se ofrecen bajo GPL-2.0-or-later, salvo los archivos derivados de KDE que conservan su licencia original. Los parches no eliminan ni sustituyen las declaraciones SPDX o los derechos de sus autores.

El efecto de KWin y su detector de movimientos fueron escritos por Vlad Zahorodnii y se distribuyen bajo GPL-2.0-or-later. El módulo de Accesibilidad conserva las licencias declaradas en sus archivos, incluida `LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL` para `kcmaccess.cpp`. Las copias pertinentes están en `LICENSES/`; los árboles completos de código fuente incluyen las declaraciones de los demás archivos.

La descarga instalable conserva los paquetes originales de CachyOS excepto el ejecutable de KWin, el módulo de Accesibilidad, su esquema de configuración y la traducción española, además de los metadatos del paquete. La biblioteca `libkwin` y sus correcciones siguen siendo las originales de CachyOS.

Las versiones publicadas incluyen árboles de código fuente con los parches aplicados. En el árbol de KWin también se conserva la corrección de Arch/CachyOS citada arriba. Los paquetes locales modificados no están firmados por CachyOS.
