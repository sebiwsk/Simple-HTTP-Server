# Simple HTTP Server in C

This is a simple HTTP server implemented in C. It listens for HTTP requests on port 42069 and serves a static `index.html` file in response to GET requests. The server also handles basic error responses for unsupported HTTP methods and invalid requests.

## Features

- Handles GET and POST HTTP methods (only GET is functional).
- Serves a static `index.html` file for valid GET requests.
- Responds with appropriate HTTP status codes for unsupported methods and invalid requests.
- Configurable port and maximum number of clients.

## Requirements

- A Unix-like operating system (Linux, macOS, etc.).
- GCC (GNU Compiler Collection).

## How to Use

### Compile the Server

To compile the server, run the following command in your terminal:

```sh
gcc -o simple_http_server simple_http_server.c
```

### Run the Server

After compiling, you can start the server with:

```sh
./simple_http_server
```

The server will start listening for connections on port 42069.

### Access the Server

Open a web browser and navigate to http://localhost:42069 to see the server in action. If an index.html file is present in the same directory as the executable, it will be served in response to GET requests.

## Code Explanation

### Main Function

The main function sets up the server socket, binds it to the specified port, and starts listening for incoming connections. It then enters an infinite loop, accepting new connections and handling HTTP requests.

### Handling HTTP Requests

The handle_http_request function reads the incoming request, parses the HTTP method and URI, and serves the index.html file if the request is valid. If the requested method is not supported or the request is invalid, appropriate HTTP error responses are sent.

## License

This project is licensed under the MIT License. Feel free to use and modify the code as you wish.

## Author
sebiwsk
