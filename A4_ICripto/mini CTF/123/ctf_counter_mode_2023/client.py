"""A simple interactive client. For non-interactive use, provide user_input
argument to client.input()."""

import argparse
import socket


class Client:
    """A simple client for our server"""

    def __init__(self, host: str, port: int) -> None:
        self._sock = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
        self._sock.connect((host, port))
        # Disable Nagle's algorithm to improve performance
        self._sock.setsockopt(socket.IPPROTO_TCP, socket.TCP_NODELAY, 1)
        self._sock_file = self._sock.makefile("rbw", buffering=0)

    def readline(self) -> str:
        """Read bytes until it reaches '\\n'. The output includes the newline
        character.
        """
        return self._sock_file.readline()

    def send(self, message: str | bytes) -> None:
        """Send some data to server. In our protocol, the message ends with
        newline.
        """
        if isinstance(message, bytes):
            self._sock_file.write(message + b"\n")
        else:
            self._sock_file.write(f"{message}\n".encode())

    def print_menu(self) -> None:
        """Print the menu to stdout."""
        for _ in range(7):
            print(self.readline(), end="")

    def input(self, user_input: str | bytes | None = None) -> str | bytes:
        """Reads the input prompt from the socket, and sends the user input.
        User input can be either provided as `user_input` argument, or from
        stdin.

        Args:
            user_input (str | None, optional): Optional user input. If not
                provided, will read from stdin. Defaults to None.

        Returns:
            str: The user input. Useful when user_input=None.
        """
        prompt = self.readline().strip()

        if user_input is None:
            user_input = input(f"{prompt} ")
        user_input = user_input.strip()

        self.send(user_input)
        return user_input

    def __del__(self) -> None:
        try:
            self._sock_file.close()
            self._sock.close()
        except:
            pass


def parse_args() -> argparse.Namespace:
    parser = argparse.ArgumentParser()
    parser.add_argument(
        "--host",
        type=str,
        default="localhost",
        help="The host (default: localhost)",
    )
    parser.add_argument(
        "--port",
        type=int,
        default=9776,
        help="The port (default: 80)",
    )
    return parser.parse_args()


def main(args: argparse.Namespace) -> None:
    client = Client(args.host, args.port)
    client.print_menu()

    while True:
        # Choose menu item
        choice = client.input()

        if choice == "1":
            # Get encrypted flag
            print(client.readline())

        elif choice == "2":
            # Send plaintext
            client.input()

            # Get encrypted plaintext
            print(client.readline())

        else:
            break

    # Print "bye" message
    print(client.readline())


if __name__ == "__main__":
    main(parse_args())
