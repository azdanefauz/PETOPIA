# PETOPIA

Game virtual pet sederhana (kucing, anjing, kelinci) dibuat pakai C dan raylib.

## Fitur
- Welcome Screen
- Pet Selection
- Pet Naming
- Main Game
- Activity Screen (FEED, PLAY, REST)

## Tim
- Azdan: kodingan (Pet Selection, Pet Naming)
- Zaky: setup, font, asset folder, image dan sprite place holder, kitchen, backyard, bedroom dan lainnya
- Tami: desain UI dan aset gambar

## Cara compile (Mac)
brew install raylib pkg-config
gcc src/*.c -o petopia $(pkg-config --cflags --libs raylib)
./petopia
