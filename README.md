# What is this ?
## OCR Project :
- Contains, a picture pretreatment
- A neural network that learn the XOR function
- A sudoku solver
- An image cutter

# For what ?
- To solve a sudoku grid based only on a picture

# Requirements:
#### SDL2
- IMPORTANT : You will need the file found at https://raw.githubusercontent.com/davidsiaw/SDL2_image/refs/heads/master/SDL_image.h
```shell
# Using package manager :
sudo apt-get install libsdl2-dev

# OR

# Commands given on SDL2 installation guide
git clone https://github.com/libsdl-org/SDL.git -b SDL2
cd SDL
mkdir build
cd build
../configure
make
sudo make install

# Command to get SDL_image.h missing file
sudo wget -O /usr/local/include/SDL2/SDL_image.h https://raw.githubusercontent.com/davidsiaw/SDL2_image/refs/heads/master/SDL_image.h
```

#### GTK3+
- To install GTK3+, you can refer to the following : https://github.com/GNOME/gtk
Everything is explained well and there is many prerequesites
- Even simpler, it is possible to install it with your package manager
```shell
sudo apt install libgtk-3-dev
```


#### To compile the project, you can use
```shell
make
```

### Then to use it :
```shell
./ocr
```
