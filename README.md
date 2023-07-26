# Laboratorio 2 Gráficas
Se utilizó SDL y C++ para modular el juego Game of Life de Conway

## Windows MSYS2 MINGW64
Para poder correr el archivo se utilizó MSYS2 MINGW64 para poder ejecutar el codigo, proporcionaré un video de como fue el proceso de la descarga de SDL en Mingw63:
https://www.youtube.com/watch?v=l_yjcm5dihg

para poder correr el código en MSYS2 MINGW64 se necesita el siguiente comando:
```shell
gcc test.c -o test.exe $(pkg-config --cflags --libs sdl2)
```
Esto generara un archivo llamado test.exe el cual podran hacer doble click y entrar para verlo en acción.
Se deberia de ver algo asi:

![juegodevida](https://github.com/GarciaAlegria/Laboratorio2graficas/assets/84537086/7589f3c2-d4e2-46ef-85cb-e539eef34035)
