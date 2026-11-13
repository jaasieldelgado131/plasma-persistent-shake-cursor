# Verificación de v1.0.0

Entorno: CachyOS Linux, x86_64_v3, Wayland, Plasma 6.8.2, Qt 6.12.0 y KDE Frameworks 6.31.0.

| Comprobación | Resultado |
|---|---|
| Prueba contra KDE original | Falló la retención con `KeepSize=true`; los casos del comportamiento original pasaron. |
| Prueba contra KWin modificado | Pasaron todos los escenarios de `kwin-testShakeCursor`, incluida la activación con reducción pendiente y la restauración al descargar el efecto. |
| Bibliotecas de CachyOS | La prueba final también pasó usando la `libkwin` instalada de la distribución. |
| KCM | Compilación correcta; valor por defecto desactivado, seguimiento de cambios, guardado de ambos estados y llamadas de reconfiguración comprobados. |
| Interfaz | Se renderizó el panel real en español; la captura está en `docs/accessibility.png`. |
| Ejecutable | Arranque y salida correctos en un compositor virtual, con las bibliotecas instaladas. |
| Contenido de los paquetes | Sólo cambian los cuatro archivos previstos y los metadatos. |
| Integridad después de instalar | `kwin`: 2280 archivos, 0 alterados; `plasma-desktop`: 4873 archivos, 0 alterados según `pacman -Qkk`. |

La instalación se comprobó con la opción desactivada y sin reiniciar la sesión real. Para activar el ejecutable nuevo es necesario volver a iniciar sesión. No se afirma una validación manual de la sesión de escritorio después de ese reinicio.

No se ejecutó toda la batería de pruebas de KDE; se ejecutaron las pruebas específicas del efecto, la comprobación del KCM y las validaciones de carga, enlace e integridad de los paquetes.
