# Client-Server-Ejemplo

Ejemplo básico de una arquitectura **cliente-servidor** implementada en **C** usando **sockets UDP**. El servidor hospeda un juego de adivinar un número; el cliente se conecta, elige la dificultad e intenta acertar el número con pistas del servidor.

## Índice

- [Descripción](#descripción)
- [Estructura del proyecto](#estructura-del-proyecto)
- [Requisitos](#requisitos)
- [Compilación](#compilación)
- [Uso](#uso)
- [Flujo del juego](#flujo-del-juego)
- [Socket Facade](#socket-facade)

---

## Descripción

La arquitectura cliente-servidor es un modelo de diseño de software en el que las tareas se reparten entre:

- **Servidor**: proveedor del servicio. Genera un número aleatorio y valida los intentos del cliente.
- **Cliente**: demandante del servicio. Se conecta al servidor, elige la dificultad e intenta adivinar el número.

La comunicación se realiza mediante **sockets UDP** (`SOCK_DGRAM`) sobre IPv4.

---

## Estructura del proyecto

```
Client-Server-Ejemplo/
├── codigo/
│   ├── client.c          # Lógica del cliente
│   └── server.c          # Lógica del servidor
├── socket_facade/
│   ├── socket_facade.h   # Interfaz de la fachada de sockets
│   └── socket_facade.c   # Implementación de la fachada de sockets
├── ejecutables/          # Binarios generados por make (no versionados)
├── Makefile
└── README.md
```

---

## Requisitos

- GCC (o cualquier compilador compatible con C99)
- Sistema operativo Linux/Unix (se necesitan las cabeceras POSIX de sockets)
- `make`

---

## Compilación

Desde la raíz del proyecto:

```bash
make
```

Esto compilará los tres targets:

| Target           | Descripción                        |
|------------------|------------------------------------|
| `socket_facade.o`| Objeto compartido de la fachada    |
| `server`         | Ejecutable del servidor            |
| `client`         | Ejecutable del cliente             |

Los binarios se generan en la carpeta `ejecutables/`.

Para limpiar los binarios:

```bash
make clean
```

---

## Uso

### 1. Arrancar el servidor

```bash
./ejecutables/server <puerto>
```

Ejemplo:

```bash
./ejecutables/server 8080
```

### 2. Arrancar el cliente (en otra terminal)

```bash
./ejecutables/client <ip_servidor> <puerto>
```

Ejemplo (servidor en la misma máquina):

```bash
./ejecutables/client 127.0.0.1 8080
```

---

## Flujo del juego

1. El cliente envía su **nombre** al servidor.
2. El servidor responde con un **mensaje de bienvenida** y las opciones de dificultad:
   - **1** → número entre 1 y 10
   - **2** → número entre 1 y 50
   - **3** → número entre 1 y 100
3. El cliente elige la dificultad.
4. El servidor genera un **número aleatorio** en el rango seleccionado.
5. El cliente introduce intentos; el servidor responde si el número es **mayor** o **menor**.
6. Al acertar, el servidor envía un mensaje de **felicitación** y la partida termina.
7. El servidor vuelve a esperar nuevos clientes.

Diagrama de flujo de la ejecución del cliente y el servidor:

<p align="center">
  <img src="https://github.com/user-attachments/assets/7375e567-79f4-426c-a795-49d123772f58" alt="Diagrama de flujo cliente-servidor" width="500">
</p>

---

## Socket Facade

El módulo `socket_facade` encapsula las llamadas al sistema de sockets en funciones de alto nivel para simplificar el código del cliente y del servidor:

| Función | Descripción |
|---|---|
| `get_socket()` | Crea y devuelve un socket UDP |
| `get_address(ip, port)` | Construye un `struct sockaddr_in` con la IP y puerto indicados |
| `bind_socket(socket, address)` | Enlaza el socket a la dirección dada |
| `send_data(socket, data, address)` | Envía datos al destinatario especificado |
| `receive_data(socket, data, address)` | Recibe datos (opcionalmente guarda la dirección del remitente) |
