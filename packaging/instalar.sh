#!/usr/bin/env bash
set -euo pipefail
bundle_dir=$(cd -- "$(dirname -- "${BASH_SOURCE[0]}")" && pwd)
cd -- "$bundle_dir"
trap 'result=$?; printf "%s\n" "$result" > "$bundle_dir/estado-instalacion.txt"' EXIT

if [[ $(pacman -Q kwin) == 'kwin 6.8.2-2.1.1' && $(pacman -Q plasma-desktop) == 'plasma-desktop 6.8.2-1.1.1' ]]; then
    echo 'La opción ya está instalada.'
    exit 0
fi
if [[ $(pacman -Q kwin) != 'kwin 6.8.2-2.1' || $(pacman -Q plasma-desktop) != 'plasma-desktop 6.8.2-1.1' ]]; then
    echo 'Las versiones de KDE cambiaron. Este paquete requiere KWin 6.8.2-2.1 y plasma-desktop 6.8.2-1.1.' >&2
    exit 1
fi
sha256sum --check --quiet SHA256SUMS
echo 'Se añadirá «Conservar el tamaño ampliado al dejar de agitar» en Accesibilidad.'
echo 'La casilla estará desactivada por defecto. No se cerrará tu sesión.'
echo 'Los paquetes originales están en la carpeta originales/ para poder revertir.'
sudo -- pacman -U -- \
    "$bundle_dir/kwin-6.8.2-2.1.1-x86_64_v3.pkg.tar.zst" \
    "$bundle_dir/plasma-desktop-6.8.2-1.1.1-x86_64_v3.pkg.tar.zst"
echo
echo 'Instalación completada. Cierra sesión y vuelve a entrar cuando te convenga.'
echo 'Después: Preferencias del sistema → Accesibilidad → Sacudir el puntero.'
echo 'Activa la nueva casilla y pulsa Aplicar. Desmárcala y aplica para volver al tamaño normal.'
