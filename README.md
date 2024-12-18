# Kate discord RPC plugin
---

Simple plugin to add discord rich presence for kate :3

![Screenshot of the plugin on a discord profile](./img/screenshot_01.png)

---
## Compilation instructions

Dependencies: a c++ compiler, cmake, Qt and KDE frameworks headers

run:

```sh
cmake -B build
cmake --build build
sudo cmake --install build
```
this should install a single file to the KF6 KTextEditor plugin path

(`/usr/lib/qt6/plugins/kf6/ktexteditor/katediscordrpcplugin.so` on arch for example, different distros may have different paths)

to uninstall it simply remove that file

---

### TODO:
 - [x] Config GUI to format the presence fields
 - [ ] More fields if possible (session name, LSP errors amount, language, etc...)
 - [ ] Idle detection
 - [ ] What to show when the formatted fields only have empty variables
 - [x] Installation instructions
 - [ ] A Pacman PKGBUILD (if someone wants to PR packaging files for other distros feel free)
