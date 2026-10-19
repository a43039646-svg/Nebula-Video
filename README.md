# Nebula Video

Nebula Video is an early Linux desktop app that combines a web browser with a library for local videos.

## First version

- Browse websites in an embedded Chromium-based view.
- Size the window to the current screen and adapt the sidebar and home shortcut grid for narrower displays.
- Switch between Russian and English from **Settings**. The selection also changes the preferred language sent to websites and applies on restart.
- Choose a distribution-inspired interface theme, or let Nebula Video select one from the Linux distribution ID. Presets include Ubuntu, Fedora, Arch, Debian, Linux Mint, openSUSE, KDE Breeze and Nebula.
- Manage saved VPN profiles from **VPN** using NetworkManager; import OpenVPN `.ovpn` files and connect or disconnect existing VPN profiles.
- Choose embedded audio and subtitle tracks for local videos when the media file and Qt multimedia backend expose those tracks.
- Check page image and HTML5 video loading from the toolbar's **?** button; page loading progress is shown in the status bar.
- Block requests to a built-in list of common advertising and tracking hosts; the toolbar toggle shows the session count and reloads the page when changed.
- Start from quick links to YouTube, Netflix, Crunchyroll and YummyAnime TV; add personal site shortcuts that are saved on the device.
- Enter a web address or search phrase in the address bar.
- Find local videos in the standard Movies and Downloads folders on first launch; additional folders can be imported.
- Scan imported folders recursively and filter the video list.
- Play a selected file in the app.
- Remember imported folders between launches.

The app does not fetch or index films from websites. Its local library scans the standard Movies and Downloads folders, plus any folders the user adds.

## Linux distribution support

Nebula Video is a Qt 6 application and can be built on Linux distributions that package Qt 6 Widgets, Qt WebEngine and Qt Multimedia. This repository provides DEB generation through CPack, RPM generation when `rpmbuild` is installed, a PKGBUILD for Arch Linux, and a Flatpak manifest. CPack also creates a TGZ archive, which uses the Qt libraries installed on the target system and is not a self-contained AppImage.

Install CMake, a C++17 compiler and the Qt 6 development modules **Widgets**, **WebEngineCore**, **WebEngineWidgets**, **Multimedia** and **MultimediaWidgets** using your distribution's package manager. Package names vary by distribution.

Configure, build and run:

```sh
cmake -S . -B build -DCMAKE_BUILD_TYPE=Release
cmake --build build -j
./build/nebula-video
```

To install Nebula Video in the desktop applications menu with its icon, run:

```sh
sudo cmake --install build
```

After installation, launch **Nebula Video** from your desktop's application menu.
Create the available distribution packages:

```sh
cmake --build build --target package
```

The generated packages are placed in `build/`. On Debian-family systems, install `dpkg-dev` for DEB dependency discovery. On RPM-family systems, install `rpm-build` to enable RPM generation. The package target always creates a TGZ archive.

On Arch Linux or an Arch-based distribution, build and install the package from the latest GitHub source with `makepkg`:

```sh
git clone https://github.com/a43039646-svg/Nebula-Video.git
cd Nebula-Video/packaging/arch
makepkg -si
```

This VCS package tracks the `main` branch. Arch package maintainers can use the recipe as a starting point for a versioned release package.

For one package format that works across distributions, build and install the Flatpak. Install Flatpak and `flatpak-builder` from your distro, then add Flathub and its Qt runtime:

```sh
flatpak remote-add --if-not-exists flathub https://flathub.org/repo/flathub.flatpakrepo
flatpak install flathub org.kde.Platform//6.10 org.kde.Sdk//6.10
flatpak-builder --user --install --force-clean --state-dir=/tmp/nebula-video-flatpak-state /tmp/nebula-video-flatpak-build flatpak/org.nebula.Video.yml
flatpak run org.nebula.Video
```

The Flatpak can browse the web and read the standard Videos and Downloads folders. When adding another media folder, grant access through the file chooser. VPN management remains optional and uses host NetworkManager, so it is not available inside the Flatpak sandbox.

Themes are stored in the app's user settings. **Automatic** uses the distro ID reported by Qt and falls back to the Nebula theme if the distribution is not recognized. Theme colors apply immediately; the language setting still requires restarting the app.

## Build dependencies

On Debian or Ubuntu, install the build tools and Qt 6 development packages:

```bash
sudo apt install build-essential cmake qt6-base-dev qt6-webengine-dev qt6-multimedia-dev
```

VPN controls use NetworkManager profiles. To import OpenVPN configurations, install NetworkManager's OpenVPN plugin as well as the provider's `.ovpn` profile. The app does not include a VPN provider or service. Existing WireGuard profiles managed by NetworkManager can also be activated here.

VPN integration is optional and requires NetworkManager plus `nmcli`; the rest of the application does not require it. The application menu entry and icon are installed from CMake on all supported Linux distributions.

Qt WebEngine uses Chromium. Run the app as your normal desktop user, not as root.

The browser profile keeps cookies and site data on this device, and automatic image loading is enabled. The built-in ad blocker is enabled by default and can be switched off from the toolbar if a site feature stops working. Browser pages use the preferred language selected in Settings; each website controls the language options shown in its own video player. Video sites can request fullscreen, and user-opened pop-up player windows are redirected into the current page. Use the **?** button to inspect codec support, visible video players, failed page images and player state. Use **System browser** in the toolbar if a site does not play correctly in the embedded view.

## Current limitations

- This is an initial prototype; downloads, browser tabs, custom collections and full bookmark management are not implemented yet. Embedded audio/subtitle track controls apply to local files, not a website's custom player.
- Video playback depends on the codecs available in the installed Qt WebEngine build. MP4/H.264 may be unavailable in builds without proprietary codecs, and DRM playback needs Widevine, which Qt WebEngine does not ship with.
- Some websites may reject embedded browsers or require DRM support that this app does not provide. **System browser** opens the current page in the desktop's normal browser as a fallback.
- A missing film poster may be caused by the site's image server, lazy loading, or the site rejecting embedded browsers. The **?** report checks regular page images; it cannot inspect images hidden in cross-origin frames or CSS backgrounds.
- The built-in blocker uses a small fixed host list rather than a full EasyList subscription, so it will miss some ads. It does not remove ad elements injected directly by the website itself.

## License

Nebula Video is free software licensed under the GNU General Public License, version 3 or (at your option) any later version. See [LICENSE](LICENSE).
