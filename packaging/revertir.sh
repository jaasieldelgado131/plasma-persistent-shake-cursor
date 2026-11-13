#!/usr/bin/env bash
set -euo pipefail
bundle_dir=$(cd -- "$(dirname -- "${BASH_SOURCE[0]}")" && pwd)
cd -- "$bundle_dir"
if [[ $(pacman -Q kwin) != 'kwin 6.8.2-2.1.1' || $(pacman -Q plasma-desktop) != 'plasma-desktop 6.8.2-1.1.1' ]]; then
    echo 'Los paquetes instalados no son las versiones locales esperadas. No se hará una bajada automática de versión.' >&2
    exit 1
fi
sha256sum --check --quiet SHA256SUMS
sudo -- pacman -U -- \
    "$bundle_dir/originales/kwin-6.8.2-2.1-x86_64_v3.pkg.tar.zst" \
    "$bundle_dir/originales/plasma-desktop-6.8.2-1.1-x86_64_v3.pkg.tar.zst"
echo 'Restaurados los paquetes originales. Cierra sesión y vuelve a entrar cuando te convenga.'
