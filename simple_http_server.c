#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>

#define PORT 42069
#define MAX_CLIENTS 5
#define BUFFER_SIZE 1024

void handle_http_request(int new_socket) {
	char buffer[BUFFER_SIZE] = { 0 };
	int bytesread = read(new_socket, buffer, BUFFER_SIZE);
	buffer[bytesread] = 0;

	printf("Received request: %s\n", buffer);

	char* uri_start = strpbrk(buffer, " ");
	if (uri_start != NULL) {
		uri_start++;
		char* uri_end = strpbrk(uri_start, " \r");
		if (uri_end != NULL) {
			size_t uri_length = uri_end - uri_start;
			char uri_buffer[uri_length + 1];
			strncpy(uri_buffer, uri_start, uri_length);
			uri_buffer[uri_length] = '\0';

			printf("Requested URI: %s\n", uri_start);

			if (memcmp(buffer, "GET", 3) == 0) {
				printf("Method: GET\n");
			}
			else if (memcmp(buffer, "POST", 4) == 0) {
				printf("Method: POST\n");
			}
			else {
				perror("Unsupported HTTP method\n");
				char* not_found_response = "HTTP/1.1 405 Method Not Allowed\r\nContent-Type: text/html\r\n\r\n<h1>405 Method Not Allowed</h1>";
				write(new_socket, not_found_response, strlen(not_found_response));
				close(new_socket);
				return;
			}
		}
		else {
			perror("Invalid HTTP request: Missing end of URI\n");
			char* not_found_response = "HTTP/1.1 400 Bad Request\r\nContent-Type: text/html\r\n\r\n<h1>400 Bad Request</h1>";
			write(new_socket, not_found_response, strlen(not_found_response));
			close(new_socket);
			return;
		}
	}
	else {
		perror("Invalid HTTP request: Missing URI\n");
		char* not_found_response = "HTTP/1.1 400 Bad Request\r\nContent-Type: text/html\r\n\r\n<h1>400 Bad Request</h1>";
		write(new_socket, not_found_response, strlen(not_found_response));
		close(new_socket);
		return;
	}

	FILE* file = fopen("index.html", "r");
	if (file == NULL) {
		perror("Error opening index.html\n");
		char* not_found_response = "HTTP/1.1 404 Not Found\r\nContent-Type: text/html\r\n\r\n<h1>404 Not Found</h1>";
		write(new_socket, not_found_response, strlen(not_found_response));
		close(new_socket);
		return;
	}

	char response[BUFFER_SIZE * 10];
	size_t bytesRead = fread(response, 1, sizeof(response), file);
	fclose(file);

	char http_response[BUFFER_SIZE + sizeof("HTTP/1.1 200 OK\r\nContent-Type: text/html\r\n\r\n")];

	if (bytesRead == 0) {
		snprintf(http_response, sizeof(http_response), "HTTP/1.1 404 Not Found\r\nContent-Type: text/html\r\n\r\n<h1>404 Not Found</h1>");
	}
	else {
		snprintf(http_response, sizeof(http_response), "HTTP/1.1 200 OK\r\nContent-Type: text/html\r\n\r\n%.*s", (int)bytesRead, response);
	}

	write(new_socket, http_response, strlen(http_response));

	close(new_socket);
}

int main() {
	int server_fd, new_socket;
	struct sockaddr_in address;
	int opt = 1;
	int addrlen = sizeof(address);

	if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0) {
		perror("Socket creation failed\n");
		exit(EXIT_FAILURE);
	}

	if(setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &opt, sizeof(opt))) {
		perror("Setsockopt failed\n");
		exit(EXIT_FAILURE);
	}

	address.sin_family = AF_INET;
	address.sin_addr.s_addr = INADDR_ANY;
	address.sin_port = htons(PORT);

	if (bind(server_fd, (struct sockaddr *)&address, sizeof(address)) < 0) {
		perror("bind failed");
		exit(EXIT_FAILURE);
	}

	if (listen(server_fd, MAX_CLIENTS) < 0 ) {
		perror("listen failed");
		exit(EXIT_FAILURE);
	}

	printf("Server listening on port %d\n", PORT);
	printf("_____________\n\n");

	while (1) {
		if ((new_socket = accept(server_fd, (struct sockaddr *)&address, (socklen_t*)&addrlen))<0) {
			perror("accept failed");
			continue;
		}

		handle_http_request(new_socket);
	}
	return 0;
}
